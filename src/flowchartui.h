/*
 * flowchartui.h
 *
 *  Created on: 27.03.2020
 *      Author: martin
 */

#ifndef FLOWCHARTUI_H_
#define FLOWCHARTUI_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>

#include "symbolstuff.h"

//+++++++++++++++++++++++++++++++++++++++

class FlowChartMenuBar : public Fl_Menu_Bar {
public:
	FlowChartMenuBar(int x, int y, int w, int h);
};

//+++++++++++++++++++++++++++++++++++++++

class FlowChartToolBar : public Fl_Group {
public:
	FlowChartToolBar(int x, int y, int w, int h);
	void registerSymbolCallback(symbolClickCallback*, void*);
	static void staticSymbolCallback(SymbolId, void*);
	void symbolCallback(SymbolId) const;
	SymbolBar* getSymbolBar() const {return _pSymbolBar;}
private:
	SymbolBar* _pSymbolBar;
	symbolClickCallback* _cb = NULL;
	void* _data = NULL;
};

//+++++++++++++++++++++++++++++++++++++++

class StatusBar : public Fl_Group {
public:
	StatusBar(int x, int y, int w, int h);
	virtual void resize(int x, int y, int w, int h);
};

//+++++++++++++++++++++++++++++++++++++++

typedef void (*CanvasCallback)(int event, void* userdata);

//++++++++++++++++++++++++++++++++++++++++
class Canvas : public Fl_Scroll {
public:
	Canvas(int x, int y, int w, int h);
	~Canvas() {}

	void registerEventCallback(CanvasCallback ccb, void* pUserdata);

	void addSymbol(Symbol* pSymbol);

protected:
	int handle(int e);

private:
	CanvasCallback _ccb = NULL;
	void* _pCbUserdata = NULL;
};

//+++++++++++++++++++++++++++++++++++++++

class FlowChartMainWindow : public Fl_Double_Window {
public:
	FlowChartMainWindow(int x, int y, int w, int h);
	void registerSymbolSelectedCallback(symbolClickCallback*, void*);
	Canvas* getCanvas() const {return _pCanvas;}
	void selectSymbol(SymbolId id);
private:
	static void staticOnSymbolSelected(SymbolId, void*);
	void onSymbolSelected(SymbolId);
private:
	FlowChartMenuBar* _pMenuBar;
	FlowChartToolBar* _pToolBar;
	StatusBar* _pStatusBar;
	Canvas* _pCanvas;
	symbolClickCallback* _cb = NULL;
	void* _data = NULL;
};

//+++++++++++++++++++++++++++++++++++++++

#endif /* FLOWCHARTUI_H_ */
