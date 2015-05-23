#include "graphics.hpp"
#include "label.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include "application.h"

using namespace genv;
using namespace std;

/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
void label::labelInit()
{
    marginSide = 10; marginTop = 7;
    visible = true;
    background = false;
    fontColor = Black;
    backgroundColor = "#ffffff";
    align = align_left;
    tabStop = false;
    display = trimBegin;

    VScrollBar = new scrollBar(x + width, y + 1, height - 2, vertical, 0, 0);
    VScrollBar->x -= VScrollBar->width; // csak a konstruktora lefutása után tudom megmondani a scrollBar szélességét
    VScrollBar->visible = false;

    lineHeight = gout.cascent() + gout.cdescent();
}

label::label(int x0, int y0, int width0, int height0) : widget(x0, y0, width0, height0)
{
    labelInit();
    eventChanged = [] (string newValue, widget* me) {};
    setText("Label");
}

label::label(int x0, int y0, int width0, int height0, string txt) : widget(x0, y0, width0, height0)
{
    labelInit();
    eventChanged = [] (string newValue, widget* me) {};
    setText(txt);
}

label::label(int x0, int y0, int width0, int height0, string txt, std::function<void(std::string, widget*)> f0) : widget(x0, y0, width0, height0)
{
    labelInit();
    eventChanged = [] (string newValue, widget* me) {};
    setText(txt);
    eventChanged = f0;
}

