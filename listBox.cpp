#include "listBox.h"
#include "graphics.hpp"
#include <iostream>
#include <math.h>
#include "application.h"

using namespace genv;
using namespace std;

enum pressed_type { none, increase, decrease };

void emptyFunctionChange(int newValue) {}

/// ------------------------------------------ Események ----------------------------------------------------------
void listBox::onClick(int posX, int posY, char button)
{
    if ( myScrollBar->visible && myScrollBar->onMe(posX, posY) )
        myScrollBar->onClick(posX, posY, button);
    else
    {
        for ( unsigned int i = 0; i < items.size(); i++ )
        {
            if ( items[i]->visible && items[i]->onMe(posX, posY) )
                { setIndex(i); break; }
        }
    }
}

void listBox::onMouseUp(int posX, int posY, char button)
{
    myScrollBar->onMouseUp(posX, posY, button);
}

void listBox::onMouseMove(int posX, int posY)
{
    if ( myScrollBar->onMe(posX, posY) )
        myScrollBar->onMouseMove(posX, posY);
}

void listBox::onKeyPress(char keyCode)
{
    switch (keyCode)
    {
    case (char)key_down :
        if ( listIndex < (signed)items.size() - 1 )
            setIndex(listIndex + 1);
        if ( myScrollBar->getValue() + myScrollBar->getValueWidth() <= listIndex )
            myScrollBar->setValue(myScrollBar->getValue() + 1);
        keyDown = increase;
        waitCounter = 0;
        break;

    case (char)key_up :
        if ( 0 < listIndex )
            setIndex(listIndex - 1);
        if ( listIndex < myScrollBar->getValue() )
            myScrollBar->setValue(myScrollBar->getValue() - 1);
        keyDown = decrease;
        waitCounter = 0;
        break;

    case (char)key_pgdn :
        setIndex(items.size() - 1);
        if (myScrollBar->getValue() < myScrollBar->getMaxValue() - myScrollBar->getValueWidth())
            myScrollBar->setValue(myScrollBar->getMaxValue() - myScrollBar->getValueWidth());
        break;

    case (char)key_pgup :
        setIndex(0);
        if ( myScrollBar->getValue() ) myScrollBar->setValue(0);
        break;
    }
}

void listBox::onKeyUp(char keyCode)
{
    keyDown = none;
}

void listBox::onTick()
{
    myScrollBar->onTick();
    for ( unsigned int i = 0; i < items.size(); i++ )
        items[i]->onTick();

    if ( waitCounter*t < waitInterval )
        waitCounter++;
    else
    {
        if ( keyDown == increase )
        {
            if ( listIndex < (signed)items.size() - 1 )
                setIndex(listIndex + 1);
            if ( myScrollBar->getValue() + myScrollBar->getValueWidth() <= listIndex )
                myScrollBar->setValue(myScrollBar->getValue() + 1);
        }
        else if ( keyDown == decrease )
        {
            if ( 0 < listIndex )
                setIndex(listIndex - 1);
            if ( listIndex < myScrollBar->getValue() )
                myScrollBar->setValue(myScrollBar->getValue() - 1);
        }
    }
}
/// ----------------------------------------------------------------------------------------------------------------


