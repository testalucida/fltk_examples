/*
 * SymbolBaseDialog.cpp
 *
 *  Created on: 26.03.2020
 *      Author: martin
 */

#include "dialogs.h"
#include "symbolstuff.h"

#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Box.H>

SymbolBaseDialog::SymbolBaseDialog(int X, int Y) :
	Fl_Double_Window(X, Y, 565, 210, "Symbol Dialog")
{
	color(FL_LIGHT3);
	int symboltext_h = 52;
	_pSymbolText = new Fl_Multiline_Input( 185, 10, 370, symboltext_h,
			                              "Enter symbol\'s label: " );
	_pSymbolText->box(FL_FLAT_BOX);
	_pSymbolText->textfont(getPreferredTextfont());
	_pSymbolText->textsize(getPreferredTextsize());
	_pSymbolText->labelfont(getPreferredLabelfont());
	_pSymbolText->labelsize(getPreferredLabelsize());

	int x = 10, vspacing = 5;
	int y = vspacing + 10 + symboltext_h;

	Fl_Group* grp = new Fl_Group( x, y, w() - 2*x, 45 );
	grp->box(FL_FLAT_BOX);
	grp->color(FL_LIGHT2);
	Fl_Font labelfont = getPreferredLabelfont();
	Fl_Fontsize labelsize = getPreferredLabelsize();

	const char* cs1 = "Next symbol to connect with: ";
	Size sz = TextMeasure::inst().get_size(cs1, labelfont, labelsize);
	int topmargin = 6;
	Fl_Box* pBox = new Fl_Box( x + 5, y + topmargin + 10, sz.w, sz.h, cs1 );
	pBox->labelfont(labelfont);
	pBox->labelsize(labelsize);

	int x2 = x + pBox->w() + 5;
	SymbolIds ids;
	ids.push_back(SymbolId::DECISION);
	ids.push_back(SymbolId::PROCESS);
	ids.push_back(SymbolId::INPUT_OUTPUT);
	ids.push_back(SymbolId::MANUAL_INPUT);
	ids.push_back(SymbolId::DOCUMENT);
	ids.push_back(SymbolId::MANUAL_OP);
	ids.push_back(SymbolId::DATA_STORAGE);
	ids.push_back(SymbolId::CONNECTOR);
	ids.push_back(SymbolId::END);
	_pSymbolBar = new SymbolBar(x2, y+topmargin, ids, NULL, NULL);

	grp->end();

	int clientarea_y = grp->y() + grp->h();
	int clientarea_h = 45;
	int clientarea_w = w() - 2*x;
	_pClientArea = new Fl_Group(x, clientarea_y, clientarea_w, clientarea_h);
	_pClientArea->box(FL_FLAT_BOX);
	_pClientArea->color(FL_LIGHT2 /*FL_LIGHT2*/);
	_pClientArea->end();

	int button_y = _pClientArea->y() + _pClientArea->h() + vspacing;
	_pBtnOk = new Fl_Return_Button(x, button_y, 72, 25, "OK");
	_pBtnOk->box(FL_THIN_UP_FRAME);
	_pBtnOk->down_box(FL_THIN_DOWN_FRAME);
	_pBtnOk->color((Fl_Color)53);
	_pBtnOk->callback(SymbolBaseDialog::staticOkCancel_cb, this);

	x += _pBtnOk->w() + 5;
	_pBtnCancel = new Fl_Button(x, button_y, 72, 25, "Cancel");
	_pBtnCancel->box(FL_THIN_UP_FRAME);
	_pBtnCancel->down_box(FL_THIN_DOWN_FRAME);
	_pBtnCancel->color((Fl_Color)53);
	_pBtnCancel->callback(SymbolBaseDialog::staticOkCancel_cb, this);

	int dlg_h = _pBtnOk->y() + _pBtnOk->h() + vspacing;
	size( w(), dlg_h );
}

SymbolBaseDialog::~SymbolBaseDialog() {
	fprintf(stderr, "SymbolBaseDialog destructed.\n");
}

Fl_Group* SymbolBaseDialog::getClientArea() const {
	return _pClientArea;
}

void SymbolBaseDialog::setClientAreaWidget(Fl_Widget* pW) {
	_pClientArea->add(pW);
}

int SymbolBaseDialog::show( bool modally ) {
	if( modally ) {
		Fl_Double_Window::set_modal();
		//Note: dialog will be placed int0 the middle of the
		//application disregarding given x and y position
	} else {
		Fl_Double_Window::set_non_modal();
	}

	_pSymbolText->take_focus();

	Fl_Double_Window::show();
	//fprintf(stderr, "position x/y: %d/%d\n", x(), y());

	while( shown() ) {
		Fl::wait();
	}
	return _ok ? 1 : 0;
}

void SymbolBaseDialog::staticOkCancel_cb(Fl_Widget* pWdgt, void* pUserData) {
	SymbolBaseDialog* pDlg = (SymbolBaseDialog*)pUserData;
	pDlg->okCancel_cb( (Fl_Button*)pWdgt );
}

