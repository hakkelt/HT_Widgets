#include "widget.h"
#include <iostream>

/// ---------------------------------------- Konstruktorok ----------------------------------------------------------
widget::widget(int x0, int y0, int width0, int height0)
{
    x = x0; y = y0;
    width = width0; height = height0;
    focus = false;
    pressed = false;
    visible = true;
    eventClick = [] (int x, int y, char button, widget*) {};
    eventMouseMove = [] (int x, int y, char button, widget*) {};
    eventMouseUp = [] (int x, int y, char button, widget*) {};
    eventKeyPress = [] (int keyCode, widget*) {};
    eventGroupChanged = [] (widget*) {};
}
widget::~widget()
{
    if ( data ) delete data;
}
/// -----------------------------------------------------------------------------------------------------------------


/// -------------------------------------- Események fogadása --------------------------------------------------------
bool widget::event (genv::event &ev)
{
    static char mouseState = 0;
    if ( !visible ) return false;

    // akkor is meg kell kapnia a felengedés eseményt, ha már nincs fölötte az egér, de fókuszban van, és csak ő kapja meg
    if ( pressed && ev.type == genv::ev_mouse && ev.button < 0 )
    {
        mouseState = 0;
        pressed = false;
        onMouseUp(ev.pos_x, ev.pos_y, -ev.button);
        eventMouseUp(ev.pos_x, ev.pos_y, -ev.button, this);
        return true;
    }
    else if ( ev.type == genv::ev_mouse && onMe(ev.pos_x, ev.pos_y) )
    {
        if ( ev.button == 0 )
        {
            onMouseMove(ev.pos_x, ev.pos_y);
            eventMouseMove(ev.pos_x, ev.pos_y, mouseState, this);
            return false;
        }
        else if ( ev.button > 0 )
        {
            pressed = true;
            mouseState = ev.button;
            onClick(ev.pos_x, ev.pos_y, ev.button);
            eventClick(ev.pos_x, ev.pos_y, ev.button, this);
        }
        return true;
    }
    else if ( ev.type == genv::ev_key && focus )
    {
        if ( ev.keycode < 0 ) onKeyUp(-ev.keycode);
        if ( ev.keycode > 0 ) { onKeyPress(ev.keycode); eventKeyPress(ev.keycode, this); }
        return true;
    }
    else if ( ev.type == genv::ev_timer )
    {
        onTick();
        return false;
    }
    return false;
}

/// ----------------------------------------------------------------------------------------------------------------

bool widget::onMe(int x0, int y0)
{
    return (x <= x0 && x0 <= x + width && y <= y0 && y0 <= y + height);
}