/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
void listBox::initListBox(std::vector<std::string> items0)
{
    tabStop = true;
    height -= height % itemHeight;

    myScrollBar = new scrollBar(x + width, y, height, vertical ,0 , items0.size());
    myScrollBar->x -= myScrollBar->width; // csak a konstruktora lefutása után tudom megmondani a scrollBar szélességét
    myScrollBar->setValueWidth(height / itemHeight);
    if ( myScrollBar->getMaxValue() == myScrollBar->getValueWidth() ) myScrollBar->visible = false;

    for (unsigned int i = 0; i < items0.size(); i++)
    {
        if ( myScrollBar->visible ) items.push_back(new label(x, y, width - myScrollBar->width, itemHeight, items0[i]));
        else  items.push_back(new label(x, y, width, itemHeight, items0[i]));
        items[i]->setMargin(5, 0);
        items[i]->background = true;
        items[i]->display = trimEnd;
    }

    listIndex = 0;
}
listBox::listBox(int x0, int y0, int width0, int height0) : widget(x0, y0, width0, height0)
{
    initListBox({});
    eventChanged = [] (int, widget*) {};
}
listBox::listBox(int x0, int y0, int width0, int height0, vector<string> items0) : widget(x0, y0, width0, height0)
{
    initListBox(items0);
    eventChanged = [] (int, widget*) {};
}
listBox::listBox(int x0, int y0, int width0, int height0, std::vector<std::string> items0, std::function<void(int, widget*)> f0) : widget(x0, y0, width0, height0)
{
    initListBox(items0);
    eventChanged = f0;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------------------- Get/Set ----------------------------------------------------------
void listBox::addItem(string newItem)
{
    myScrollBar->setRange(0, items.size() + 1);

    // Ha látható a scrollBar, mert amúgy nem férnek el az elemek
    if ( myScrollBar->visible )
    {
        items.push_back( new label(x, y, width - myScrollBar->width, itemHeight, newItem) );
        myScrollBar->setValueWidth(height / itemHeight);
    }
    // Ha ugyan nem látszik, de már nem férnének el
    else if ( height / itemHeight < myScrollBar->getMaxValue() )
    {
        myScrollBar->visible = true;
        myScrollBar->setValueWidth(height / itemHeight);
        items.push_back( new label(x, y, width - myScrollBar->width, itemHeight, newItem) );
        for ( auto element : items )
            element->width = width - myScrollBar->width;
    }
    // Ha még így is elférnek
    else
        items.push_back( new label(x, y, width, itemHeight, newItem) );

    // mindenesetre legyen háttere, ne legyen felső margója, és a végéről vágja le azokat a karaktereket, amik nem férnek ki
    items[items.size()-1]->setMargin(5, 0);
    items[items.size()-1]->background = true;
    items[items.size()-1]->display = trimEnd;
    items[items.size()-1]->textRefresh();
}

void listBox::addItems(std::vector<std::string> newItems)
{
    for (auto element : newItems )
        addItem(element);
}

void listBox::deleteItem(unsigned int i)
{
    if ( items.size() <= i )
    {
        cerr << "Item no." << i << " cannot be deleted! - listBox::setIndex" << std::endl;
        return;
    }

    items.erase(items.begin() + i);

    if ( items.size() <= i && 0 < items.size() ) listIndex = --i;

    eventChanged(listIndex, this);
}

void listBox::clearList()
{
    while ( 0 < items.size() ) items.pop_back();
}

string listBox::getItem(int i)
    { return items[i]->getText(); }

std::string listBox::getSelectedItem()
    { return items[listIndex]->getText(); }

int listBox::getCount()
    { return items.size(); }

bool listBox::setIndex(int newValue)
{
    if ( newValue < (signed)items.size() )
    {
        listIndex = newValue;
        eventChanged(listIndex, this);
        eventGroupChanged(this);
        return true;
    }
    else
    {
        listIndex = items.size() - 1;
        cerr << "Given index is greater than the number of items! - listBox::setIndex" << std::endl;
        return false;
    }
}

int listBox::getIndex()
    { return listIndex; }
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
void listBox::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if (x < 0) { cerr << "Object is out of window! (x < 0) - listBox::draw" << endl; alreadyAlarmed = true; }
        else if (y < 0) { cerr << "Object is out of window! (y < 0) - listBox::draw" << endl; alreadyAlarmed = true; }
        else if (X < x + width) { cerr << "Object is out of window! (X < x + width) - listBox::draw" << endl; alreadyAlarmed = true; }
        else if (Y < y + height) { cerr << "Object is out of window! (Y < y + height) - listBox::draw" << endl; alreadyAlarmed = true; }
    }

    colorize(White);
    gout << move_to(x, y) << box(width, height);

    if ( myScrollBar->visible )
    {
        // Nem látható elemek elrejtése (az onClick eseményben lesz szükség rá, hogy tudjuk, melyik látható)
        for ( int i = 0; i < myScrollBar->getValue(); i++ )
            items[i]->visible = false;

        // Látható elemek kirajzolása
        for ( int i = myScrollBar->getValue(); i < myScrollBar->getValue() + myScrollBar->getValueWidth(); i++ )
        {
            if (focus && listIndex == i) items[i]->backgroundColor = ActiveColor;
            else if (listIndex == i) items[i]->backgroundColor = Gray;
            else items[i]->backgroundColor = White;
            items[i]->y = y + (i - myScrollBar->getValue())*itemHeight;
            items[i]->draw();
            items[i]->visible = true;
        }

        // Nem látható elemek elrejtése (az onClick eseményben lesz szükség rá, hogy tudjuk, melyik látható)
        for ( int i = myScrollBar->getValue() + myScrollBar->getValueWidth(); i < (signed)items.size(); i++ )
            items[i]->visible = false;
    }
    else
    {
        for ( int i = 0; i < (signed)items.size(); i++ )
        {
            if ( focus && listIndex == i ) items[i]->backgroundColor = ActiveColor;
            else if ( listIndex == i ) items[i]->backgroundColor = Gray;
            else items[i]->backgroundColor = White;
            items[i]->y = y + (i - myScrollBar->getValue())*itemHeight;
            items[i]->draw();
            items[i]->visible = true;
        }
    }

    rectange(x, y, width, height);
    myScrollBar->draw();

}
/// ----------------------------------------------------------------------------------------------------------------


/// ----------------------------------------- Detruktorok ----------------------------------------------------------
listBox::~listBox()
{
    for (unsigned int i = 0; i < items.size(); i++)
        delete items[i];
    delete myScrollBar;
}
/// ----------------------------------------------------------------------------------------------------------------



