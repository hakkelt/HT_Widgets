#include "radioButton.h"
#include "graphics.hpp"
#include <iostream>
#include "application.h"

using namespace genv;
using namespace std;

/// ------------------------------------------ Események ----------------------------------------------------------
void radioButton::onClick(int posX, int posY, char button)
{
    if ( (x+boxX) < posX && posX < (x + boxX + boxW) && (y + boxY) < posY && posY < (y + boxY + boxH) )
        { setValue(true); }
}

void radioButton::onKeyPress(char keyCode)
{
    if ( keyCode == key_space ) { setValue(true); }
}
/// ----------------------------------------------------------------------------------------------------------------


/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
radioButton::radioButton(int x0, int y0, unsigned int width0, unsigned int height0, node * _node) : widget(x0, y0, width0, height0)
{
    checked = false;
    myLabel = new label(x0, y0, width0, height0);
    myLabel->setMargin(boxX + boxW + 5, 0);
    myLabel->setText("RadioButton");
    tabStop = true;
    eventChanged = [] (widget* me) {};
    myNode = _node;
    myNode->add(this);
}
radioButton::radioButton(int x0, int y0, unsigned int width0, unsigned int height0, node * _node, std::function<void(widget*)> f0)
                                                                                        : widget(x0, y0, width0, height0)
{
    checked = false;
    myLabel = new label(x0, y0, width0, height0);
    myLabel->setMargin(boxX + boxW + 5, 0);
    myLabel->setText("RadioButton");
    tabStop = true;
    eventChanged = f0;
    myNode = _node;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------------------- Get/Set ----------------------------------------------------------
void radioButton::setText(string txt)
{
    myLabel->setText(txt);
}

string radioButton::getText() const
{
    return myLabel->getText();
}

void radioButton::setValue(bool newValue)
{
    checked = newValue;

    if ( !newValue ) return;

    for (unsigned int i = 0; i < myNode->members.size(); i++)
        if ( myNode->members[i] != this && dynamic_cast <radioButton*> (myNode->members[i]) )
            ((radioButton*)myNode->members[i])->setValue(false);

    eventChanged(this);
    eventGroupChanged(this);
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
void radioButton::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if (x < 0) { cerr << "Object is out of window! (x < 0) - radioButton::draw" << endl; alreadyAlarmed = true; }
        else if (y < 0) { cerr << "Object is out of window! (y < 0) - radioButton::draw" << endl; alreadyAlarmed = true; }
        else if (X < x + width) { cerr << "Object is out of window! (X < x + width) - radioButton::draw" << endl; alreadyAlarmed = true; }
        else if (Y < y + height) { cerr << "Object is out of window! (Y < y + height) - radioButton::draw" << endl; alreadyAlarmed = true; }
    }

    myLabel->draw();

    if ( focus ) circle(x + boxX, y + boxY, boxW, ActiveColor);
    else circle(x + boxX, y + boxY, boxW, White);

    circle3D(x+boxX, y+boxY, boxW);

    if ( checked ) circle(x + boxX + 2, y + boxY + 2, boxW - 4, Black);
}
/// ----------------------------------------------------------------------------------------------------------------

