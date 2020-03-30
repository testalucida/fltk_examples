/*
 * svg.cpp
 *
 *  Created on: 26.03.2020
 *      Author: martin
 */

#include <FL/Fl_Window.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>

class resizable_box: public Fl_Box {
public:
	resizable_box(int w, int h) :
			Fl_Box(0, 0, w, h, NULL) {
	}
	virtual void resize(int x, int y, int w, int h) {
		Fl_Box::resize(x, y, w, h);
		image()->scale(w, h, 1, 1);
	}
	virtual void draw() {
		Fl_Box::draw();
	}
};
int test_svg() {
	Fl_Window *win = new Fl_Window(130, 130);
	Fl_SVG_Image *svg = new Fl_SVG_Image("./src/symbols/decision.svg");
			//"/home/martin/Projects/cpp/fltk_examples/src/symbols/process.svg");

	resizable_box *box = new resizable_box(win->w(), win->h());
	box->box(FL_FLAT_BOX);
	box->color(FL_RED);
	box->image(svg);
	svg->scale(box->w(), box->h(), 1, 1);

//	Fl_Toggle_Button* pBtn = new Fl_Toggle_Button(0, 0, 50, 50);
//	pBtn->clear_visible_focus();
//	pBtn->image(svg);
//	//svg->proportional = true;
//	svg->scale(30, 30, 1, 1);

	win->end();
	win->resizable(win);
	win->show();
	return Fl::run();
}
