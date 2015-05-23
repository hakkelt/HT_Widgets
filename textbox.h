#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED

#include "label.h"

class textBox : public label
{
private:

    char pressedKey; // Az épp lenyomott billentyű
    const unsigned int pressInterval = 1000; // Ennyi idő után kezdi el folyamatosan kiírni a lenyomonyott karaktert
    unsigned int pressedTime; // Ennyi ideje van lenyomva a billentyű

    const unsigned int cursorInterval = 500; // Kurzor villogásának az időtartama
    bool cursorOn; // Épp látszik-e a kurzor
    unsigned int cursorTime; // Ennyi ideje látszik/nem látszik a kurzor

protected:
    void textInit(); // Konstuktorok közös parancsai (mezők inicializása)

public:
    /// ---- Mezők ----
    std::string allowedChar;

    /// ---- Konstruktorok ----
    textBox(int x0, int y0, unsigned int width0, unsigned int height0);
    textBox(int x0, int y0, unsigned int width0, unsigned int height0, std::string txt);
    /// ---- Destruktor ----
    ~textBox() {}

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button);
    virtual void onMouseMove(int posX, int posY);
    virtual void onKeyPress(char keyCode);
    virtual void onKeyUp(char keyCode);
    virtual void onTick();

    /// ---- Kirajzoltatás ----
    virtual void draw() const;
};

#endif // TEXTBOX_H_INCLUDED
