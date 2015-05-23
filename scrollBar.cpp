#include "scrollBar.h"
#include "graphics.hpp"
#include <iostream>
#include <math.h>    // round()
#include "application.h"

using namespace genv;

enum pressed_type { none, scrollHandle, increase, decrease };

/// ------------------------------------------ Események ----------------------------------------------------------
void scrollBar::onClick(int posX, int posY, char button)
{
    if ( alignment == horizontal )
    {
        // Csökkent
        if ( x < posX && posX < x + thickness )
        {
            setValue(value-1);
            mouseDown = decrease;
            waitCounter = 0;
        }
        // Növel
        else if ( x + width - thickness < posX && posX < x + width )
        {
            setValue(value+1);
            mouseDown = increase;
            waitCounter = 0;
        }
        // Csuszkára kattint
        else if ( x + thickness + scrollHandlePosition < posX && posX < x + thickness + scrollHandlePosition + scrollHandleLength )
        {
            mouseDown = scrollHandle;
            prevPos = posX;
            prevVal = value;
        }
        // Csuszka elé kattint
        else if ( x + thickness < posX && posX < x + thickness + scrollHandlePosition )
            setValue(value - numRange/5);
        // Csuszka után kattint
        else if ( x + thickness + scrollHandlePosition + scrollHandleLength < posX && posX < x + width - thickness )
            setValue(value + numRange/5);
    }
    else if (alignment == vertical)
    {
        // Csökkent
        if ( y < posY && posY < y + thickness )
        {
            setValue(value-1);
            mouseDown = decrease;
            waitCounter = 0;
        }
        // Növel
        else if ( y + height - thickness < posY && posY < y + height )
        {
            setValue(value+1);
            mouseDown = increase;
            waitCounter = 0;
        }
        // Csuszkára kattint
        else if ( y + thickness + scrollHandlePosition < posY && posY < y + thickness + scrollHandlePosition + scrollHandleLength )
        {
            mouseDown = scrollHandle;
            prevPos = posY;
            prevVal = value;
        }
        // Csuszka elé kattint
        else if ( y + thickness < posY && posY < y + thickness + scrollHandlePosition )
            setValue(value - numRange/5);
        // Csuszka után kattint
        else if ( y + thickness + scrollHandlePosition + scrollHandleLength < posY && posY < y + height - thickness )
            setValue(value + numRange/5);
    }
}

void scrollBar::onMouseUp(int posX, int posY, char button)
    { mouseDown = none; }

void scrollBar::onMouseMove(int posX, int posY)
{
    if ( mouseDown == scrollHandle )
    {
        if ( alignment == horizontal )
            setValue( prevVal + round( (posX - prevPos) / (graphicRange - scrollHandleLength) * (numRange - valueWidth) ) );
        else
            setValue( prevVal + round( (posY - prevPos) / (graphicRange - scrollHandleLength) * (numRange - valueWidth) ) );
    }
}

void scrollBar::onKeyPress(char keyCode)
{
    if ( alignment == horizontal )
    {
        switch ( keyCode )
        {
        case (char)key_left :
            setValue(value - 1);
            keyDown = decrease;
            waitCounter = 0;
            break;

        case (char)key_right :
            setValue(value + 1);
            keyDown = increase;
            waitCounter = 0;
            break;

        case (char)key_pgup :
            setValue(minValue);
            break;

        case (char)key_pgdn :
            setValue(maxValue);
            break;
        }
    }
    else
    {
        switch ( keyCode )
        {
        case (char)key_up :
            setValue(value - 1);
            keyDown = decrease;
            waitCounter = 0;
            break;

        case (char)key_down :
            setValue(value + 1);
            keyDown = increase;
            waitCounter = 0;
            break;

        case (char)key_pgup :
            setValue(minValue);
            break;

        case (char)key_pgdn :
            setValue(maxValue);
            break;
        }
    }
}

void scrollBar::onKeyUp(char keyCode)
    { keyDown = none; }

void scrollBar::onTick()
{
    if ( waitCounter*t < waitInterval )
        waitCounter++;
    else
    {
        if ( mouseDown == increase || keyDown == increase ) setValue(value + 1);
        else if ( mouseDown == decrease || keyDown == decrease ) setValue(value - 1);
    }
}
/// ----------------------------------------------------------------------------------------------------------------


/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
void scrollBar::initScrollBar(int length, scrollAlign align, int minV, int maxV)
{
    alignment = align;
    setRange(minV, maxV);
    value = minValue;
    tabStop = true;
    keyDown = none;
    mouseDown = none;

    valueWidth = 0;

    numRange = maxValue - minValue;
    graphicRange = length - 2 * thickness;
    scrollHandleLength = graphicRange / numRange;
    if ( scrollHandleLength < thickness / 2 ) scrollHandleLength = thickness / 2;
    scrollHandlePosition = (graphicRange - scrollHandleLength) * ((value - minValue) / numRange);

    if ( align == horizontal )
    {
        height = thickness;
        width = length;
    }
    else
    {
        width = thickness;
        height = length;
    }
}

scrollBar::scrollBar(int x0, int y0, int length, scrollAlign align, int minV, int maxV) : widget(x0, y0, 0, 0)
{
    initScrollBar(length, align, minV, maxV);
    eventChanged = [] (int, widget*) {};
}

