/*
 * flow.cpp
 *
 *  Created on: 23.03.2020
 *      Author: martin
 */
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/names.h>
#include <FL/Fl_Box.H>

#include "symbolstuff.h"
#include "dialogs.h"
#include "flowchartui.h"
#include "controllerstuff.h"

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

void canvasCallback(int e, void* data) {
	fprintf(stderr, "canvasCallback: event = %s\n", fl_eventnames[e]);
}


class MyBox : public Fl_Box {
public:
	MyBox(int x, int y ) : Fl_Box(x, y, 90, 53) {
		box(FL_FLAT_BOX);
		//color(FL_RED);
	}

	virtual void draw() {
		this->draw_box(); //only for testing purposes
		fl_color(FL_DARK2);
		fl_line_style( FL_SOLID, 3, NULL );

		int xx = this->x();
		int yy = this->y();
		int ww = this->w();
		int hh = this->h();

		int x = xx + 10;
		int y = yy + 10;
		int w = ww - 20;
		int h = hh - 20;

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

		//fl_color(FL_DARK2);
		Y = y+1;
		fl_pie(X, Y, W, H, -90, 90);

		fl_line_style(0);

		if( _selected ) {
			drawSelectionSquare( xx, yy, ww, hh );
		}
	}
protected:
	int handle(int e) {
		switch(e) {
		case FL_PUSH:
			//do selection callback to unselect other selected symbols
			_selected = true;
			redraw();
			return 1;
		default:
			return Fl_Box::handle(e);
		}
	}
private:
	void drawSelectionSquare( int x, int y, int w, int h ) {
		int len = 10;
		uchar r = 249, g = 255, b = 0;
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
};

//++++++++++++++++++++++++++++++++++++++++
int flow() {
	//FlowChartMainWindow* pWin = new FlowChartMainWindow(100, 100, 800, 700);
	Fl_Double_Window* pWin = new Fl_Double_Window(200, 200, 500, 500, "Test Drawing");
	pWin->box(FL_FLAT_BOX);
	pWin->color(FL_WHITE);
	new MyBox(100, 100 );
	new MyBox(100, 200);

	//pWin->getCanvas()->registerEventCallback(canvasCallback, NULL);
//	CanvasController* pCtrl = new CanvasController(pWin->getCanvas());
//	pWin->registerSymbolSelectedCallback(
//			CanvasController::symbolSelectedCallback_static, pCtrl);
//	pWin->selectSymbol(SymbolId::NONE);
	pWin->end();
	pWin->show();

	return Fl::run();
}



