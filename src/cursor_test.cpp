/*
 * cursor_test.cpp
 *
 *  Created on: 02.04.2020
 *      Author: martin
 */
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>

class Box : public Fl_Box {
public:
	Box( int x, int y, int w, int h ) : Fl_Box( x, y, w, h ) {
		box( FL_FLAT_BOX );
		color( FL_YELLOW );
	}
protected:
	int handle( int evt ) {
		switch( evt ) {
		case FL_PUSH:
			fprintf( stderr, "PUSH\n" );
			//this->top_window()->cursor( FL_CURSOR_WAIT ); //doesn't work
			fl_cursor( FL_CURSOR_SE ); //doesn't work either
			return 1;
		case FL_RELEASE:
			fl_cursor( FL_CURSOR_DEFAULT );
			return 1;
		default:
			return Fl_Box::handle( evt );
		}
	}
};

int cursor_test() {
	Fl_Double_Window* pWin = new Fl_Double_Window(200, 200, 100, 100, "Test Cursor");
	pWin->box(FL_FLAT_BOX);
	pWin->color(FL_WHITE);
	new Box( 20, 20, 60, 60 );
	pWin->end();
	pWin->show();

	return Fl::run();
}
