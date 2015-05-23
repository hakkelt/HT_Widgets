#include "application.h"
#include <fstream>

using namespace genv;
using namespace std;

int X = 0; int Y = 0;

/// ---------------------------------------- Konstansok ----------------------------------------------------------
const unsigned int t = 40;  // timer idõköze


/// ---------------------------------------- Függvények ----------------------------------------------------------
// Ablak törlése
void application::scrClear()
{
    colorize(BackgroundColor);
    gout << move_to(0,0) << box(X,Y);
}

// Általános billentyűvezérlés, gyorsbillentyűk
bool application::keyEvents(event &ev)
{
    if ( ev.keycode == key_tab )
        { next(); return true; }
    if ( ev.keycode == key_escape )
        { shutdownFlag = true; return true; }
    else if ( eventHotKeys(ev.keycode, keyControl, keyAlt) )
        return true;
    else if ( ev.keycode == key_lctrl || ev.keycode == key_rctrl )
        keyControl = true;
    else if ( ev.keycode == -key_lctrl || ev.keycode == -key_rctrl )
        keyControl = false;
    else if ( ev.keycode == key_lalt || ev.keycode == key_ralt )
        keyAlt = true;
    else if ( ev.keycode == -key_lalt || ev.keycode == -key_ralt )
        keyAlt = false;
    return false;
}

void application::shutdown()
    { shutdownFlag = true; }
/// ----------------------------------------------------------------------------------------------------------------



/// ---------------------------------------- Konstruktor / Destruktor ----------------------------------------------
void application::initApp(int X0, int Y0, bool fullScreen)
{
    X = X0; Y = Y0;
    gout.open(X,Y, fullScreen);
    tabIndex = 0;
    keyControl = false; keyAlt = false;
    timeTick = [] () {};
    eventClick = [] (int x, int y, char button) {};
    eventMouseUp = [] (int x, int y, char button) {};
    eventMouseMove = [] (int x, int y, char button) {};
    eventHotKeys = [] (char keyCode, bool Control, bool Alt) { return false; };
    //init();
}

application::application(int X0, int Y0, bool fullScreen)
{
    initApp(X0, Y0, fullScreen);
}

application::application(int X0, int Y0, std::function<bool(int, bool, bool)>f0, bool fullScreen)
{
    initApp(X0, Y0, fullScreen);
    eventHotKeys = f0;
}

application::~application()
{
    for( unsigned int i = 0; i < widgets.size(); i++ )
        delete widgets[i];
}
/// ----------------------------------------------------------------------------------------------------------------



/// ---------------------------------------- Bővítés ---------------------------------------------------------------
void application::add(widget *newWidget)
{
    widgets.push_back(newWidget);
}

int application::removeWidget(widget *widgetToRemove)
{
    for ( unsigned int i = 0; i < widgets.size(); i++ )
    {
        if ( widgets[i] == widgetToRemove )
        {
            delete widgets[i];
            widgets.erase(widgets.begin() + i);
            return i;
        }
    }
    return -1;
}

void application::removeWidget(int widgetToRemove)
{
    delete widgets[widgetToRemove];
    widgets.erase(widgets.begin() + widgetToRemove);
}

void application::removeLastWidget()
{
    delete widgets[widgets.size()-1];
    widgets.pop_back();
}

node* application::newGroup()
{
    node* temp = new node;
    groups.push_back(temp);
    return temp;
}
/// ----------------------------------------------------------------------------------------------------------------



/// ---------------------------------------- Működés ---------------------------------------------------------------
 void application::run()
 {
    shutdownFlag = false;

    event ev;
    gin.timer(t);

    while ( gin >> ev && !shutdownFlag )
    {
        scrClear();

        // Általános billentyűparancsok kezelése (pl.: tab)
        if (ev.type == ev_key && keyEvents(ev)) continue;

        // Vezérlők kirajzolása
        for (auto element : widgets )
            element->draw();

        // Ha több elemhez is eljutna az esemény, akkor csak a legfelső kapja meg, és az kerüljön fókuszba
        int i;
        for (i = widgets.size()-1; i >= 0; i--)
            if (widgets[i]->event(ev)) { setFocus(i); break; }

        // Ha egyik elemre sem veszi fel az egéreseményt, akkor az ablak felveszi
        static char mouseState = 0;
        if ( i < 0 && ev.type == ev_mouse && ev.button != 0 )
        {
            if ( 0 < ev.button )  { eventClick(ev.pos_x, ev.pos_y, ev.button); mouseState = ev.button; }
            else { eventMouseUp(ev.pos_x, ev.pos_y, ev.button); mouseState = 0; }
        }
        if ( i < 0 && ev.type == ev_mouse && ev.button == 0 ) eventMouseMove(ev.pos_x, ev.pos_y, mouseState);

        timeTick();

        gout << refresh;
    }
 }

 int application::findIndex(widget* me)
 {
     for ( unsigned int i = 0; i <widgets.size(); i++ )
        if ( widgets[i] == me ) return i;
     return -1;
 }
/// ----------------------------------------------------------------------------------------------------------------



/// ---------------------------------------- Fókusz ---------------------------------------------------------------
void application::next()  // Fókusz továbbadása
{
    widgets[tabIndex]->focus = false;
    // Legközelebbi fókuszálható elem keresése
    while( !widgets[(++tabIndex %= widgets.size())]->tabStop )
        if (tabIndex > widgets.size() * 2) return;
    widgets[tabIndex]->focus = true;
}

bool application::setFocus(unsigned int newFocus)  // Fókusz állítása adott elemre
{
    if ( newFocus < 0 || widgets.size() <= newFocus || !widgets[newFocus]->tabStop ) return false; // Ha nem állítható az elemre fókusz
    widgets[tabIndex]->focus = false;
    widgets[newFocus]->focus = true;
    tabIndex = newFocus;
    return true;
}
/// ----------------------------------------------------------------------------------------------------------------

#include <sstream>

int convertI(std::string temp)
{
    int out;
    std::stringstream ss;
    ss << temp;
    ss >> out;
    return out;
}

float convertF(std::string temp)
{
    float out;
    std::stringstream ss;
    ss << temp;
    ss >> out;
    return out;
}