label::~label()
{
    delete VScrollBar;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------------------- Get/Set ----------------------------------------------------------
void label::setText(string txt)
{
    Text = txt;
    textRefresh();textRefresh();
    eventChanged(Text, this);
    eventGroupChanged(this);
}

string label::getText() const { return Text; }

void label::setMargin(int mS, int mT)
{
    marginSide = mS;
    marginTop = mT;
}
/// ----------------------------------------------------------------------------------------------------------------


/// ------------------------------------------ Események ----------------------------------------------------------
void label::onClick(int posX, int posY, char button)
{
    if ( VScrollBar->visible && VScrollBar->onMe(posX, posY) )
        VScrollBar->onClick(posX, posY, button);
}

void label::onMouseMove(int posX, int posY)
{
    if ( VScrollBar->visible && VScrollBar->onMe(posX, posY) )
        VScrollBar->onMouseMove(posX, posY);
}

void label::onMouseUp(int posX, int posY, char button)
{
    if ( VScrollBar->visible )
            VScrollBar->onMouseUp(posX, posY, button);
}

void label::onTick()
{
    static char time = 0;
    if ( 10 < time ) { textRefresh(); time = 0; }
    else time++;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
// Szöveg tördelése
void label::textRefresh()
{
    dispText = Text;

    if ( marginSide * 2 + gout.twidth(Text) < width && display != autoSize ) return;

    numberOfRows = 1;

    switch ( display )
    {
    case autoSize :
        {string row;
        dispText = "";
        unsigned int i = 0;
        unsigned int prevPos = 0;
        longestRow = 0;
        // Ebben a ciklusban nem karakterenként, hanem szavanként megyek végig a szövegen
        while ( i < Text.length() )
        {
            prevPos = i;
            i = Text.find(' ',prevPos + 1);
            if ( X - x - VScrollBar->width * VScrollBar->visible - 10 - marginSide*2
                < gout.twidth(row + Text.substr(prevPos, i-prevPos)) ) // Ha már nem fér ki a következõ szó
            {
                dispText += row + "\n";
                numberOfRows++;

                // Megszámolom az enterek számát, mivel ez is növeli a sorok számát
                unsigned int prevEnterPos = 0;
                unsigned int j = row.find("\n");
                while ( j < row.length() )
                {
                    j = row.find("\n" ,prevEnterPos + 2);
                    numberOfRows++;
                    prevEnterPos = j;
                }

                if ( longestRow < gout.twidth(row) ) longestRow = gout.twidth(row);

                // A következõ szó már csak a következõ sorba fér ki, és nem kell az elejére a space
                row = Text.substr(prevPos + 1, i - prevPos);
            }
            else
                row += Text.substr(prevPos, i - prevPos);
        }
        dispText += row;
        numberOfRows++;
        height = marginTop*2 + numberOfRows * lineHeight;
        width = longestRow + 2*marginSide;
        break;}

    case trimBegin :
        for ( unsigned int i = Text.length()-1; i > 0 ; i-- )
        {
            dispText = "..." + Text.substr(i, Text.length());
            if (width < marginSide * 2 + gout.twidth(dispText) + 4) break;
        }
        longestRow = gout.twidth(dispText);
        break;

    case trimEnd :
        for ( unsigned int i = Text.length()-1; i > 0 ; i-- )
        {
            dispText = Text.substr(0,i) + "...";
            if (width > marginSide * 2 + gout.twidth(dispText)) break;
        }
        longestRow = gout.twidth(dispText);
        break;

    case wordWrap :
        {string row;
        dispText = "";
        unsigned int i = 0;
        unsigned int prevPos = 0;
        // Ebben a ciklusban nem karakterenként, hanem szavanként megyek végig a szövegen
        while ( i < Text.length() )
        {
            prevPos = i;
            i = Text.find(' ',prevPos + 1);
            // Ha már nem fér ki a következõ szó
            if ( width - (VScrollBar->visible ? VScrollBar->width : 0) - marginSide*2 < gout.twidth(row + Text.substr(prevPos, i-prevPos)) )
            {
                dispText += row + "\n";
                numberOfRows++;

                // Megszámolom az enterek számát, mivel ez is növeli a sorok számát
                unsigned int prevEnterPos = 0;
                unsigned int j = row.find("\n");
                while ( j < row.length() )
                {
                    j = row.find("\n" ,prevEnterPos + 2);
                    numberOfRows++;
                    prevEnterPos = j;
                }

                if ( longestRow < gout.twidth(row) ) longestRow = gout.twidth(row);

                // A következõ szó már csak a következõ sorba fér ki, és nem kell az elejére a space
                row = Text.substr(prevPos + 1, i - prevPos);
            }
            else
                row += Text.substr(prevPos, i - prevPos);
        }
        dispText += row;
        numberOfRows++;
        break;}
    }

    // scrollBar beállítása
    if ( (display == autoSize || display == wordWrap) && height < marginTop*2 + numberOfRows * lineHeight )
    {
        VScrollBar->setRange(0,--numberOfRows);
        VScrollBar->setValueWidth(ceil((height - marginTop*2)/lineHeight));
        VScrollBar->visible = true;
        this->tabStop = true;
        if (this->focus) VScrollBar->focus = true;
        else VScrollBar->focus = false;
    }
    else
    {
        VScrollBar->visible = false;
        this->tabStop = false;
        VScrollBar->focus = false;
    }
}

// A szövegdoboz kirajzolása >> örökösök is ki tudják rajzoltatni
void label::labelDraw() const
{

    if ( background )
    {
        colorize(backgroundColor);
        gout << move_to(x,y) << box(width, height);
    }

    colorize(fontColor);
    if ( display == wordWrap )
        gout << move_to(x + marginSide, y + marginTop + gout.cascent());
    else if ( align == align_right )
        gout << move_to(x + width - gout.twidth(dispText)  - marginSide, y + marginTop + gout.cascent());
    else if ( align == align_center )
        gout << move_to(x + (width - gout.twidth(dispText))/2, y + marginTop + gout.cascent());
    else
        gout << move_to(x + marginSide, y + marginTop + gout.cascent());

    if ( VScrollBar->visible )
    {
        // Kiválasztom azokat a sorokat, amiket megjelenítek
        vector<string> rows;
        string textToDraw = "";
        stringstream ss;
        ss << dispText;
        string temp;
        // -->> Elõször szétválasztom a szöveget sorokra
        while ( getline(ss, temp) ) rows.push_back(temp + "\n");

        for ( int i = VScrollBar->getValue(); i < VScrollBar->getValue() + VScrollBar->getValueWidth() + 1; i++ )
            textToDraw += rows[i];

        gout << text(textToDraw);
    }
    else
        gout << text(dispText);

    VScrollBar->draw();
}

void label::draw() const
{

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if ( x < 0 ) { cerr << "Object is out of window! (x < 0) - label::draw" << endl; alreadyAlarmed = true; }
        else if ( y < 0 ) { cerr << "Object is out of window! (y < 0) - label::draw" << endl; alreadyAlarmed = true; }
        else if ( X < x + width ) { cerr << "Object is out of window! (X < x + width) - label::draw" << endl; alreadyAlarmed = true; }
        else if ( Y < y + height ) { cerr << "Object is out of window! (Y < y + height) - label::draw" << endl; alreadyAlarmed = true; }
    }

    if ( visible ) labelDraw();
}
/// ----------------------------------------------------------------------------------------------------------------

