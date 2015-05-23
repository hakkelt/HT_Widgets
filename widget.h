#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include <vector>
#include "graphics.hpp"
#include "color.h"
#include <functional>
class node;

/// Õs widget osztály

class widget
{
protected:
    bool pressed;

public:
    /// ---- Mezők ----
    int x,y;
    int width, height;
    bool tabStop;
    bool focus;
    bool visible;
    node * container;
    node * children;
    void * data;

    std::function<void(int, int, char, widget*)> eventClick;
    std::function<void(int, int, char, widget*)> eventMouseMove;
    std::function<void(int, int, char, widget*)> eventMouseUp;
    std::function<void(int, widget*)> eventKeyPress;
    std::function<void(widget*)> eventGroupChanged;

    /// ---- Konstruktor ----
    widget(int x0, int y0, int width0, int height0);
    /// ---- Destruktor ----
    virtual ~widget();

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button) = 0;
    virtual void onMouseUp(int posX, int posY, char button) = 0;
    virtual void onMouseMove(int posX, int posY) = 0;
    virtual void onKeyPress(char keyCode) = 0;
    virtual void onKeyUp(char keyCode) = 0;
    virtual void onTick() = 0;

    /// ---- Események és kirajzolás ----
    bool event (genv::event &ev);
    virtual void draw() const = 0;

    bool onMe(int x0, int y0);
};

#endif // WIDGET_H_INCLUDED
