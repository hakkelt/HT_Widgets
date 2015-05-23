#ifndef CHECKBOX_H_INCLUDED
#define CHECKBOX_H_INCLUDED

#include "widget.h"
#include "label.h"

/// Kipipálós mezõ

class checkBox : public widget
{
private:

    // Pipálható terület paraméterei
    const int boxX = 2, boxY = 2;
    const int boxW = 10, boxH = 10;

protected:
    label *myLabel;

public:
    /// ---- Mezők ----
    bool checked;

    std::function<void(bool, widget*)> eventChanged;

    /// ---- Konstruktor, Destruktor ----
    checkBox(int x0, int y0, unsigned int width0, unsigned int height0);
    checkBox(int x0, int y0, unsigned int width0, unsigned int height0, std::function<void(bool, widget*)> f0);
    ~checkBox() {}

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button) {}
    virtual void onMouseMove(int posX, int posY) {}
    virtual void onKeyPress(char keyCode);
    virtual void onKeyUp(char keyCode) {}
    virtual void onTick() {}

    /// --- Get/Set ----
    void setText(std::string txt);
    std::string getText() const;

    /// ---- Kirajzolás ----
    void draw() const;
};

#endif // CHECKBOX_H_INCLUDED
