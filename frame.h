#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "widget.h"

enum frameStyle { flat, deep, high };

class frame : public widget
{
public:
    /// ---- Mezõk ----
    bool filled;
    Color backgroundColor;
    Color borderColor;
    frameStyle style;

    /// ---- Konstruktor, destruktor ----
    frame(int x0, int y0, int width0, int height0);
    virtual ~frame() {}

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button) {}
    virtual void onMouseUp(int posX, int posY, char button) {}
    virtual void onMouseMove(int posX, int posY) {}
    virtual void onKeyPress(char keyCode) {}
    virtual void onKeyUp(char keyCode) {}
    virtual void onTick() {}

    /// ---- Kirajzolás ----
    virtual void draw() const;
};

#endif // FRAME_H_INCLUDED
