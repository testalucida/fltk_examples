/*
 * context_menu.cpp
 *
 *  Created on: 06.04.2020
 *      Author: martin
 */

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/fl_message.H>
#include <stdlib.h>             /* exit() */
#include <string.h>             /* strcmp() */

Fl_Window          *_win  = 0;
Fl_Menu_Button     *G_menu = 0;
Fl_Multiline_Input *G_input = 0;

// POPUP MENU CALLBACK
static void Menu_CB(Fl_Widget*,void*) {
  // DETERMINE WHICH ITEM USER PICKED
  const char *text = G_menu->text();     // get label of item user picked
  if ( !text ) return;
  if ( strcmp(text, "Do thing#1") == 0 ) { fl_message("You chose to do a thing"); }
  if ( strcmp(text, "Do thing#2") == 0 ) { fl_message("You chose to do a different thing"); }
  if ( strcmp(text, "Quit")       == 0 ) { exit(0); }
}

int context_menu() {
  _win = new Fl_Window(640,480,"Simple popup menu");
  _win->tooltip("Use right-click for popup menu..");
  {
    // SETUP RIGHT-CLICK CONTEXT MENU
    //    Entire background area within 0,0,640,480 will be
    //    sensitive to right-clicks for popup.
    //add(const char* label, int shortcut, Fl_Callback*, void *user_data=0, int flags=0)
    G_menu = new Fl_Menu_Button(0,0,640,480,"Popup Menu");
    G_menu->type(Fl_Menu_Button::POPUP3);         // pops menu on right click
    G_menu->add("Do thing#1", "^1", Menu_CB, 0);  // ctrl-1 hotkey
    G_menu->add("Do thing#2", "^2", Menu_CB, 0, FL_MENU_INACTIVE);  // ctrl-2 hotkey
    G_menu->add("Quit",       "^q", Menu_CB, 0);  // ctrl-q hotkey
  }
  // ADD INPUT WIDGET
  //    Helps demonstrate other widgets can coexist /within/ invisible
  //    menu button's xywh area.
  //
  G_input = new Fl_Multiline_Input(100,200,350,50,"Input");
  G_input->value("Right-click anywhere on gray window area\nfor popup menu");
  _win->end();
  _win->show();
  return(Fl::run());
}


