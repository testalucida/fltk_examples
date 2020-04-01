/*
 * flow.cpp
 *
 *  Created on: 23.03.2020
 *      Author: martin
 */
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/names.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Rect.H>

#include "symbolstuff.h"
#include "dialogs.h"
#include "flowchartui.h"
#include "controllerstuff.h"

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace std;

void canvasCallback(int e, void* data) {
	fprintf(stderr, "canvasCallback: event = %s\n", fl_eventnames[e]);
}



struct Position {
	int x = 0;
	int y = 0;
};

class MyBox;
typedef void (*SelectionCallback)(MyBox*, bool shift_pressed, void*);
static int cnt = 0;

MyBox* box1;
MyBox* box2;

//struct DragDirection {
//	bool N = false;
//	bool E = false;
//	bool S = false;
//	bool W = false;
//	void reset() {
//		N = E = S = W = false;
//	}
//};

struct DragDelta {
	int new_x = 0;
	int new_y = 0;
	int delta_x = 0;
	int delta_y = 0;
};

class DragHelper {
public:
	DragHelper() {}
	/**
	 * Call this method after having received a PUSH event
	 * and before receiving the first DRAG event.
	 * x and y have to be the upper left corner of the Widget to be dragged.
	 */
	void prepareDragging( int x, int y ) {
		_x = x;
		_y = y;
		_offset_x = x - Fl::event_x();    // save where user clicked for dragging
		_offset_y = y - Fl::event_y();
	}

	/**
	 * Call this for dragging purposes. (Opposite to resizing dragging means
	 * moving the widget as a whole according to the dragging amount
	 * in x and y direction)
	 * Gets the amount of dragging into x and y direction.
	 * Gets the new calculated top left corner of the widget to draw.
	 */
	inline DragDelta& getDragDelta() {
		_dragDelta.new_x = _offset_x + Fl::event_x();
		_dragDelta.new_y = _offset_y + Fl::event_y();
		_dragDelta.delta_x = _dragDelta.new_x - _x;
		_dragDelta.delta_y = _dragDelta.new_y - _y;
		_x = _dragDelta.new_x;
		_y = _dragDelta.new_y;
		return _dragDelta;
	}

private:
	int _x = 0;
	int _y = 0;
	int _offset_x = 0;
	int _offset_y = 0;
	DragDelta _dragDelta;
};

class ResizeHelper {
public:
	ResizeHelper() {}

	void prepareResizing( Fl_Widget* w ) {
		_widget = w;
		_dragHelper.prepareDragging( w->x(), w->y() );
	}

	inline void resizeWidget() {
		DragDelta& dd = _dragHelper.getDragDelta();
		int x, w;
		getXW( dd, x, w );
		int y, h;
		getYH( dd, y, h );
		h = 10; //test

		_widget->resize( x, y, w, h );
	}

	inline void getXW( const DragDelta& dd, int& x, int& w ) {
		x = _widget->x();
		w = _widget->w();

		bool drag_right_side = false;
		bool enlarging_w = false;
		if( dd.delta_x >= 0 ) { //dragging to the right (but not necessarily the right side)
			if( dd.new_x > (x + w) ) {
				drag_right_side = true;
				enlarging_w = true;
			}
		} else { //dragging to the left
			if( dd.new_x < x ) {
				enlarging_w = true;
			} else {
				drag_right_side = true;
			}
		}

		int delta_x = abs( dd.delta_x );
		if( drag_right_side ) {
			//dragging right side...
			if( enlarging_w ) {
				//...to the right -- width becomes wider
				w += dd.delta_x;
			} else {
				//...to the left -- width becomes smaller
				w -= delta_x;
			}
		} else {
			//dragging left side...
			if( enlarging_w ) {
				//...to the left - x becomes lesser, right side stable so w must increase
				x -= delta_x;
				w += delta_x;
			} else {
				//...to the right - x becomes greater, right side stable so w must decrease
				x += delta_x;
				w -= delta_x;
			}
		}
	}

