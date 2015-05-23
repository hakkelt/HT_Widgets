#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "widget.h"
#include "scrollBar.h"
#include "label.h"
#include <vector>

class listBox : public widget
{
private:
    const int itemHeight = 20;

    // Lenyomva tartott billentyű a folyamatos görgetéshez
    char keyDown;

    // Hogy ne azonnal kezdjen el görgetni
    bool wait;
    const unsigned int waitInterval = 500;
    unsigned int waitCounter;

protected:
    std::vector<label*> items;
    scrollBar *myScrollBar;
    int listIndex;

    void initListBox(std::vector<std::string> items0);

public:
    std::function<void(int, widget*)> eventChanged;

    /// ---- Konstruktor ----
    listBox(int x0, int y0, int width0, int height0);
    listBox(int x0, int y0, int width0, int height0, std::vector<std::string> items0);
    listBox(int x0, int y0, int width0, int height0, std::vector<std::string> items0, std::function<void(int, widget*)> f0);
    /// ---- Destruktor ----
    ~listBox();

    /// ---- Események ----
    virtual void onClick(int posX, int posY, char button);
    virtual void onMouseUp(int posX, int posY, char button);
    virtual void onMouseMove(int posX, int posY);
    virtual void onKeyPress(char keyCode);
    virtual void onKeyUp(char keyCode);
    virtual void onTick();

    /// ---- Get/Set ----
    void addItem(std::string newItem);
    void addItems(std::vector<std::string> newItems);
    void deleteItem(unsigned int i);
    void clearList();
    std::string getItem(int i);
    std::string getSelectedItem();
    int getCount();
    bool setIndex(int newValue);
    int getIndex();

    /// ---- Kirajzolás ----
    virtual void draw() const;
};

#endif // LIST_H_INCLUDED
