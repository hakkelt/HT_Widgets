#ifndef RADIOBUTTON_H_INCLUDED
#define RADIOBUTTON_H_INCLUDED

#include "widget.h"
#include "label.h"
#include "node.h"
#include <iostream>

class radioButton : public widget
{
friend class group;
private:
    // Pipálható terület paraméterei
    const int boxX = 2, boxY = 2;
    const int boxW = 13, boxH = 13;

protected:
    bool checked;
    label *myLabel;

public:
    std::function<void(widget*)> eventChanged;
    node * myNode;

    /// ---- Konstruktor, Destruktor ----
    radioButton(int x0, int y0, unsigned int width0, unsigned int height0, node * _node);
    radioButton(int x0, int y0, unsigned int width0, unsigned int height0, node * _node, std::function<void(widget*)> f0);
    virtual ~radioButton() { std::cout << "radioButton: OK\n"; }

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
    void setValue(bool newValue);
    inline bool getValue() { return checked; }

    /// ---- Kirajzolás ----
    void draw() const;
};

#endif // RADIOBUTTON_H_INCLUDED
