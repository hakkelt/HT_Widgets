#include "checkbox.h"
#include "graphics.hpp"
#include <iostream>
#include "application.h"

using namespace genv;
using namespace std;

/// ------------------------------------------ Események ----------------------------------------------------------
void checkBox::onClick(int posX, int posY, char button)
{
    if ( (x+boxX) < posX && posX < (x + boxX + boxW) && (y + boxY) < posY && posY < (y + boxY + boxH) )
    {
        checked = !checked;
        eventChanged(checked, this);
        eventGroupChanged(this);
    }
}

void checkBox::onKeyPress(char keyCode)
{
    if ( keyCode == key_space )
    {
        checked = ! checked;
        eventChanged(checked, this);
        eventGroupChanged(this);
    }
}
/// ----------------------------------------------------------------------------------------------------------------


/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
checkBox::checkBox(int x0, int y0, unsigned int width0, unsigned int height0) : widget(x0, y0, width0, height0)
{
    checked = false;
    myLabel = new label(x0, y0, width0, height0);
    myLabel->setMargin(boxX + boxW + 5, 0);
    myLabel->setText("CheckBox");
    tabStop = true;
    eventChanged = [] (bool newValue, widget* me) {};
}
checkBox::checkBox(int x0, int y0, unsigned int width0, unsigned int height0, std::function<void(bool, widget*)> f0) : widget(x0, y0, width0, height0)
{
    checked = false;
    myLabel = new label(x0, y0, width0, height0);
    myLabel->setMargin(boxX + boxW + 5, 0);
    myLabel->setText("CheckBox");
    tabStop = true;
    eventChanged = f0;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------------------- Get/Set ----------------------------------------------------------
void checkBox::setText(string txt)
{
    myLabel->setText(txt);
}

string checkBox::getText() const
{
    return myLabel->getText();
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
void checkBox::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if (x < 0) { cerr << "Object is out of window! (x < 0) - checkBox::draw" << endl; alreadyAlarmed = true; }
        else if (y < 0) { cerr << "Object is out of window! (y < 0) - checkBox::draw" << endl; alreadyAlarmed = true; }
        else if (X < x + width) { cerr << "Object is out of window! (X < x + width) - checkBox::draw" << endl; alreadyAlarmed = true; }
        else if (Y < y + height) { cerr << "Object is out of window! (Y < y + height) - checkBox::draw" << endl; alreadyAlarmed = true; }
    }

    myLabel->draw();

    if ( focus ) colorize(ActiveColor);
    else colorize(White);
    gout << move_to(x+boxX, y+boxY) << box(boxW,boxH);

    rectange3D(x+boxX, y+boxY, boxW,boxH, true);

    if ( checked )
    {
        colorize(Black);
        gout << move_to(x + boxX + 1, y + boxY + 1) << line(boxW - 2, boxH - 2);
        gout << move_to(x + boxX + 1, y + boxY + boxH - 2) << line(boxW - 2 ,-boxH + 2);
    }
}
/// ----------------------------------------------------------------------------------------------------------------

