/*
 * symbolstuff.cpp
 *
 *  Created on: 25.03.2020
 *      Author: martin
 */

#include "symbolstuff.h"

#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/fl_draw.H>

#include <string>
#include <iostream>
using namespace std;

#include "flx_base.h"

#include "dialogs.h"

//#include "./symbols/decision.xpm"
//#include "./symbols/process.xpm"
//#include "./symbols/inputoutput.xpm"
//#include "./symbols/manual_input.xpm"
//#include "./symbols/document.xpm"
//#include "./symbols/manual_op.xpm"
//#include "./symbols/datastorage.xpm"
//#include "./symbols/connector.xpm"
//#include "./symbols/start.xpm"
//#include "./symbols/end.xpm"


//const char** symbolpixmap[] = {
//		decision_xpm,
//		process_xpm,
//		inputoutput_xpm,
//		manual_input_xpm,
//		document_xpm,
//		manual_op_xpm,
//		datastorage_xpm,
//		connector_xpm,
//		start_xpm,
//		end_xpm
//};

const char* symbol_svg[] = {
		"./src/symbols/start.svg",
		"./src/symbols/line.svg",
		"./src/symbols/decision.svg",
		"./src/symbols/process.svg",
		"./src/symbols/inputoutput.svg",
		"./src/symbols/manualinput.svg",
		"./src/symbols/document.svg",
		"./src/symbols/manualoperation.svg",
		"./src/symbols/datastorage.svg",
		"./src/symbols/connector.svg",
		"./src/symbols/end.svg",
		"./src/symbols/none.svg"
};

const char* symboltooltip[] = {
		"Start",
		"Draw a connection line between 2 symbols",
		"Decision",
		"Process",
		"Input / Output",
		"Manual Input",
		"Document",
		"Manual Operation",
		"Data Storage",
		"Connector",
		"End",
		"Unselect symbol, activate Select Tool"
};



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//SymbolButton::SymbolButton(int x, int y, SymbolId symbolId, const char** pXpm) :
//		Fl_Toggle_Button( x, y, SymbolButton::getButtonWidth(), SymbolButton::getButtonWidth())
//{
//	_symbolId = symbolId;
//	clear_visible_focus();
//	box(FL_FLAT_BOX);
//	color(FL_LIGHT2);
//	down_box(FL_DOWN_BOX);
//	down_color(FL_YELLOW);
//	Fl_Pixmap* p = new Fl_Pixmap(pXpm);
//	image(p);
//}