void SymbolBaseDialog::okCancel_cb( Fl_Button* pBtn ) {
    if( pBtn == _pBtnOk  ) {
		_ok = true;
		hide();
    } else {
        _ok = false;
        hide();
    }
}

SymbolId SymbolBaseDialog::getSelectedSymbol() const {
	return _pSymbolBar->getSelectedSymbol();
}

void SymbolBaseDialog::setSymbolSelected(SymbolId symbolId) {
	_pSymbolBar->setSymbolSelected(symbolId);
}

//void SymbolBaseDialog::
//getClientAreaSizeAndPosition(int &x, int &y, int &w, int &h) const
//{
//	x = _pClientArea->x();
//	y = _pClientArea->y();
//	w = _pClientArea->w();
//	h = _pClientArea->h();
//}

//*********************************************************

SymbolDialog::SymbolDialog( int x, int y ) :
		SymbolBaseDialog( x, y )
{
	Fl_Font labelfont = getPreferredLabelfont();
	Fl_Fontsize labelsize = getPreferredLabelsize();
	const char* lbl = "Enter label for connection line: ";
	Size sz = TextMeasure::inst().get_size(lbl, labelfont, labelsize);

	Fl_Group* grp = getClientArea();
	grp->begin();

	int hspacing = 5;
	int vspacing = 5;
	int X = grp->x() + hspacing;
	int Y = grp->y() + grp->h()/2 - labelsize/2;

	Fl_Box* pBox = new Fl_Box( X, Y, sz.w, sz.h, lbl );
	pBox->labelfont(labelfont);
	pBox->labelsize(labelsize);

	X += pBox->w() + hspacing;
	int W = grp->w() - (pBox->x() + pBox->w() + hspacing);
	_pConnectionLabel = new Fl_Multiline_Input(
			X, grp->y() + vspacing, W, grp->h() - 2*vspacing );
	_pConnectionLabel->box(FL_FLAT_BOX);

	grp->end();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

DecisionDialog::DecisionDialog(int X, int Y) : SymbolBaseDialog(X, Y) {
	this->label("Decision Dialog");

	int leftmargin = 5, delta_x = 5;
	int topmargin = 8, delta_h = 5;

	Fl_Group* grp = getClientArea();
	grp->begin();
	int x = grp->x();
	int y = grp->y();
	int w = grp->w();
	int h = grp->h();

	Fl_Font labelfont = getPreferredLabelfont();
	Fl_Fontsize labelsize = getPreferredLabelsize();

	const char* cs2 = "Branch to connect with: ";
	Size sz = TextMeasure::inst().get_size(cs2, labelfont, labelsize);
	Fl_Box* pBox = new Fl_Box( x+leftmargin, y+topmargin+2, sz.w, sz.h, cs2 );
	pBox->labelfont(labelfont);
	pBox->labelsize(labelsize);
	int x2 = x + pBox->w() + delta_x;
	_pOptionYes = new Fl_Round_Button(x2, pBox->y(), 70, pBox->h(), "YES");
	_pOptionYes->tooltip("Connect next symbol with the YES branch of current decision symbol");
	_pOptionYes->down_box(FL_ROUND_DOWN_BOX);

	_pOptionNo = new Fl_Round_Button(
			_pOptionYes->x() + _pOptionYes->w() + delta_x, pBox->y(),
			70, pBox->h(), "NO");
	_pOptionNo->tooltip("Connect next symbol with the NO branch of current decision symbol");
	_pOptionNo->down_box(FL_ROUND_DOWN_BOX);

	_pOptionYes->callback(onOptionYesNo, _pOptionNo);
	_pOptionNo->callback(onOptionYesNo, _pOptionYes);
	_pOptionYes->value(1);

	grp->end();

}

DecisionDialog::~DecisionDialog() {
	fprintf(stderr, "DecisionDialog destructed.\n");
}

bool DecisionDialog::connectNextSymbolWithYesBranche() {
	return (_pOptionYes->value() == 1);
}

void DecisionDialog::onOptionYesNo(Fl_Widget* p, void* data) {
	Fl_Round_Button* pChanged = (Fl_Round_Button*)p;
	Fl_Round_Button* pOther = (Fl_Round_Button*)data;
	char v = pChanged->value();
	int other_v = v == 0 ? 1 : 0;
	pOther->value(other_v);
}

//*********************************************************

void onShowDialog(Fl_Widget*) {
	SymbolBaseDialog* dlg = new SymbolBaseDialog( 100, 100);
	int rc = dlg->show(false);
	fprintf(stderr, "closed dialog with rc = %d\n", rc);
}

int testSymbolBaseDialog() {
	Fl_Double_Window* pWin = new Fl_Double_Window(100, 100, 800, 700, "Flowchart Tests");
	Fl_Button* btn = new Fl_Button(20, 20, 190, 25, "Show SymbolBaseDialog");
	btn->callback(onShowDialog);
	pWin->end();
	pWin->show();

	return Fl::run();
}


