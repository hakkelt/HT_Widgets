#ifndef SCROLLBAR_H_INCLUDED
#define SCROLLBAR_H_INCLUDED

#include "widget.h"

enum scrollAlign { horizontal = 0, vertical = 1 };

class scrollBar : public widget
{
private:

    // Lenyomva tartott billentyű vagy egérgomb a folyamatos görgetéshez
    char mouseDown;
    int prevPos;
    int prevVal;
    char keyDown;

    // Hogy ne azonnal kezdjen el görgetni
    bool wait;
    const unsigned int waitInterval = 500;
    unsigned int waitCounter;

    const int thickness = 20; // ScrollBar vastagsága (függőleges esetén szélesség, vízszintesnél magasság)
    float numRange; // szélsőértékeinek küldönbsége
    float graphicRange; // Szélső növelő/csökkentő gombok közti tér hossza
    int scrollHandleLength; // Csuszka hossza
    int scrollHandlePosition; // Csuszka helye

protected:
    int value;
    int maxValue, minValue;
    int valueWidth;
    scrollAlign alignment;

    void initScrollBar(int length, scrollAlign align, int minV, int maxV);

public:
    std::function<void(int, widget*)> eventChanged;

    /// ---- Konstruktor, Destruktor ----
    scrollBar(int x0, int y0, int length, scrollAlign align, int minV, int maxV);
    scrollBar(int x0, int y0, int length, scrollAlign align, int minV, int maxV, std::function<void(int, widget*)> f0);
    ~scrollBar() {}

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button);
    virtual void onMouseMove(int posX, int posY);
    virtual void onKeyPress(char keyCode);
    virtual void onKeyUp(char keyCode);
    virtual void onTick();

    /// --- Get/Set ----
    void setValue(int newValue);
    int getValue() const;

    void setValueWidth(int newValue);
    int getValueWidth() const;

    bool setRange(int minV, int maxV);
    int getMinValue() const;
    int getMaxValue() const;
    int getNumRange() const;


    /// ---- Kirajzolás ----
    virtual void draw() const;
};

#endif // SCROLLBAR_H_INCLUDED
