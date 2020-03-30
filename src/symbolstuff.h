/*
 * symbolstuff.h
 *
 *  Created on: 25.03.2020
 *      Author: martin
 */

#ifndef SYMBOLSTUFF_H_
#define SYMBOLSTUFF_H_

#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>

#include "flx_base.h"

#include "dialogs.h"

#include "std.h"


static int ALL_SYMBOLS_COUNT = 12;

static Size defaultSymbolSizes[] = {
		{70, 40}, //Start
		{70, 70}, //Line
		{70, 70}, //Decision
		{70, 70}, //Process
		{70, 70}, //Input / Output
		{70, 70},
		{70, 70}, //Document
		{70, 70},
		{70, 70},
		{70, 70},
		{70, 70},
		{70, 70}  //Unselect symbol
};

typedef void (symbolClickCallback)(SymbolId, void*);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SymbolButton : public Fl_Toggle_Button {
public:
	SymbolButton(int x, int y, SymbolId symbolId, const char** pXpm);
	SymbolButton(int x, int y, SymbolId symbolId, const char* svg_file);
	static int getButtonWidth() {
		return 30;
	}

	SymbolId getSymbolId() const {
		return _symbolId;
	}

private:
	SymbolId _symbolId;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef std::vector<SymbolId> SymbolIds;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SymbolBar : public Fl_Group {
public:
	SymbolBar(int x, int y, symbolClickCallback* cb = NULL, void* data = NULL);
	SymbolBar(int x, int y, SymbolIds, symbolClickCallback* cb = NULL, void* data = NULL);

	/** Get the currently selected Symbol */
	SymbolId getSelectedSymbol() const;

	/** Marks the given Symbol as selected */
	void setSymbolSelected(SymbolId);

	SymbolButton* getSymbolButton(SymbolId) const;

	/** enable/disable the button representing the given SymbolId*/
	void enableSymbol(SymbolId, bool enable);

	/** Remove SymbolButton with the given SymbolId from the bar */
	void hideSymbol(SymbolId);

	static void staticOnSymbolClick(Fl_Widget* p, void *data);

	void onSymbolClick(SymbolButton *b);

	void resize(int x, int y, int w, int h);

private:
	void addButton(int x, int y, SymbolId, symbolClickCallback*, void *data = NULL);

private:
	SymbolIds _symbolIds;
	symbolClickCallback* _cb = NULL;
	void* _data = NULL;
	static const int SPACING = 2;
	SymbolId _selectedSymbol = SymbolId::NONE;
};

//++++++++++++++++++++++++++++++++++++++++

class SymbolDefaults {
public:
	static Size getDefaultSize(SymbolId id) {
		return defaultSymbolSizes[id];
	}
};

//+++++++++++++++++++++++++++++++++++++++++

#include <FL/Fl_SVG_Image.H>
class Symbol : public Fl_Box {
public:
	Symbol(int x, int y, int w, int h) :
		Fl_Box(x, y, w, h)
    {
		box(FL_FLAT_BOX);
		Fl_SVG_Image *svg = new Fl_SVG_Image("./src/symbols/start.svg");
		svg->scale( this->w()-2, this->h()-2, 1, 1 );
		image(svg);
	}
	virtual ~Symbol() {}
	virtual void draw() {Fl_Box::draw();}

	virtual void resize(int x, int y, int w, int h) {
		Fl_Box::resize(x, y, w, h);
		image()->scale(w, h, 1, 1);
	}

	virtual SymbolId getSymbolId() const = 0;

	//label
	void setLabel(const char* txt);
	const char* getLabel() const {return _label.c_str();}
	//border color
	void setBorderColor( Fl_Color color );
	Fl_Color getBorderColor() const {return _bordercolor;}
	//border width
	void setBorderWidth(int w);
	int getBorderWidth() const {return _borderwidth;}
	//labelfont
	void setLabelFont(Fl_Font);
	Fl_Font getLabelFont() const {return _labelfont;}
	//labelfontsize
	void setLabelFontsize(Fl_Fontsize);
	Fl_Fontsize getLabelFontsize() const {return _labelfontsize;}

protected:
	int handle(int e);
	Fl_Color _bordercolor = FL_DARK1;
	int _borderwidth = 3;
	Fl_Font _labelfont = FL_HELVETICA;
	Fl_Fontsize _labelfontsize = 13;
	std::string _label;
};

//++++++++++++++++++++++++++++++++++++++++

class Start : public Symbol {
public:
	Start(int x, int y, int w = 0, int h = 0 ) : Symbol(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::START;}

	~Start() {}

	virtual void draw();

};
//++++++++++++++++++++++++++++++++++++++++

class Decision : public Symbol {
public:
	Decision(int x, int y, int w = 0, int h = 0 ) : Symbol(x, y, w, h)
	{
	}

	virtual SymbolId getSymbolId() const {return SymbolId::DECISION;}

	~Decision() {}

	virtual void draw();

};

//++++++++++++++++++++++++++++++++++++++++

class SymbolFactory {
public:
	static SymbolFactory& inst() {
		static SymbolFactory inst;
		return inst;
	}

	~SymbolFactory() {
		//todo: delete all symbols in _pSymbols
//		for (auto s : _symbols) {
//			delete s;
//		}
	}

	Symbol* create(SymbolId type, int x, int y, int w = 0, int h = 0, bool xy_is_center = true);

protected:
	SymbolFactory(){} // Prevent construction
	SymbolFactory(const SymbolFactory&); // Prevent construction by copying
	SymbolFactory& operator=(const SymbolFactory&); // Prevent assignment
	//~SymbolFactory() {} // Prevent unwanted destruction

private:
	void setSizeAndPosition( Symbol*, int x, int y, int w, int h, bool xy_is_center);
private:
	std::vector<Symbol*> _symbols;
};


#endif /* SYMBOLSTUFF_H_ */
