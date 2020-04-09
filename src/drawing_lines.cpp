/*
 * drawing_lines.cpp
 *
 *  Created on: 08.04.2020
 *      Author: martin
 */

#include "fltk_std.h"
#include <fltk_ext/Line.h>
#include <fltk_ext/Canvas.h>
#include <fltk_ext/DragBox.h>
#include <fltk_ext/Line.h>
#include <vector>
#include <stdexcept>

using namespace std;

class MyConnection {
public:
	MyConnection( DragBox* box1, DragBox* box2 ) {
		_box1 = box1, _box2 = box2;
	}

	inline bool isElementOf( DragBox* box, int x, int y ) const {
		if( x < box->x() || x > box->x() + box->w() ) return false;
		if( y < box->y() || y > box->y() + box->h() ) return false;
		return true;
	}

	bool boxesInTouch() {
		int x21 = _box2->x();
		int y21 = _box2->y();
		if( isElementOf( _box1, x21, y21 ) ) return true;
		int x22 = x21 + _box2->w();
		if( isElementOf( _box1, x22, y21 ) ) return true;
		int y22 = _box2->y() + _box2->h();
		if( isElementOf( _box1, x22, y22 ) ) return true;
		if( isElementOf( _box1, x21, y22 ) ) return true;
		//still here ==> _box2 is either completey contained by _box1
		//or they don't touch each other at all.
		//One additional check is enough:
		if( isElementOf( _box2, _box1->x(), _box1->y() ) ) return true;

		return false;
	}

	void draw() {
		//don't try to draw any line when boxes are in contact:
		if( boxesInTouch() ) return;
		int x1, y1, x2, y2;
		_box1->getCenter( x1, y1 );
		_box2->getCenter( x2, y2 );
		Line line( x1, y1, x2, y2 ); //the line to draw
		//we don't want it to be drawn from center to center but
		//from border intersection to border intersection.
		Intersection X1 = getIntersection( _box1, line );
		if( !X1.intersects ) {
			char buf[300];
			buf[299] = 0x00;
			snprintf( buf, 299,
					 "Connection::draw(): line doesn't intersect box1.\n"
					 "Line: x1/y1, x2/y2, parallel: %d/%d, %d/%d, %s\n",
					 x1, y1, x2, y2, X1.linesParallel ? "true" : "false" );
			throw runtime_error( buf );
		}

		Intersection X2 = getIntersection( _box2, line );
		if( !X2.intersects ) {
			char buf[300];
			buf[299] = 0x00;
			snprintf( buf, 299,
					 "Connection::draw(): line doesn't intersect box2.\n"
					 "Line: x1/y1, x2/y2, parallel: %d/%d, %d/%d, %s\n",
					 x1, y1, x2, y2, X2.linesParallel ? "true" : "false" );
			throw runtime_error( buf );
		}

		fl_line_style( FL_SOLID, 3 );
		Fl_Color memo = fl_color();
		Fl_Color color = _isSelected ? FL_YELLOW : FL_DARK3;
		fl_color( color );
		fl_begin_line();
		fl_vertex( X1.x, X1.y );
		fl_vertex( X2.x, X2.y );
		fl_end_line();

//		if( _isSelected ) {
//			fl_rectf( X1.x, X1.y-10, 10, 10, FL_YELLOW );
//		}

		fl_line_style( 0 );
		fl_color( memo );
	}

	Intersection getIntersection( DragBox* box, Line& line ) {
		//which sideline does line intersect?
		//get all 4 vertices of box:
		int x1 = box->x();
		int x2 = x1 + box->w();
		int y1 = box->y();
		int y2 = y1 + box->h();

		//intersects with top line?
		Line topline( x1, y1, x2, y1 );
		Intersection X = topline.getIntersection( line );
		if( X.intersects && X.withinSegments ) {
			return X;
		} else {
			//right sideline?
			Line rightline( x2, y1, x2, y2 );
			X = rightline.getIntersection( line );
			if( X.intersects && X.withinSegments ) {
				return X;
			} else {
				//bottom line?
				if( line.x1() == 65 && line.y1() == 65 && line.x2() == 65 ) {
						int bla = 1;
					}
				Line bottomline( x1, y2, x2, y2 );
				X = bottomline.getIntersection( line );
				if( X.intersects && X.withinSegments ) {
					return X;
				} else {
					//left sideline?
					Line bottomline( x1, y1, x1, y2 );
					X = bottomline.getIntersection( line );
					return X;
				}
			}
		} //!X.intersects
	}  //Intersection getIntersection( ...

