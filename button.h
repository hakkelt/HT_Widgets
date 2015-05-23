#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "label.h"

class button : public label
{
private:

protected:
    std::string Text;
    bool pressed;

    void buttonInit(); // Konstuktorok közös parancsai (mezők inicializása)

public:
    /// ---- Mezõk ----


    /// ---- Konstruktorok ----
    button(int x0, int y0, unsigned int width0, unsigned int height0);
    button(int x0, int y0, unsigned int width0, unsigned int height0, std::string txt);
    button(int x0, int y0, unsigned int width0, unsigned int height0, std::string txt, std::function<void(int, int, char, widget*)> f0);
    /// ---- Destruktor ----
    ~button() {}

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button);
    virtual void onMouseMove(int posX, int posY) {}
    virtual void onKeyPress(char keyCode) {}
    virtual void onKeyUp(char keyCode) {}
    virtual void onTick();

    /// ---- Kirajzolás ----
    virtual void draw() const;
};

#endif // BUTTON_H_INCLUDED