	inline void getYH( const DragDelta& dd, int& y, int& h ) {
		y = _widget->y();
		h = _widget->h();
	}

private:
	Fl_Widget* _widget;
	Position _dragstart;
	DragHelper _dragHelper;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class MyBox : public Fl_Box {
public:
	MyBox(int x, int y ) : Fl_Box(x, y, 90, 53) {
		cnt++;
		_cnt = cnt;
		box(FL_FLAT_BOX);
//		color(FL_WHITE); //color of canvas
	}

	void registerSelectionCallback(SelectionCallback cb) {
		_selectionCallback = cb;
	}

	void toggleSelection( bool selected ) {
		//fprintf(stderr, "toggled box %d, selected = %d\n", _cnt, _selected );
		_selected = selected;
		_selectionChanged = true;
		redraw();
		_selectionChanged = false;
	}

	virtual void draw() {
		//fprintf(stderr, "drawing box %d, selected = %d\n", _cnt, _selected );
		//this->draw_box(); //for testint purpose only
		int xx = this->x();
		int yy = this->y();
		int ww = this->w();

		if( !_selectionChanged ) {
			fl_color(FL_DARK2);
			fl_line_style( FL_SOLID, 3, NULL );

			int xx = this->x();
			int yy = this->y();
			int ww = this->w();
			int x = xx + 10;
			int y = yy + 10;
			int w = ww - 20;

			int X = x+1;
			int Y = y+1;
			int W = 30, H = 30;
			//X, Y, W, H define the surrounding square,
			//so the width of the pie will be W/2, the height will be H
			fl_pie(X, Y, W, H, 90, 270);
			//remaining width after having drawn left pie:
			int w_rem = w - W/2;

			//draw upper and lower line:
			X += W/2;
			int len = w_rem - W/2;
			fl_line(X, Y, X+len, Y);
			Y += (H-1);
			fl_line(X, Y, X+len, Y);

			fl_draw ("Start", X+2, Y - 10 );

			X += (len - W/2 - 2);
			//remaining width after having draw lines:
			w_rem -= len;

			Y = y+1;
			fl_pie(X, Y, W, H, -90, 90);

			fl_line_style(0);
		}

		int hh = this->h();
		drawSelectionSquare( xx, yy, ww, hh );
	}

	void move( int delta_x, int delta_y ) {
		//fprintf( stderr, "moving %d, %d\n", delta_x, delta_y );
		position( x() + delta_x, y() + delta_y );
	}

protected:
	int handle(int e) {
		//static int offset[2] = { 0, 0 };
		switch(e) {
		case FL_PUSH: {
			fprintf( stderr, "MyBox(%d)::handle(FL_PUSH)\n", _cnt );
			//prepare dragging:
			_dragHelper.prepareDragging( x(), y() );

			//do selection callback to unselect other selected symbols
			bool draw = false;
			if( !_selected ) {
				_selected = true;
				draw = true;
				bool c = Fl::event_key(FL_Shift_L);
				//fprintf(stderr, "Shift pressed: %d \n", c );
				if( _selectionCallback ) {
					(_selectionCallback)(this, c, NULL);
				}
			}

			if( draw ) {
				redraw();
			}

			return 1;
		}
		case FL_DRAG: {
			//fprintf( stderr, "MyBox::handle(FL_DRAG\n)" );
			DragDelta& dd = _dragHelper.getDragDelta();
			move( dd.delta_x, dd.delta_y );

			//TODO: create DraggingCallback
			if( _cnt == 1 ) { //testtesttest
				box2->move( dd.delta_x, dd.delta_y );
			}
			parent()->redraw();
			return(1);
		}
		default:
			return Fl_Box::handle(e);
		}
	}
private:
	void drawSelectionSquare( int x, int y, int w, int h ) {
		int len = 10;
		uchar r, g, b;
		if( _selected ) {
			r = 249, g = 255, b = 0; //yellow
		} else {
			r = 255, g = 255, b = 255; //color of canvas
		}
		//small square top left:
		fl_rectf ( x, y, len, len, r, g, b );
		//small square top right:
		fl_rectf( x+w-len, y, len, len, r, g, b );
		//small square bottom right:
		fl_rectf( x+w-len, y+h-len, len, len, r, g, b );
		//small square bottom left:
		fl_rectf( x, y+h-len, len, len, r, g, b );

	}
private:
	bool _selected = false;
	bool _selectionChanged = false;
	SelectionCallback _selectionCallback = NULL;
	DragHelper _dragHelper;
	int _cnt = 0;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++

class SelectionBox : public Fl_Box {
public:
	SelectionBox( int x, int y, int w, int h ) : Fl_Box( x, y, w, h ) {
	}

	void draw() {
		if( _w > 0 ) {
			fl_rect( _x, _y, _w, _h, FL_WHITE );
		}
		int X = x() + 1;
		int Y = y() + 1;
		int W = w() - 2;
		int H = h() - 2;
		fl_rect( X, Y, W, H, FL_BLACK );
		_x = X;
		_y = Y;
		_w = W;
		_h = H;
	}

private:
	int _x = 0, _y = 0, _w = 0, _h = 0;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++

class MyCanvas : public Fl_Group {
public:
	MyCanvas( int x, int y, int w, int h ) : Fl_Group( x, y, w, h ) {
		box(FL_FLAT_BOX);
		color( FL_WHITE );
	}

	void unselectAllSymbols() {
		for( int i = 0, imax = children(); i < imax; i++ ) {
			MyBox* box = (MyBox*)child( i );
			box->toggleSelection( false );
		}
	}
protected:
	int handle(int evt) {
		//static int offset[2] = { 0, 0 };
		switch( evt ) {
		case FL_PUSH: {
//			offset[0] = x() - Fl::event_x();    // save where user clicked for dragging
//			offset[1] = y() - Fl::event_y();
//			fprintf( stderr, "MyCanvas::handle(FL_PUSH)\n" );
			Fl_Widget* w = Fl::belowmouse ();
			if( w == this ) {
				//clicking on canvas results in unselecting all selected symbols:
				unselectAllSymbols();

				//prepare dragging:
//				fprintf( stderr, "MyCanvas: PUSH: x() = %d, y() = %d, event_x() = %d, event_y() = %d\n",
//						 x(), y(), Fl::event_x(), Fl::event_y() );
				_start.x = Fl::event_x() - x();
				_start.y = Fl::event_y() - y();

				if( !_selectionBox ) {
					_selectionBox = new SelectionBox( _start.x, _start.y, 1, 1 );
					add( _selectionBox );
				}

				_resizeHelper.prepareResizing( _selectionBox );
				//fprintf( stderr, "Drawing startpoint: %d, %d\n", _start.x, _start.y );
				return 1;
			} else {
				return Fl_Group::handle(evt);
			}
		}
		case FL_RELEASE: {
			if( _selectionBox ) {
				//dragging finished, select children enclosed by _selectionBox
				int sel_x = _selectionBox->x();
				int sel_y = _selectionBox->y();
				int sel_xmax = sel_x + _selectionBox->w();
				int sel_ymax = sel_y + _selectionBox->h();
				for( int i = 0, imax = children(); i < imax; i++ ) {
					Fl_Widget* ch = child( i );
					int ch_x = ch->x();
					int ch_y = ch->y();
					int ch_xmax = ch_x + ch->w();
					int ch_ymax = ch_y + ch->h();
					if( ch_x >= sel_x && ch_y >= sel_y ) {
						if( ch_xmax <= sel_xmax && ch_ymax <= sel_ymax ) {
							((MyBox*)ch)->toggleSelection( true );
						}
					}
				}
				remove( _selectionBox );
				_selectionBox = NULL;
				redraw();
			}

			return 1;
		}
		case FL_DRAG: {
//			if( !_selectionBox ) {
//				_selectionBox = new SelectionBox( _start.x, _start.y, 1, 1 );
//				add( _selectionBox );
//			}

			_resizeHelper.resizeWidget();

//			fprintf( stderr,
//					"origin: %d,%d "
//					"delta_x/y: %d,%d "
//					"new_x/y: %d,%d\n"
//					, _start.x, _start.y,
//					  dd.delta_x, dd.delta_y,
//					  dd.new_x, dd.new_y );

			//fprintf( stderr, "MyCanvas::handle(FL_DRAG)\n" );
			_end.x = Fl::event_x() - x();
			_end.y = Fl::event_y() - y();
			int w = _end.x - _start.x;
			w = w < 0 ? w*(-1) : w;
			int h = _end.y - _start.y;
			h = h < 0 ? h*(-1) : h;
			//_selectionBox->resize( _start.x, _start.y, w, h );
			//fprintf( stderr, "DRAGged to: %d,%d\n", _end.x, _end.y );
			redraw();

			return 1;
		}
		default:
			break;
		} //swithc

		return Fl_Group::handle( evt );
	}

private:
	Position _start;
	Position _end;
	SelectionBox* _selectionBox = NULL;
	DragHelper _dragHelper;
	ResizeHelper _resizeHelper;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void onSelection(MyBox* box, bool shift_pressed, void* data=NULL) {
	if( !shift_pressed ) {
		if( box == box1 ) {
			box2->toggleSelection( false );
		} else {
			box1->toggleSelection( false );
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++
int flow() {
	//FlowChartMainWindow* pWin = new FlowChartMainWindow(100, 100, 800, 700);
	Fl_Double_Window* pWin = new Fl_Double_Window(200, 200, 500, 500, "Test Drawing");

	pWin->box(FL_FLAT_BOX);
	pWin->color(FL_WHITE);
	MyCanvas* canvas = new MyCanvas( 0, 0, 500, 500 );
	box1 = new MyBox(100, 100 );
	box1->registerSelectionCallback( onSelection );
	box2 = new MyBox(100, 200);
	box2->registerSelectionCallback( onSelection );
	canvas->end();

	//pWin->getCanvas()->registerEventCallback(canvasCallback, NULL);
//	CanvasController* pCtrl = new CanvasController(pWin->getCanvas());
//	pWin->registerSymbolSelectedCallback(
//			CanvasController::symbolSelectedCallback_static, pCtrl);
//	pWin->selectSymbol(SymbolId::NONE);
	pWin->end();
	pWin->show();

	return Fl::run();
}



