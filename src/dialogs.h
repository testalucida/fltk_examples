/*
 * dialogs.h
 *
 *  Created on: 26.03.2020
 *      Author: martin
 */

#ifndef DIALOGS_H_
#define DIALOGS_H_

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Multiline_Input.H>

enum SymbolId {
	START,
	LINE,
	DECISION,
	PROCESS,
	INPUT_OUTPUT,
	MANUAL_INPUT,
	DOCUMENT,
	MANUAL_OP,
	DATA_STORAGE,
	CONNECTOR,
	END,
	NONE,
	//count = 12
};


class Fl_Return_Button;
class Fl_Button;
class Fl_Multiline_Input;

class SymbolBar;

class SymbolBaseDialog: public Fl_Double_Window {
public:
	SymbolBaseDialog(int x, int y);
	virtual ~SymbolBaseDialog();
	//void getClientAreaSizeAndPosition(int& x, int& y, int& w, int&h) const;
	void setClientAreaWidget(Fl_Widget*);
	Fl_Group* getClientArea() const;
	Fl_Font getPreferredTextfont() const {return FL_HELVETICA;}
	Fl_Fontsize getPreferredTextsize() const {return 13;}
	Fl_Font getPreferredLabelfont() const {return FL_HELVETICA_BOLD;}
	Fl_Fontsize getPreferredLabelsize() const {return 14;}
	int show(bool modally);
	static void staticOkCancel_cb( Fl_Widget*, void*);
	void okCancel_cb(Fl_Button* pBtn);
	const char* getSymbolText() const {return _pSymbolText->value();}
	void setSymbolText(const char* pText) {_pSymbolText->value(pText);}
	SymbolId getSelectedSymbol() const;
	void setSymbolSelected(SymbolId symbolId);
private:
	Fl_Return_Button *_pBtnOk=(Fl_Return_Button *)0;
	Fl_Button *_pBtnCancel=(Fl_Button *)0;
	Fl_Multiline_Input* _pSymbolText = NULL;
	Fl_Group* _pClientArea;
	SymbolBar* _pSymbolBar;
	bool _ok = false;
};

//++++++++++++++++++++++++++++++++++++++++

class SymbolDialog : public SymbolBaseDialog {
public:
	SymbolDialog( int x, int y );
	virtual ~SymbolDialog() {}
	const char* getConnectionLabel() const {return _pConnectionLabel->value();}
	void setConnectionLabel( const char* lbl ) {_pConnectionLabel->value(lbl);}
private:
	//label of connection line between 2 symbols
	Fl_Multiline_Input* _pConnectionLabel;
};

//++++++++++++++++++++++++++++++++++++++++
class Fl_Round_Button;

class DecisionDialog : public SymbolBaseDialog {
public:
	DecisionDialog(int X, int Y);
	~DecisionDialog();

	bool connectNextSymbolWithYesBranche();

private:
	static void onOptionYesNo(Fl_Widget* p, void* data);
private:
	//Fl_Group* _pGrp;
	//SymbolBar* _pSymbolBar;
	Fl_Round_Button* _pOptionYes;
	Fl_Round_Button* _pOptionNo;
};

#endif /* DIALOGS_H_ */