SymbolButton::SymbolButton(int x, int y, SymbolId symbolId, const char* svg_file) :
		Fl_Toggle_Button( x, y, SymbolButton::getButtonWidth(), SymbolButton::getButtonWidth())
{
	_symbolId = symbolId;
	clear_visible_focus();
	box(FL_FLAT_BOX);
	color(FL_LIGHT2);
	down_box(FL_DOWN_BOX);
	down_color(FL_YELLOW);
	Fl_SVG_Image *svg = new Fl_SVG_Image(svg_file);
	svg->scale(w()-2, h()-2);
	image(svg);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

SymbolBar::SymbolBar(int x, int y, symbolClickCallback* cb, void* data) :
		Fl_Group(x, y, ALL_SYMBOLS_COUNT*SymbolButton::getButtonWidth() +
				       ALL_SYMBOLS_COUNT*SPACING + SPACING, 34)
{
	_cb = cb;
	_data = data;
	//type(Fl_Pack::HORIZONTAL);
	box(FL_BORDER_FRAME);
	color(FL_WHITE);
	//spacing(SPACING);
	end();
	int X = x + SymbolBar::SPACING, Y = y + SymbolBar::SPACING;
	for (int i = 0, imax = ALL_SYMBOLS_COUNT; i < imax; i++) {
		SymbolId id = (SymbolId)i;
		_symbolIds.push_back(id);
		addButton(X, Y, id, cb, data);
		X += (SymbolButton::getButtonWidth() + SymbolBar::SPACING);
	}
	_selectedSymbol = _symbolIds.at(0);
}

SymbolBar::SymbolBar(int x, int y, SymbolIds ids, symbolClickCallback* cb, void* data) :
		Fl_Group(x, y, ids.size()*SymbolButton::getButtonWidth() +
				       ids.size()*SPACING + SPACING, 34)
{
	_cb = cb;
	_data = data;
	//type(Fl_Pack::HORIZONTAL);
	box(FL_BORDER_FRAME);
	color(FL_WHITE);
	//spacing(SPACING);
	end();
	int X = x + SymbolBar::SPACING, Y = y + SymbolBar::SPACING;
	for (int i = 0, imax = ids.size(); i < imax; i++) {
		SymbolId id = ids.at(i);
		_symbolIds.push_back(id);
		addButton(X, Y, id, cb, data);
		X += (SymbolButton::getButtonWidth() + SymbolBar::SPACING);
	}
	_selectedSymbol = ids.at(0);
}

void SymbolBar::resize(int x, int y, int w, int h) {
	Fl_Group::resize(x, y, w, h);
}

void SymbolBar::staticOnSymbolClick(Fl_Widget* p, void *data) {
	SymbolBar* pThis = (SymbolBar*)data;
	pThis->onSymbolClick((SymbolButton*)p);
}

void SymbolBar::onSymbolClick(SymbolButton *b) {
	_selectedSymbol = b->getSymbolId();

	for (int i = 0, imax = children(); i < imax; i++) {
		try {
			SymbolButton* btn = (SymbolButton*)child(i);
			if (btn != b) btn->value(0);
		} catch(...) {}
	}

	fprintf(stderr, "click: %d\n", b->getSymbolId());
	if (_cb ) {
		(*_cb)(_selectedSymbol, _data);
	}
}

SymbolButton* SymbolBar::getSymbolButton(SymbolId symbolId) const {
	for (int i = 0, imax = children(); i < imax; i++) {
		try {
			SymbolButton* btn = (SymbolButton*)child(i);
			SymbolId child_symbolId = btn->getSymbolId();
			if (child_symbolId == symbolId) {
				return btn;
			}
		} catch(...) {}
	}
	throw runtime_error("SymbolBar::getSymbolButton " +
						to_string(symbolId) + " not found.");
}

void SymbolBar::setSymbolSelected(SymbolId symbolId) {
	//unselect previous Symbol:
	SymbolButton* btn = getSymbolButton(_selectedSymbol);
	btn->value(0);
	//select desired Symbol:
	btn = getSymbolButton(symbolId);
	btn->value(1);
	_selectedSymbol = symbolId;
}

SymbolId SymbolBar::getSelectedSymbol() const {
	return _selectedSymbol;
}

void SymbolBar::enableSymbol(SymbolId id, bool enable) {
	SymbolButton* btn = getSymbolButton(id);
	if( enable ) {
		btn->activate();
	} else {
		btn->deactivate();
	}
}

void SymbolBar::hideSymbol(SymbolId id) {
	SymbolButton* btn = getSymbolButton(id);
	btn->hide();

	for( int i = id+1, imax = _symbolIds.size(); i < imax; i++) {
		SymbolButton* next = getSymbolButton(_symbolIds.at(i));
		int x = next->x();
		x -= (btn->w());// + SymbolBar::SPACING);
		next->position(x, next->y());
	}

	int W = w();
	W -= (btn->w() + SymbolBar::SPACING);
	//size(W, h());
}

void SymbolBar::addButton( int x, int y, SymbolId id,
		                   symbolClickCallback *cb, void *data )
{
	begin();
	//SymbolButton *b = new SymbolButton( 0, 0, (SymbolId)i, symbolpixmap[i]);
	SymbolButton *b = new SymbolButton( x, y, id, symbol_svg[id]);
	const char* t = symboltooltip[id];
	b->tooltip(t);
	b->callback(staticOnSymbolClick, this);
	end();
}

//*********************************************************

void Symbol::setLabel(const char* txt) {
	_label.clear();
	_label.append(txt);
}

void Symbol::setBorderColor( Fl_Color color ) {
	_bordercolor = color;
	//redraw();
}

void Symbol::setBorderWidth(int w) {
	_borderwidth = w;
}

void Symbol::setLabelFont(Fl_Font font) {
	_labelfont = font;
}

void Symbol::setLabelFontsize(Fl_Fontsize h) {
	_labelfontsize = h;
}

int Symbol::handle(int e) {
	//cerr << "Symbol::handle(e) -> Event is " << fl_eventnames[e] << endl;
	static int offset[2] = { 0, 0 };
	switch ( e ) {
	case FL_PUSH:
		cerr << "Symbol::push" << endl;
		//prepare dragging:
		offset[0] = x() - Fl::event_x();    // save where user clicked for dragging
		offset[1] = y() - Fl::event_y();
		return(1);
	case FL_RELEASE:
		return(1);
	case FL_DRAG:
		position(offset[0]+Fl::event_x(), offset[1]+Fl::event_y());
		parent()->redraw();
		return(1);
	default:
		return Fl_Widget::handle(e);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Start::draw() {
	Symbol::draw();
	//this->draw_box(); //only for testing purposes
//	fl_color(_bordercolor);
//	fl_line_style( FL_SOLID, _borderwidth, NULL );
//
//	int x = this->x();
//	int y = this->y();
//	int w = this->w();
//	int h = this->h();
//
//	int X = x+1;
//	int Y = y+1;
//	int W = 30, H = 30;
//	//X, Y, W, H define the surrounding square,
//	//so the width of the pie will be W/2, the height will be H
//	fl_pie(X, Y, W, H, 90, 270);
//
//	fl_color(FL_YELLOW);
//	X += W/2;
//	int l = w - X - W/2;
//	fl_line(X, Y, X+l, Y);
//	Y += (H-1);
//	fl_line(X, Y, X+l, Y);
//
//	fl_color(_bordercolor);
//	Y = y+1;
//	//fl_pie(X+15, Y, W, H, -90, 90);
//
//	fl_line_style(0);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Decision::draw() {
	//fl_rect(this->x(), this->y(), this->w(), this->h(), FL_RED);
	fl_color(_bordercolor);
	fl_line_style( FL_SOLID, _borderwidth, NULL );

	int x = this->x();
	int y = this->y();
	int w = this->w();
	int h = this->h();

	fl_loop( x + w/2, y,
			 x + w, y + h/2,
			 x + w/2, y + h,
			 x, y + h/2 );

	fl_line_style(0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#include <FL/Fl_SVG_Image.H>
Symbol* SymbolFactory::create( SymbolId type, int x, int y, int w, int h,
		                       bool xy_is_center ) {
	//xy_is_center = true: create Symbol not at x, y but in such a way
	//that x, y is the middle  of the created Symbol.
	Symbol* pSymbol;
	switch(type) {
	case SymbolId::START:
		pSymbol = new Start(0, 0, 0, 0);
		pSymbol->box(FL_NO_BOX);
		//pSymbol->color(FL_WHITE);
		break;
	case SymbolId::DECISION:
		pSymbol = new Decision(0, 0, 0, 0);
		break;
	default:
		string msg = "SymbolFactory::create(): " + std::to_string(type);
		msg += ": unknown SymbolType.";
		throw runtime_error(msg);
	}

	setSizeAndPosition( pSymbol, x, y, w, h, xy_is_center );
	_symbols.push_back(pSymbol);

	return pSymbol;
}

void SymbolFactory::setSizeAndPosition( Symbol* pSymbol, int x, int y, int w, int h,
		                                bool xy_is_center)
{
	if( w == 0 || h == 0 ) {
		Size size = SymbolDefaults::getDefaultSize(pSymbol->getSymbolId());
		w = w == 0 ? size.w : w;
		h = h == 0 ? size.h : h;
	}

	if( xy_is_center ) {
		x -= w/2;
		y -= h/2;
	}

	pSymbol->position(x, y);
	pSymbol->size(w, h);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//SymbolSettings::SymbolSettings() {
//	//todo: read properties from a file
//}
//
//SymbolSettings& SymbolSettings::inst() {
//	static SymbolSettings* pThis = NULL;
//	if( !pThis ) {
//		pThis = new SymbolSettings();
//	}
//	return *pThis;
//}

//*********************************************************
void onShowSymbolDialog(Fl_Widget*) {
	DecisionDialog* dlg = new DecisionDialog( 100, 100);
	//SymbolDialog* dlg = new SymbolDialog( 100, 100 );
	dlg->setSymbolText("this is the ingenious data storage.\nSomewhere in the cloud.");
	//dlg->setSymbolSelected(SymbolId::DATA_STORAGE);
	int rc = dlg->show(false);
	fprintf(stderr, "closed dialog with rc = %d\n", rc);
	if (rc > 0) {
//		fprintf(stderr, "selected symbol: %d\n", dlg->getSelectedSymbol());
//		fprintf(stderr, "connect next symbol with YES: %s\n",
//				dlg->connectNextSymbolWithYesBranche() ? "y" : "n");
	}
	delete dlg;

}

int testSymbolStuff() {
	Fl_Double_Window* pWin = new Fl_Double_Window(100, 100, 800, 700, "Flowchart Tests");
	Fl_Button* btn = new Fl_Button(20, 20, 190, 25, "Show SymbolBaseDialog");
	btn->callback(onShowSymbolDialog);
	pWin->end();
	pWin->show();

	return Fl::run();
}


