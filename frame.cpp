#include "graphics.hpp"
#include <iostream>
#include "application.h"
#include "color.h"
#include "frame.h"

using namespace genv;
using namespace std;

/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
frame::frame(int x0, int y0, int width0, int height0) : widget(x0, y0, width0, height0)
{
    filled = false;
    backgroundColor = White - 10;
    borderColor = Black;
}
/// ----------------------------------------------------------------------------------------------------------------



/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
void frame::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if ( x < 0 ) { cerr << "Object is out of window! (x < 0) - frame::draw" << endl; alreadyAlarmed = true; }
        else if ( y < 0 ) { cerr << "Object is out of window! (y < 0) - frame::draw" << endl; alreadyAlarmed = true; }
        else if ( X < x + width ) { cerr << "Object is out of window! (X < x + width) - frame::draw" << endl; alreadyAlarmed = true; }
        else if ( Y < y + height ) { cerr << "Object is out of window! (Y < y + height) - frame::draw" << endl; alreadyAlarmed = true; }
    }

    if ( filled )
    {
        colorize(backgroundColor);
        gout << move_to(x, y) << box(width, height);
    }
    switch ( style )
    {
        case flat: rectange(x, y, width, height); break;
        case deep: rectange3D(x, y, width, height, true); break;
        case high: rectange3D(x, y, width, height, false); break;
        default : rectange(x, y, width, height); break;
    }
}
/// ----------------------------------------------------------------------------------------------------------------
