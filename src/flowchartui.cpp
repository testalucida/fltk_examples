/*
 * flowchartui.cpp
 *
 *  Created on: 27.03.2020
 *      Author: martin
 */

#include "flowchartui.h"
#include <FL/Fl_Toggle_Button.H>

#include "std.h"

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartMainWindow::FlowChartMainWindow(int x, int y, int w, int h) :
		Fl_Double_Window(x, y, w, h, "FlowChart Creator")
{
	int margin = 10;
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	_pMenuBar = new FlowChartMenuBar(0, 0, w, 25);
	_pToolBar = new FlowChartToolBar(0, _pMenuBar->h(), w, 35);
	_pToolBar->registerSymbolCallback(
			FlowChartMainWindow::staticOnSymbolSelected, this);
	int status_h = 30;
	_pStatusBar = new StatusBar(0, h - status_h, w, status_h);

	int canvas_h = h - _pMenuBar->h() - _pToolBar->h() - _pStatusBar->h();
	_pCanvas = new Canvas(0+margin, _pToolBar->y() + _pToolBar->h(),
			              w-2*margin, canvas_h);

	resizable(_pCanvas);

	end();
}

void FlowChartMainWindow::
registerSymbolSelectedCallback(symbolClickCallback* cb, void* data) {
	_cb = cb;
	_data = data;
}

void FlowChartMainWindow::selectSymbol(SymbolId id) {
	SymbolBar* pBar = _pToolBar->getSymbolBar();
	pBar->setSymbolSelected(id);
}

void FlowChartMainWindow::staticOnSymbolSelected(SymbolId id, void* data) {
	FlowChartMainWindow* win = (FlowChartMainWindow*)data;
	win->onSymbolSelected(id);
}

void FlowChartMainWindow::onSymbolSelected(SymbolId id) {
	//fprintf(stderr, "FlowChartMainWindow::onSymbolSelected\n");
	if (_cb) {
		(*_cb)(id, _data);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartMenuBar::FlowChartMenuBar(int x, int y, int w, int h) :
		Fl_Menu_Bar(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_LIGHT1);
	labelcolor(FL_WHITE);
	add("File/Quit",   FL_CTRL+'q', NULL /*callback*/);
	add("Edit/Change", FL_CTRL+'c', NULL);
	add("Edit/Submenu/Aaa");
	add("Edit/Submenu/Bbb");
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FlowChartToolBar::FlowChartToolBar(int x, int y, int w, int h) :
		Fl_Group(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	_pSymbolBar = new SymbolBar(x+10, y,
			FlowChartToolBar::staticSymbolCallback, (void*)this);
	int filler_x = _pSymbolBar->x() + _pSymbolBar->w();
	int filler_w = w - filler_x;
	Fl_Box* pFiller = new Fl_Box(filler_x, y, filler_w, h);
	end();
	resizable(pFiller);
}

void FlowChartToolBar::
registerSymbolCallback(symbolClickCallback* cb, void* data) {
	_cb = cb;
	_data = data;
}

void FlowChartToolBar::staticSymbolCallback(SymbolId id, void* data) {
	FlowChartToolBar* pTb = (FlowChartToolBar*)data;
	pTb->symbolCallback(id);
}

void FlowChartToolBar::symbolCallback(SymbolId id) const {
	fprintf(stderr, "FlowChartToolBar::symbolCallback: selected: %d\n", id);
	//forward to registered callback:
	if (_cb) {
		(*_cb)(id, _data);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Canvas::Canvas(int x, int y, int w, int h) : Fl_Scroll(x, y, w, h) {
	box(FL_FLAT_BOX);
	color(FL_WHITE);
	end();
}

void Canvas::registerEventCallback(CanvasCallback ccb, void* pUserdata) {
	_ccb = ccb;
	_pCbUserdata = pUserdata;
}

void Canvas::addSymbol(Symbol* pSymbol) {
	add(pSymbol);
	redraw();
}

int Canvas::handle(int e) {
	//cerr << "Canvas::handle(e) -> Event is " << fl_eventnames[e] << endl;
	int rc = Fl_Scroll::handle(e);
	if( rc != 1 ) {
		switch ( e ) {
		case FL_PUSH: {
			cerr << "Canvas::push" << endl;
			if (_ccb) {
				(_ccb)(FL_PUSH, _pCbUserdata);
			}
	//			this->add(new Decision(x() + Fl::event_x(), y() + Fl::event_y()));
	//			redraw();
			return 1;
		}
		default:
			return Fl_Scroll::handle(e);
			//break;
		} //switch
	}
	return rc;
}

//+++++++++++++++++++++++++++++++++++++++

StatusBar::StatusBar(int x, int y, int w, int h) :
		Fl_Group(x, y, w, h)
{
	box(FL_FLAT_BOX);
	color(FL_DARK1);
	end();
}

void StatusBar::resize(int x, int y, int w, int h) {
//	int H = this->h();
//	fprintf(stderr, "StatusBar::resize: x/y/w/h/H: %d/%d/%d/%d/%d\n ",
//			x, y, w, h, H);

	Fl_Group::resize(x, y, w, h);
}
//+++++++++++++++++++++++++++++++++++++++

