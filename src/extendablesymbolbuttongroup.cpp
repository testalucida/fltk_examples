/*
 * extendablesymbolbuttongroup.cpp
 *
 *  Created on: 25.03.2020
 *      Author: martin
 */

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Pack.H>
#include "symbolstuff.h"

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

//enum SymbolId {
//	DECISION,
//	PROCESS,
//	INPUT_OUTPUT,
//	MANUAL_INPUT,
//	DOCUMENT,
//	MANUAL_OP,
//	DATA_STORAGE,
//	CONNECTOR,
//	START,
//	END,
//	count = 10
//};

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

//const char* symboltooltip[] = {
//		"Decision",
//		"Process",
//		"Input / Output",
//		"Manual Input",
//		"Document",
//		"Manual Operation",
//		"Data Storage",
//		"Connector",
//		"Start",
//		"End"
//};

//class SymbolButton : public Fl_Button {
//public:
//	SymbolButton(int x, int y, SymbolId symbolId, const char** pXpm) :
//		Fl_Button( x, y, SymbolButton::getButtonWidth(), SymbolButton::getButtonWidth())
//	{
//		_symbolId = symbolId;
//		clear_visible_focus();
//		box(FL_FLAT_BOX);
//		color(FL_LIGHT2);
//		Fl_Pixmap* p = new Fl_Pixmap(pXpm);
//		image(p);
//	}
//
//	static int getButtonWidth() {
//		return 30;
//	}
//
//	SymbolId getSymbolId() {
//		return _symbolId;
//	}
//
//private:
//	SymbolId _symbolId;
//};

//typedef void (*symbolClickCallback)(SymbolId, void*);

//class SymbolBar : public Fl_Pack {
//public:
//	SymbolBar(int x, int y, symbolClickCallback *cb = NULL, void* data = NULL) :
//		Fl_Pack(x, y, SymbolId::count*SymbolButton::getButtonWidth() +
//				      SymbolId::count*SPACING, 34)
//	{
//		_cb = cb;
//		_data = data;
//		type(Fl_Pack::HORIZONTAL);
//		box(FL_BORDER_FRAME);
//		color(FL_WHITE);
//		spacing(SPACING);
//		end();
//
//		for (int i = 0, imax = SymbolId::count; i < imax; i++) {
//			addButton(i, cb, data);
//		}
//
//	}
//
//	static int getWidth() {
//		return SymbolId::count * SymbolButton::getButtonWidth();
//	}
//
//	static void staticOnSymbolClick(Fl_Widget* p, void *data) {
//		SymbolBar* pThis = (SymbolBar*)data;
//		pThis->onSymbolClick((SymbolButton*)p);
//	}
//
//	void onSymbolClick(SymbolButton *b) {
//		fprintf(stderr, "click: %d\n", b->getSymbolId());
//		if (_cb ) {
//			(*_cb)(b->getSymbolId(), _data);
//		}
//	}
//
//private:
//	void addButton(int i, symbolClickCallback *cb, void *data = NULL) {
//		begin();
//	    SymbolButton *b = new SymbolButton( 0, 0, (SymbolId)i, symbolpixmap[i]);
//	    const char* t = symboltooltip[i];
//	    b->tooltip(t);
//	    b->callback(staticOnSymbolClick, this);
//		end();
//	}
//private:
//	symbolClickCallback* _cb = NULL;
//	void* _data = NULL;
//	static const int SPACING = 2;
//};


void onSymbolClicked(SymbolId id, void* data) {
	fprintf(stderr, "onSymbolClicked");
}

int ext_symbols_grp() {
	Fl_Double_Window* pWin = new Fl_Double_Window(100, 100, 400, 200, "Extendable Symbol Group");
	pWin->box(FL_FLAT_BOX);
	pWin->color(FL_LIGHT3);
	//SymbolBar* sb = new SymbolBar(20, 20);
	pWin->end();
	pWin->show();
	pWin->redraw();
	return Fl::run();
}

