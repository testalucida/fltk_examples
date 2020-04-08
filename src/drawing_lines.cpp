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

	void draw() {
		int x1, y1, x2, y2;
		_box1->getCenter( x1, y1 );
		_box2->getCenter( x2, y2 );
		Line line( x1, y1, x2, y2 ); //the line to draw
		Intersection X1 = getIntersection( _box1, line );
		if( !X1.intersects ) {
			throw runtime_error( "Connection::draw(): "
					             "line doesn't intersect box1");
		}

		Intersection X2 = getIntersection( _box2, line );
		if( !X2.intersects ) {
			throw runtime_error( "Connection::draw(): "
								 "line doesn't intersect box2");
		}

		fl_line_style( FL_SOLID, 3 );
		Fl_Color memo = fl_color();
		fl_color( FL_DARK3 );
		fl_begin_line();
		fl_vertex( X1.x, X1.y );
		fl_vertex( X2.x, X2.y );
		fl_end_line();
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
		if( X.intersects ) {
			return X;
		} else {
			//right sideline?
			Line rightline( x2, y1, x2, y2 );
			X = rightline.getIntersection( line );
			if( X.intersects ) {
				return X;
			} else {
				//bottom line?
				Line bottomline( x1, y2, x2, y2 );
				X = bottomline.getIntersection( line );
				if( X.intersects ) {
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
private:
	DragBox* _box1;
	DragBox* _box2;
};

class MyCanvas : public Canvas {
public:
	MyCanvas( int x, int y, int w, int h ) : Canvas( x, y, w, h ) {
	}

//	void draw() {
//		Canvas::draw();
//
//	}

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
	canvas->end();
	canvas->createConnection( dragbox1, dragbox2 );

//	Fl_Button* btn = new Fl_Button( 12, 470, 70, 25, "Connect" );
//	btn->callback( onClick );

	win->end();
	win->show();
	return Fl::run();
}