scrollBar::scrollBar(int x0, int y0, int length, scrollAlign align, int minV, int maxV, std::function<void(int, widget*)> f0) : widget(x0, y0, 0, 0)
{
    initScrollBar(length, align, minV, maxV);
    eventChanged = f0;
}
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------------------- Get/Set ----------------------------------------------------------
void scrollBar::setValue(int newValue)
{
    if ( newValue < minValue ) value = minValue;
    else if ( maxValue < newValue + valueWidth ) value = maxValue -  valueWidth;
    else value = newValue;

    scrollHandlePosition = (graphicRange - scrollHandleLength) * (value - minValue) / (numRange - valueWidth);

    eventChanged(value, this);
    eventGroupChanged(this);
}

int scrollBar::getValue() const
    { return value; }

bool scrollBar::setRange(int minV, int maxV)
{
    if ( minV > maxV )
    {
        minValue = 0;
        maxValue = 100;
        std::cerr << "Incorrect range! (minValue is greater or equal to maxValue) - setRange" << std::endl;
        return false;
    }

    minValue = minV;
    maxValue = maxV;

    numRange = maxValue - minValue;
    if ( numRange == 0 ) numRange = 1;

    // scrollHandleLength hosszának a meghatározása
    if ( valueWidth == 0 ) scrollHandleLength = graphicRange/numRange;
    else scrollHandleLength = graphicRange/numRange * valueWidth;
    // Ha túl rövid lenne...
    if ( scrollHandleLength < thickness/2 ) scrollHandleLength = thickness / 2;

    // scrollHandle helyének meghatározása
    scrollHandlePosition = (graphicRange - scrollHandleLength) * (value - minValue) / (numRange - valueWidth);

    return true;
}

int scrollBar::getMinValue() const
    { return minValue; }

int scrollBar::getMaxValue() const
    { return maxValue; }

void scrollBar::setValueWidth(int newValue)
{
    if ( newValue == 0 ) valueWidth = 1;
    else if ( numRange < newValue ) valueWidth = numRange;
    else valueWidth = newValue;

    if ( maxValue < value + valueWidth ) value = minValue;

    scrollHandleLength = graphicRange/numRange * valueWidth;
    if ( scrollHandleLength < thickness / 2 ) scrollHandleLength = thickness / 2;
}

int scrollBar::getValueWidth() const
    { return valueWidth; }

int scrollBar::getNumRange() const
    { return numRange; }
/// ----------------------------------------------------------------------------------------------------------------


/// --------------------------------- Mûködéshez szükséges függvények ------------------------------------------------
enum directions { left, up, right, down };

void drawArrow(int x, int y, int thickness, directions d)
{
    colorize(Black);

    switch ( d )
    {
    case left :
        gout << move_to(x + thickness*2/3, y + thickness/3) << line(-thickness/3, thickness/3) << line(thickness/3, thickness/3);
        break;

    case up :
        gout << move_to(x + thickness/3, y + thickness*2/3) << line(thickness/3, -thickness/3) << line(thickness/3, thickness/3);
        break;

    case right :
        gout << move_to(x + thickness/3, y + thickness/3) << line(thickness/3, thickness/3) << line(-thickness/3, thickness/3);
        break;

    case down :
        gout << move_to(x + thickness/3, y + thickness/3) << line(thickness/3, thickness/3) << line(thickness/3, -thickness/3);
        break;
    }
}

void scrollBar::draw() const
{
    if ( !visible ) return;

    static bool alreadyAlarmed = false;
    if ( !alreadyAlarmed )
    {
        if ( x < 0 ) { std::cerr << "Object is out of window! (x < 0) - scrollBar::draw" << std::endl; alreadyAlarmed = true; }
        else if ( y < 0 ) { std::cerr << "Object is out of window! (y < 0) - scrollBar::draw" << std::endl; alreadyAlarmed = true; }
        else if ( X < x + width ) { std::cerr << "Object is out of window! (X < x + width) - scrollBar::draw" << std::endl; alreadyAlarmed = true; }
        else if ( Y < y + height ) { std::cerr << "Object is out of window! (Y < y + height) - scrollBar::draw" << std::endl; alreadyAlarmed = true; }
    }

    if ( focus ) colorize(ActiveColor);
    else colorize(Gray);
    gout << move_to(x,y) << box(width, height);

if ( alignment == horizontal )
    {
        colorize(ButtonColor);
            gout << move_to(x + thickness + scrollHandlePosition, y) << box(scrollHandleLength, thickness);

            gout << move_to(x, y) << box(thickness, thickness);
            gout << move_to(x + width - thickness, y) << box(thickness, thickness);

        rectange3D(x, y, thickness, thickness, (mouseDown == decrease) );
        rectange3D(x + width - thickness, y, thickness, thickness, (mouseDown == increase) );

        drawArrow(x, y, thickness, left);
        drawArrow(x + width - thickness, y, thickness, right);

        rectange3D(x + thickness + scrollHandlePosition, y, scrollHandleLength, thickness, false);
    }
    else
    {
        colorize(ButtonColor);
            gout << move_to(x, y + thickness + scrollHandlePosition) << box(thickness, scrollHandleLength);

            gout << move_to(x, y) << box(thickness, thickness);
            gout << move_to(x, y + height - thickness) << box(thickness, thickness);

        rectange3D(x, y, thickness, thickness, (mouseDown == decrease) );
        rectange3D(x, y + height - thickness, thickness, thickness, (mouseDown == increase) );

        drawArrow(x, y, thickness, up);
        drawArrow(x, y + height - thickness, thickness, down);

        rectange3D(x, y + thickness + scrollHandlePosition + 1, thickness, scrollHandleLength - 2, false);
    }
}
/// ----------------------------------------------------------------------------------------------------------------