	bool isNearby( int x, int y, int radius ) {
		fprintf( stderr, "checking point %d/%d\n", x, y );
		int c1x, c1y, c2x, c2y;
		_box1->getCenter( c1x, c1y );
		_box2->getCenter( c2x, c2y );
		Line line( c1x, c1y, c2x, c2y );

		fprintf( stderr, "line created from %d/%d to %d/%d. m=%f, b=%f\n",
				c1x, c1y, c2x, c2y, line.m(), line.b() );
		return line.isPointInSegment( x, y );
	}

	void setSelected( bool selected = true ) {
		_isSelected = selected;
	}
private:
	DragBox* _box1;
	DragBox* _box2;
	bool _isSelected = false;
}; // class MyConnection

class MyCanvas : public Canvas {
public:
	MyCanvas( int x, int y, int w, int h ) : Canvas( x, y, w, h ) {
	}

	int handle( int e ) {
		int rc = Canvas::handle( e );
		switch( e ) {
		case FL_PUSH:
			resetSelections();
			checkSelectConnectionAt( Fl::event_x(), Fl::event_y() );
			break;
		default:
			break;
		}
		return rc;
	}

	void checkSelectConnectionAt( int x, int y ) {
		for( auto c : _connections ) {
			if( c->isNearby( x, y ) ) {
				c->setSelected( true );
			}
		}
	}

	void resetSelections() {
		for( auto c : _connections ) {
			c->setSelected( false );
		}
	}

	void createConnection( DragBox* box1, DragBox* box2 ) {
		MyConnection* conn = new MyConnection( box1, box2 );
		_connections.push_back( conn );
	}

	vector<MyConnection*> _connections;

protected:
	virtual void draw_then() {
		for( auto conn : _connections ) {
			conn->draw();
		}
	}
};

//*++++++++++++++++++++++++++++++++++++++++++++++++

DragBox* dragbox1;
DragBox* dragbox2;

void connect( DragBox* box1, DragBox* box2 ) {
	Point center1 = { (2*box1->x() + box1->w()) / 2,
					  (2*box1->y() + box1->h()) / 2 };
	Point center2 = { (2*box2->x() + box2->w()) / 2,
			          (2*box2->y() + box2->h()) / 2 };
	fprintf( stderr, "centers: %d/%d, %d/%d\n",
			center1.x, center1.y, center2.x, center2.y );
	box1->getCenter( center1.x, center1.y );
	box2->getCenter( center2.x, center2.y );
	fprintf( stderr, "centers: %d/%d, %d/%d\n",
				center1.x, center1.y, center2.x, center2.y );
}

void onClick( Fl_Widget*, void* ) {
	connect( dragbox1, dragbox2 );
}

int drawing_lines() {
	Fl_Double_Window* win = new Fl_Double_Window( 100, 100, 500, 500 );

	MyCanvas* canvas = new MyCanvas( 0, 0, 500, 470 );
	dragbox1 = new DragBox( 30, 30, 70, 70 );
	dragbox2 = new DragBox( 170, 170, 70, 70 );
	DragBox* dragbox3 = new DragBox( 290, 100, 80, 80 );
	canvas->end();
	canvas->createConnection( dragbox1, dragbox2 );
	canvas->createConnection( dragbox2, dragbox3 );

//	Fl_Button* btn = new Fl_Button( 12, 470, 70, 25, "Connect" );
//	btn->callback( onClick );

	win->end();
	win->show();
	return Fl::run();
}
