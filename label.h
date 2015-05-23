#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include "graphics.hpp"
#include "widget.h"
#include "scrollBar.h"

/// Statikus szöveg megjelenítésére szolgáló osztály

enum textAlign { align_left = 1, align_right = 2, align_center = 3 };
enum textDisplay { autoSize, trimBegin, trimEnd, wordWrap };
// enum borderStyle { flat, deep, high };

class label : public widget
{
private:
    int longestRow;

protected:
    std::string Text;
    std::string dispText; // A label szövegének képernyőn megjelenő része
    int numberOfRows; // Tördelt szöveg sorainak száma
    int lineHeight; // Egy sor magassága

    void labelInit(); // Konstuktorok közös parancsai (mezők inicializása)
    void labelDraw() const; // Öröklődés miatti overloading miatt szükség volt a draw függvény törzsének kiemelésére

    scrollBar *VScrollBar;

public:
    /// ---- Mezők ----
    Color fontColor;
    Color backgroundColor;
    int marginSide, marginTop;
    bool background; // van-e háttér
    textAlign align; // szöveg igazítása
    textDisplay display; // Túl hosszú szöveg kezelése
    borderStyle style;

    std::function<void(std::string, widget*)> eventChanged;

    /// ---- Konstruktorok ----
    label(int x0, int y0, int width0, int height0);
    label(int x0, int y0, int width0, int height0, std::string txt);
    label(int x0, int y0, int width0, int height0, std::string txt, std::function<void(std::string, widget*)> f0);
    /// ---- Destruktor ----
    ~label();

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button);
    virtual void onMouseMove(int posX, int posY);
    virtual void onKeyPress(char keyCode) {}
    virtual void onKeyUp(char keyCode) {}
    virtual void onTick();

    /// --- Get/Set ----
    void setText(std::string txt);
    std::string getText() const;
    void textRefresh(); // Szöveg újratördelése

    void setMargin(int mS, int mT);

    /// ---- Kirajzolás ----
    virtual void draw() const;
};

#endif // LABEL_H_INCLUDED
