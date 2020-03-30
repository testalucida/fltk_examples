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

//++++++++++++++++++++++++++++++++++++++++
int flow() {
	FlowChartMainWindow* pWin = new FlowChartMainWindow(100, 100, 800, 700);
	//pWin->getCanvas()->registerEventCallback(canvasCallback, NULL);
	CanvasController* pCtrl = new CanvasController(pWin->getCanvas());
	pWin->registerSymbolSelectedCallback(
			CanvasController::symbolSelectedCallback_static, pCtrl);
	pWin->selectSymbol(SymbolId::NONE);
	pWin->end();
	pWin->show();

	return Fl::run();
}



