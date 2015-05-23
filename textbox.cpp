#include "textbox.h"
#include "graphics.hpp"
#include <iostream>
#include "application.h"

using namespace genv;
using namespace std;

/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
void textBox::textInit()
{
    background = true;
    backgroundColor = White;
    tabStop = true;
    pressedKey = 0;
    pressedTime = 0;
    cursorOn = true;
    display = trimEnd;
    allowedChar = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789|€[]$<>#&@{}\,.-?:_;*§\"'+!%=/()~ˇ^˘°˛`˙´ ";
}

textBox::textBox(int x0, int y0, unsigned int width0, unsigned int height0) : label(x0, y0, width0, height0)
{
    textInit();
    setText("TextBox");
}

textBox::textBox(int x0, int y0, unsigned int width0, unsigned int height0, string txt) : label(x0, y0, width0, height0, txt)
{
    textInit();
    setText(txt);
}
/// ----------------------------------------------------------------------------------------------------------------


/// ------------------------------------------- Események ----------------------------------------------------------
void textBox::onClick(int posX, int posY, char button)
{
    if ( VScrollBar->visible && VScrollBar->onMe(posX, posY) )
        VScrollBar->onClick(posX, posY, button);
}

void textBox::onMouseMove(int posX, int posY)
{
    if ( VScrollBar->visible && VScrollBar->onMe(posX, posY) )
            VScrollBar->onMouseMove(posX, posY);
}

void textBox::onMouseUp(int posX, int posY, char button)
{
    if ( VScrollBar->visible )
            VScrollBar->onMouseUp(posX, posY, button);
}

void textBox::onKeyPress(char keyCode)
{
    if ( key_space <= keyCode && keyCode <= 255 && allowedChar.find(keyCode) < allowedChar.length() )
        setText(Text + keyCode);
    else if ( keyCode == key_backspace )
        setText(Text.substr(0,Text.length()-1));
    else if ( keyCode == key_enter && (display == wordWrap || display == autoSize) )
        setText(Text + "\n");
}

void textBox::onKeyUp(char keyCode)
{
    pressedKey = 0;
    pressedTime = 0;
}

void textBox::onTick()
{
    if ( pressedKey !=0 ) pressedTime++;
    if ( pressedTime > pressInterval / t )
    {
        if ( pressedKey == key_backspace ) setText(Text.substr(0,Text.length()-1));
        else setText(Text + pressedKey);
    }

    cursorTime++;
    if ( cursorTime > cursorInterval / t ) cursorOn = true;
        else cursorOn = false;
    if ( cursorTime > cursorInterval / t * 2 ) cursorTime = 0;

    static char time = 0;
    if ( 10 < time ) { textRefresh(); time = 0; }
    else time++;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------

void textBox::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if ( x < 0 ) { cerr << "Object is out of window! (x < 0) - textBox::draw" << endl; alreadyAlarmed = true; }
        else if ( y < 0 ) { cerr << "Object is out of window! (y < 0) - textBox::draw" << endl; alreadyAlarmed = true; }
        else if ( X < x + width ) { cerr << "Object is out of window! (X < x + width) - textBox::draw" << endl; alreadyAlarmed = true; }
        else if ( Y < y + height ) { cerr << "Object is out of window! (Y < y + height) - textBox::draw" << endl; alreadyAlarmed = true; }
    }

    labelDraw();

    rectange(x, y, width, height);

    if ( focus && cursorOn && (!VScrollBar->visible || VScrollBar->getValue() == VScrollBar->getMaxValue() - VScrollBar->getValueWidth()) )
    {
        colorize(Black);
        if ( align == align_right )
            gout << move_to(x + width - marginSide, y + marginTop + VScrollBar->getValueWidth()*lineHeight) << line(0, gout.cascent() + gout.cdescent());
        else if ( align == align_center )
            gout << move_to(x + (width + gout.twidth(dispText))/2, y + marginTop + VScrollBar->getValueWidth()*lineHeight) << line(0, gout.cascent() + gout.cdescent());
        else
            gout << move_to(x + marginSide + gout.twidth(dispText), y + marginTop + VScrollBar->getValueWidth()*lineHeight) << line(0, gout.cascent() + gout.cdescent());
    }
}

/// ----------------------------------------------------------------------------------------------------------------
