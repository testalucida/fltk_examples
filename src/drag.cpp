
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pixmap.H>
#include <stdio.h>
//
// Demonstrate user-movable boxes in a scroll region
// erco@netcom.com 08/06/02
// erco@3dsite.com 01/06/05 -- added call to Fl_Box::handle()
//
static const char *cat_xpm[] = {                       // XPM
"50 34 4 1",
"  c black",
"o c #ff9900",
"@ c #ffffff",
"# c None",
"##################################################",
"###      ##############################       ####",
"### ooooo  ###########################  ooooo ####",
"### oo  oo  #########################  oo  oo ####",
"### oo   oo  #######################  oo   oo ####",
"### oo    oo  #####################  oo    oo ####",
"### oo     oo  ###################  oo     oo ####",
"### oo      oo                     oo      oo ####",
"### oo       oo  ooooooooooooooo  oo       oo ####",
"### oo        ooooooooooooooooooooo        oo ####",
"### oo     ooooooooooooooooooooooooooo    ooo ####",
"#### oo   ooooooo ooooooooooooo ooooooo   oo #####",
"####  oo oooooooo ooooooooooooo oooooooo oo  #####",
"##### oo oooooooo ooooooooooooo oooooooo oo ######",
"#####  o ooooooooooooooooooooooooooooooo o  ######",
"###### ooooooooooooooooooooooooooooooooooo #######",
"##### ooooooooo     ooooooooo     ooooooooo ######",
"##### oooooooo  @@@  ooooooo  @@@  oooooooo ######",
"##### oooooooo @@@@@ ooooooo @@@@@ oooooooo ######",
"##### oooooooo @@@@@ ooooooo @@@@@ oooooooo ######",
"##### oooooooo  @@@  ooooooo  @@@  oooooooo ######",
"##### ooooooooo     ooooooooo     ooooooooo ######",
"###### oooooooooooooo       oooooooooooooo #######",
"###### oooooooo@@@@@@@     @@@@@@@oooooooo #######",
"###### ooooooo@@@@@@@@@   @@@@@@@@@ooooooo #######",
"####### ooooo@@@@@@@@@@@ @@@@@@@@@@@ooooo ########",
"######### oo@@@@@@@@@@@@ @@@@@@@@@@@@oo ##########",
"########## o@@@@@@ @@@@@ @@@@@ @@@@@@o ###########",
"########### @@@@@@@     @     @@@@@@@ ############",
"############  @@@@@@@@@@@@@@@@@@@@@  #############",
"##############  @@@@@@@@@@@@@@@@@  ###############",
"################    @@@@@@@@@    #################",
"####################         #####################",
"##################################################",
};

Fl_Double_Window *G_win    = NULL;
Fl_Scroll        *G_scroll = NULL;
static Fl_Pixmap  G_cat(cat_xpm);

#define BOXWIDTH  80
#define BOXHEIGHT 50

// A 'MOVABLE' BOX
class Box : public Fl_Box {
protected:
    int handle(int e) {
        static int offset[2] = { 0, 0 };
        int ret = Fl_Box::handle(e);
        switch ( e ) {
            case FL_PUSH:
                offset[0] = x() - Fl::event_x();    // save where user clicked for dragging
                offset[1] = y() - Fl::event_y();
                return(1);
            case FL_RELEASE:
                return(1);
            case FL_DRAG:
                position(offset[0]+Fl::event_x(), offset[1]+Fl::event_y());     // handle dragging
                G_win->redraw();
                return(1);
        }
        return(ret);
    }
public:
    Box(int X, int Y, int W, int H, const char *L=0) : Fl_Box(X,Y,W,H,L) {
        image(G_cat);
        box(FL_UP_BOX);
        color(FL_GRAY);
    }
    Box(int X, int Y) : Fl_Box(X,Y,BOXWIDTH,BOXHEIGHT,0) {
        image(G_cat);
        box(FL_UP_BOX);
        color(FL_GRAY);
    }
};

/// MAIN
int drag_example() {
    G_win = new Fl_Double_Window(420,300);
    G_scroll = new Fl_Scroll(10,10,420-20,300-20);
    G_scroll->box(FL_FLAT_BOX);
    G_scroll->color(FL_RED);
    G_scroll->begin();
    {
        // CREATE NEW BOXES ON THE SCROLLABLE 'DESK'
        for ( int x=20; x<=G_scroll->w()-BOXWIDTH; x+= BOXWIDTH+20)
            for ( int y=20; y<=G_scroll->h()-BOXHEIGHT; y+= BOXHEIGHT+20)
                new Box(x,y);
    }
    G_scroll->end();
    G_win->resizable(G_win);
    G_win->show();
    return(Fl::run());
}





