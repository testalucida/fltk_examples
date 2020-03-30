/*
 * controllerstuff.cpp
 *
 *  Created on: 27.03.2020
 *      Author: martin
 */

#include "controllerstuff.h"
#include <FL/names.h>

#include "flowchartui.h"
#include "std.h"

using namespace std;

CanvasController::CanvasController(Canvas* p) : _pCanvas(p) {
	_pCanvas->registerEventCallback(CanvasController::canvasEventCallback_static, this);
}

void CanvasController::canvasEventCallback_static(int evt, void* pThis) {
	((CanvasController*)pThis)->onCanvasEvent(evt);
}

void CanvasController::onSymbolSelected(SymbolId id) {
	//fprintf(stderr, "CanvasController::onSymbolSelected\n");
	_symbolId = id;
}

void CanvasController::onCanvasEvent(int evt) {
		//cerr << "CanvasController::onCanvasEvent: evt = " << fl_eventnames[evt] << endl;
		switch(evt) {
		case FL_PUSH:
			if (!_isSymbolDialogOpen && _symbolId != SymbolId::NONE) {
				int x = Fl::event_x();
				int y = Fl::event_y();
				Symbol* pSymbol = SymbolFactory::inst().create(_symbolId, x, y);
				_pCanvas->addSymbol(pSymbol);
				x = _pCanvas->parent()->x() + _pCanvas->x();
				y = _pCanvas->parent()->y() + _pCanvas->y();
				SymbolBaseDialog* pDlg;
				if (_symbolId == SymbolId::DECISION) {
					pDlg = new DecisionDialog(x, y);
				} else {
					//open default dialog
					pDlg = new SymbolDialog( x, y );
				}
				_isSymbolDialogOpen = true;
				int rc = pDlg->show(false);
				_isSymbolDialogOpen = false;

			}
			break;
		default:
			break;
		}
	}
