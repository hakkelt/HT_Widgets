#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <functional>
#include "graphics.hpp"
#include <iostream>
#include <vector>
#include <sstream>

/// ---- Vezérlők --------------------
class application;
#include "widget.h"
#include "node.h"
#include "color.h"

#include "label.h"
#include "checkbox.h"
#include "textbox.h"
#include "scrollBar.h"
#include "listBox.h"
#include "button.h"
#include "radioButton.h"
#include "frame.h"

/// ---- Konstansok ------------------
extern int X;
extern int Y;
extern const unsigned int t;

template <typename T>
std::string convertS(T temp)
{
    std::string out;
    std::stringstream ss;
    ss << temp;
    ss >> out;
    return out;
}
int convertI(std::string temp);
float convertF(std::string temp);

/// ---- Application osztály --------------------
class application
{
protected:
    std::vector<widget*> widgets;
    std::vector<node*> groups;
    unsigned int tabIndex;
    bool shutdownFlag;
    bool keyControl, keyAlt;

    /// ---- Függvények ----
    void scrClear(); // Ablak törlése
    bool keyEvents(genv::event &ev); // Gyorsbillyentyűk elkapása
    void initApp(int X0, int Y0, bool fullScreen = false);
    virtual void init() = 0;

    void next();  // Fókusz továbbadása
    bool setFocus(unsigned int newFocus);  // Fókusz állítása adott elemre

public:
    /// ---- Események -----
    std::function<bool(int, bool, bool)> eventHotKeys;
    std::function<void()> timeTick;
    std::function<void(int, int, char)> eventClick;
    std::function<void(int, int, char)> eventMouseUp;
    std::function<void(int, int, char)> eventMouseMove;

    /// ---- Konstruktor / Destruktor ----
    application() {}
    application(int X0, int Y0, bool fullScreen = false);
    application(int X0, int Y0,std::function<bool(int, bool, bool)>f0, bool fullScreen = false);
    ~application();

    /// ---- Bővítés -----
    void add(widget *newWidget);
    int removeWidget(widget *widgetToRemove);
    void removeWidget(int widgetToRemove);
    void removeLastWidget();
    node* newGroup();

    /// ---- Működés ----
    void run();
    void shutdown();
    int findIndex(widget* me);
};

#endif // APPLICATION_H_INCLUDED
