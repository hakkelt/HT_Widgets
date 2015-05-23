#ifndef CODEGENERATOR_H_INCLUDED
#define CODEGENERATOR_H_INCLUDED

#include "application.h"
#include <vector>

struct property
{
    std::string type;
    std::vector<std::string> propertyNames;
    std::vector<std::string> properties;
    std::string name;
    std::string group;
};

class codeGenerator : public application
{
private:
    int selectedWidget;
    void saveXML();
    void saveH();
    void saveCPP();

public:
    /// ---- Widgetek ----
    frame * windowArea;
    frame * controlBar;
    textBox * projectName;
    listBox * controls;
    listBox * groupList;
    textBox * newGroupName;
    button * addGroup;
    textBox * name;
    textBox * x_koord;
    textBox * y_koord;
    textBox * width_koord;
    textBox * height_koord;
    listBox * propertiesList;
    textBox * newProperty;
    button * saveButton;
    button * loadButton;

    /// ---- Események ----
    void window_Click(int _x, int _y, char button);
    void Tick();
    void windowArea_Click(int _x, int _y, char Button, widget* me);
    void widget_KeyPress(int keyCode, widget* me);
    void propertiesList_SelectionChanged(int index, widget* me);
    void groupList_SelectionChanged(int index, widget* me);
    void addGroup_Click(int _x, int _y, char Button, widget* me);
    void widget_Click(int _x, int _y, char button, widget* me);
    void resizeWidget(int _x, int _y, char Button, widget* me);
    void name_KeyPress(int keyCode, widget* me);
    void x_koord_KeyPress(int keyCode, widget* me);
    void y_koord_KeyPress(int keyCode, widget* me);
    void width_koord_KeyPress(int keyCode, widget* me);
    void height_koord_KeyPress(int keyCode, widget* me);
    void newProperty_KeyPress(int keyCode, widget* me);
    void saveButton_Click(int _x, int _y, char Button, widget* me);
    void loadButton_Click(int _x, int _y, char Button, widget* me);

    /// ---- Konstruktor, destruktor ----
    codeGenerator();
    virtual ~codeGenerator() {}

    /// ---- Mezõk feltöltése, tulajdonások beállítása ----
    virtual void init();
};

#endif // CODEGENERATOR_H_INCLUDED
