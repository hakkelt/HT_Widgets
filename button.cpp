#include "graphics.hpp"
#include <iostream>
#include "button.h"
#include "application.h"

using namespace genv;
using namespace std;

/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
void button::buttonInit()
{
    background = true;
    backgroundColor = ButtonColor;
    pressed = false;
    tabStop = true;
    align = align_center;
}

button::button(int x0, int y0, unsigned int width0, unsigned int height0) : label(x0, y0, width0, height0)
{
    buttonInit();
}

button::button(int x0, int y0, unsigned int width0, unsigned int height0, std::string txt) : label(x0, y0, width0, height0, txt)
{
    buttonInit();
}

button::button(int x0, int y0, unsigned int width0, unsigned int height0, std::string txt, std::function<void(int, int, char, widget*)> f0) : label(x0, y0, width0, height0, txt)
{
    eventClick = f0;
    buttonInit();
}
/// ----------------------------------------------------------------------------------------------------------------


/// ------------------------------------------ Események ----------------------------------------------------------
void button::onClick(int posX, int posY, char button)
{
    pressed = true;
}

void button::onMouseUp(int posX, int posY, char button)
{
    pressed = false;
}

void button::onTick()
{
    if (focus) backgroundColor = ActiveColor;
    else backgroundColor = ButtonColor;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
void button::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if ( x < 0 ) { cerr << "Object is out of window! (x < 0) - button::draw" << endl; alreadyAlarmed = true; }
        else if ( y < 0 ) { cerr << "Object is out of window! (y < 0) - button::draw" << endl; alreadyAlarmed = true; }
        else if ( X < x + width ) { cerr << "Object is out of window! (X < x + width) - button::draw" << endl; alreadyAlarmed = true; }
        else if ( Y < y + height ) { cerr << "Object is out of window! (Y < y + height) - button::draw" << endl; alreadyAlarmed = true; }
    }

    labelDraw();

    rectange3D(x, y, width, height, pressed);
}
/// ----------------------------------------------------------------------------------------------------------------
