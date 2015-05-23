
#ifdef _WIN32
    #include "wtypes.h"
    using namespace std;

    // Get the horizontal and vertical screen sizes in pixel
    void GetDesktopResolution(int& horizontal, int& vertical)
    {
       RECT desktop;
       // Get a handle to the desktop window
       const HWND hDesktop = GetDesktopWindow();
       // Get the size of screen to the variable desktop
       GetWindowRect(hDesktop, &desktop);
       // The top left corner will have coordinates (0,0)
       // and the bottom right corner will have coordinates
       // (horizontal, vertical)
       horizontal = desktop.right;
       vertical = desktop.bottom;
    }
#else
    void GetDesktopResolution(int& horizontal, int& vertical)
    {
       horizontal = 1024;
       vertical = 800;
    }
#endif // _WIN32

/// --------------------------------------------------------------------------------------------------

#include "codeGenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

enum widget_types { Wbutton = 0, WcheckBox, Wframe, Wlabel, WlistBox, WradioButton, WscrollBar_h, WscrollBar_v, WtextBox };

const vector<string> checkBoxProperties = { "checked", "Text" };
const vector<string> frameProperties = { "filled", "backgroundColor", "borderColor", "style" };
const vector<string> labelProperties = { "fontColor", "backgroundColor", "marginSide", "marginTop", "background", "align", "display", "Text", "style" };
const vector<string> buttonProperties = labelProperties;
const vector<string> listBoxProperties = { "items" };
const vector<string> radioButtonProperties = { "Text" };
const vector<string> scrollBarProperties = { "Range", "ValueWidth" };
const vector<string> textBoxProperties = { "fontColor", "backgroundColor", "marginSide", "marginTop", "background", "align", "display", "Text", "style", "allowedChar" };

int prev_x, prev_y;
bool notToMoveFlag;

/// ------------------------------------ Események ----------------------------------------------------
void codeGenerator::window_Click(int _x, int _y, char button)
{
    windowArea->visible = true;
    windowArea->width = _x;
    windowArea->height = _y;
}

void codeGenerator::widget_Click(int _x, int _y, char button, widget* me)
{
    prev_x = _x;
    prev_y = _y;
    notToMoveFlag = false;

    selectedWidget = findIndex(me);

    name->setText( ((property*)me->data)->name );
    x_koord->setText( convertS(me->x) );
    y_koord->setText( convertS(me->y) );
    width_koord->setText( convertS(me->width) );
    height_koord->setText( convertS(me->height) );
    propertiesList->clearList();
    propertiesList->addItems( ((property*)me->data)->propertyNames );
    for ( int i = 0; i < groupList->getCount(); i++ )
    {
        if ( groupList->getItem(i) == ((property*)me->data)->group )
        {
            groupList->setIndex(i);
            break;
        }
    }
}

void resizeWindow(int _x, int _y, char Button, widget* me)
{
    if ( Button != 1 ) return; // Csak akkor mérétezem át az ablakot, ha le van nyomva a bal gomb

    if ( me->x + me->width - 20 < _x && _x < me->x + me->width - 5 )
        me->width += _x - prev_x;
    if ( me->y + me->height - 20 < _y && _y < me->y + me->height - 5 )
        me->height += _y - prev_y;
    prev_x = _x;
    prev_y = _y;
}

void codeGenerator::resizeWidget(int _x, int _y, char Button, widget* me)
{
    if ( Button != 1 ) return; // Csak akkor mérétezem át az ablakot, ha le van nyomva a bal gomb
    if ( notToMoveFlag ) return;

    bool onTheLeftEdge = ( me->x + me->width - 15 < _x );
    bool onTheBottomEdge = ( me->y + me->height - 15 < _y );
    if (onTheLeftEdge || onTheBottomEdge )
    {
        if ( onTheLeftEdge ) me->width += _x - prev_x;
        if ( onTheBottomEdge ) me->height += _y - prev_y;
    }
    else
    {
        me->x += _x - prev_x;
        me->y += _y - prev_y;
    }
    prev_x = _x;
    prev_y = _y;

    x_koord->setText( convertS(me->x) );
    y_koord->setText( convertS(me->y) );
    width_koord->setText( convertS(me->width) );
    height_koord->setText( convertS(me->height) );
}

void codeGenerator::widget_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_delete ) removeWidget(me);
}

void codeGenerator::windowArea_Click(int _x, int _y, char Button, widget* me)
{
    if ( (windowArea->width - 20 < _x && _x < windowArea->width - 5) ||
            (windowArea->height - 20 < _y && _y < windowArea->height - 5) )
    {
        prev_x = _x;
        prev_y = _y;
    }
    else if ( _x < windowArea->width - 20 && _y < windowArea->height - 20 )
    {
        notToMoveFlag = true;
        widget * temp;
        property * tempControl = new property;
        switch ( controls->getIndex() )
        {
        case Wbutton :
            temp = new button(_x, _y, 100, 30, "Button" );
            tempControl->propertyNames = buttonProperties;
            tempControl->properties = { "", "", "", "", "", "", "", "", "" };
            tempControl->type = "button";
            break;
        case WcheckBox :
            temp = new checkBox(_x, _y, 100, 30);
            tempControl->propertyNames = checkBoxProperties;
            tempControl->properties = { "", "" };
            tempControl->type = "checkBox";
            break;
        case Wframe :
            temp = new frame(_x, _y, 100, 30);
            tempControl->propertyNames = frameProperties;
            tempControl->properties = { "", "", "", "" };
            tempControl->type = "frame";
            break;
        case Wlabel :
            temp = new label(_x, _y, 100, 30, "label" );
            tempControl->propertyNames = labelProperties;
            tempControl->properties = { "", "", "", "", "", "", "", "", "" };
            tempControl->type = "label";
            break;
        case WlistBox:
            temp = new listBox(_x, _y, 100, 30, { "listBox" } );
            tempControl->propertyNames = listBoxProperties;
            tempControl->properties = { "" };
            tempControl->type = "listBox";
            break;
        case WradioButton:
            temp = new radioButton(_x, _y, 100, 30, groups[0]);
            tempControl->propertyNames = radioButtonProperties;
            tempControl->properties = { "" };
            tempControl->type = "radioButton";
            break;
        case WscrollBar_h:
            temp = new scrollBar(_x, _y, 100, horizontal,0 , 100);
            tempControl->propertyNames = scrollBarProperties;
            tempControl->properties = { "", "" };
            tempControl->type = "scrollBar_h";
            break;
        case WscrollBar_v:
            temp = new scrollBar(_x, _y, 100, vertical,0 , 100);
            tempControl->propertyNames = scrollBarProperties;
            tempControl->properties = { "", "" };
            tempControl->type = "scrollBar_v";
            break;
        case WtextBox:
            temp = new textBox(_x, _y, 100, 30, "textBox" );
            tempControl->propertyNames = textBoxProperties;
            tempControl->properties = { "", "", "", "", "", "", "", "", "", "" };
            tempControl->type = "textBox";
            break;
        }
        tempControl->group = groupList->getSelectedItem();
        tempControl->name = "newWidget";

        temp->tabStop = true;
        temp->eventClick = [=] (int _x, int _y, char button, widget* me) { this->widget_Click(_x, _y, button, temp); };
        temp->eventMouseMove = [=] (int _x, int _y, char button, widget* me) { this->resizeWidget(_x, _y, button, temp); };
        temp->eventKeyPress = [=] (int keyCode, widget * me) { this->widget_KeyPress(keyCode, temp); };
        temp->data = tempControl;
        widgets.push_back( temp );
        selectedWidget = widgets.size() - 1;
        setFocus(selectedWidget);
    }
}

void codeGenerator::addGroup_Click(int _x, int _y, char Button, widget* me)
{
    groupList->addItem(newGroupName->getText());
}

void codeGenerator::propertiesList_SelectionChanged(int index, widget* me)
{
    if ( selectedWidget )
        newProperty->setText( ((property*)widgets[selectedWidget]->data)->properties[index] );
}

void codeGenerator::newProperty_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        ((property*)widgets[selectedWidget]->data)->properties[propertiesList->getIndex()] = newProperty->getText();
}

void codeGenerator::x_koord_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        widgets[selectedWidget]->x = convertI(x_koord->getText());
}
void codeGenerator::y_koord_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        widgets[selectedWidget]->y = convertI(y_koord->getText());
}
void codeGenerator::width_koord_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        widgets[selectedWidget]->width = convertI(width_koord->getText());
}
void codeGenerator::height_koord_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        widgets[selectedWidget]->height = convertI(height_koord->getText());
}
void codeGenerator::name_KeyPress(int keyCode, widget* me)
{
    if ( keyCode == genv::key_enter && selectedWidget )
        ((property*)widgets[selectedWidget]->data)->name = name->getText();
}

void codeGenerator::groupList_SelectionChanged(int index, widget* me)
{
    if ( selectedWidget )
        ((property*)widgets[selectedWidget]->data)->group = groupList->getSelectedItem();
}
void codeGenerator::Tick()
{
    if ( selectedWidget )
    {
        widget * w = widgets[selectedWidget];
        rectange(w->x, w->y, w->width, w->height, Red);
    }
}
/// -------------------------------------------------------------------------------------------------



/// -------------------------------- Mentés és beolvasás --------------------------------------------
void codeGenerator::saveXML()
{
    ofstream f(projectName->getText() + ".xml");

    f << "<?xml -- HT_widget application :: properties table>\n\n";
    f << "<window>\n";
    f << "\t<name>" << projectName->getText() << "</name>\n";
    f << "\t<width>" << windowArea->width << "</width>\n";
    f << "\t<height>" << windowArea->height << "</height>\n";

    f << "\t<nodes>";
    for ( int i = 0; i < groupList->getCount() - 1; i++)
        f << "\"" << groupList->getItem(i) << "\", ";
    f << "\"" << groupList->getItem(groupList->getCount() - 1) << "\"</nodes>\n";

    f << "\t<widgets>\n";
    for ( unsigned int i = 17; i < widgets.size(); i++ )
    {
        f << "\t\t<" << ((property*)widgets[i]->data)->type << ">\n";
        f << "\t\t\t<name>" << ((property*)widgets[i]->data)->name << "</name>\n";
        f  << "\t\t\t<group>" << ((property*)widgets[i]->data)->group << "</group>\n";
        f << "\t\t\t<x>" << widgets[i]->x << "</x>\n";
        f << "\t\t\t<y>" << widgets[i]->y << "</y>\n";
        f << "\t\t\t<width>" << widgets[i]->width << "</width>\n";
        f << "\t\t\t<height>" << widgets[i]->height << "</height>\n";

        unsigned int j = 0;
        for ( auto propName : ((property*)widgets[i]->data)->propertyNames )
        {
            f << "\t\t\t<" << propName << ">";
            f << ((property*)widgets[i]->data)->properties[j++];
            f << "</" << propName << ">\n";
        }

        f << "\t\t</" << ((property*)widgets[i]->data)->type << ">\n";
    }
    f << "\t</widgets>\n</window>";

    f.close();
}

void codeGenerator::saveH()
{
    /// ------------------ .h -------------------------------
    ofstream f(projectName->getText() + ".h");

    string projName = projectName->getText();
    transform(projName.begin(), projName.end(),projName.begin(), ::toupper);
    f << "#ifndef " << projName << "_H_INCLUDED\n";
    f << "#define " << projName << "_H_INCLUDED\n\n";

    f << "#include \"application.h\"\n\n";

    f << "class " << projectName->getText() << " : public application\n";
    f << "{\nprotected:\n";

    f << "\t ///---- Csomópontok ----\n";
    for ( int i = 0; i < groupList->getCount(); i++)
        f << "\tnode * " << groupList->getItem(i) << ";\n";

    f << "\n\t/// ---- Események ----\n";
    f << "\tbool handleHotKeys (int keyCode, bool Control, bool Alt);\n";
    f << "\n\t// ---- Csomópontok ----\n";
    for ( int i = 0; i < groupList->getCount(); i++ )
        f << "\tvoid " << groupList->getItem(i) << "_groupChanged (widget* me);\n";
    f << endl;

    for ( unsigned int i = 17; i < widgets.size(); i++ )
    {
        property temp = *((property*)widgets[i]->data);
        f << "\t// -- " << temp.name << " --\n";
        f << "\tvoid " << temp.name << "_Click (int _x, int _y, char button, widget* me);\n";
        f << "\tvoid " << temp.name << "_MouseMove (int _x, int _y, char button, widget* me);\n";
        f << "\tvoid " << temp.name << "_MouseUp (int _x, int _y, char button, widget* me);\n";
        f << "\tvoid " << temp.name << "_KeyPress (int keyCode, widget* me);\n";
        if ( temp.type == "checkBox" )
            f << "\tvoid " << temp.name << "_Changed (bool newValue, widget* me);\n";
        else if ( temp.type == "textBox" )
            f << "\tvoid " << temp.name << "_Changed (std::string newValue, widget* me);\n";
        else if ( (temp.type == "listBox") || (temp.type == "scrollBar_h") || (temp.type == "scrollBar_v") )
            f << "\tvoid " << temp.name << "_Changed (int newValue, widget* me);\n";
        else if ( temp.type == "radioButton" )
            f << "\tvoid " << temp.name << "_Changed (widget* me);\n";
        f << endl;
    }

    f << "\npublic:\n";

    f << "\t ///---- Widgetek ----\n";
    for ( unsigned int i = 17; i < widgets.size(); i++ )
    {
        string type = ((property*)widgets[i]->data)->type;
        if ( type == "scrollBar_h" )
            f << "\tscrollBar * " << ((property*)widgets[i]->data)->name << ";\n";
        else if ( type == "scrollBar_v" )
            f << "\tscrollBar * " << ((property*)widgets[i]->data)->name << ";\n";
        else
            f << "\t" << type<< " * " << ((property*)widgets[i]->data)->name << ";\n";
    }

    f << "\n\t/// ---- Konstruktor, destruktor ----\n";
    f << "\t" << projectName->getText() << "();\n\tvirtual ~" << projectName->getText() << "() {}\n";

    f << "\t/// ---- Mezõk feltöltése, tulajdonások beállítása ----\n";
    f << "\tvirtual void init();\n};\n";

    f << "\n#endif // " << projName << "_H_INCLUDED";

    f.close();
}

void codeGenerator::saveCPP()
{
    /// ------------------ .cpp ------------------------------
    string projName = projectName->getText();
    ofstream f(projName + ".cpp");

    f << "#include \"" << projName << ".h\"\n";
    f << "#include <iostream>\n\n";

    f << "using namespace std;\n\n";

    f << "/// ---------------------------------------- Események ----------------------------------------------------------\n";
    f << "bool " << projName << "::" << "handleHotKeys (int keyCode, bool Control, bool Alt) { return false; }\n";
    f << "// ---- Csomópontok ----\n";
    for ( int i = 0; i < groupList->getCount(); i++ )
        f << "void " << projName << "::" << groupList->getItem(i) << "_groupChanged (widget* me) {}\n";

    for ( unsigned int i = 17; i < widgets.size(); i++ )
    {
        property temp = *((property*)widgets[i]->data);
        f << "// -- " << temp.name << " --\n";
        f << "void " << projName << "::" << temp.name << "_Click (int _x, int _y, char button, widget* me) {}\n";
        f << "void " << projName << "::" << temp.name << "_MouseMove (int _x, int _y, char button, widget* me) {}\n";
        f << "void " << projName << "::" << temp.name << "_MouseUp (int _x, int _y, char button, widget* me) {}\n";
        f << "void " << projName << "::" << temp.name << "_KeyPress (int keyCode, widget* me) {}\n";
        if ( temp.type == "checkBox" )
            f << "void " << projName << "::" << temp.name << "_Changed (bool newValue, widget* me) {}\n";
        else if ( temp.type == "textBox" )
            f << "void " << projName << "::" << temp.name << "_Changed (std::string newValue, widget* me) {}\n";
        else if ( temp.type == "listBox" || temp.type == "scrollBar" )
            f << "void " << projName << "::" << temp.name << "_Changed (int newValue, widget* me) {}\n";
        if ( temp.type == "radioButton" )
            f << "void " << projName << "::" << temp.name << "_Changed (widget* me) {}\n";
        f << endl;
    }
    f << "/// ------------------------------------------------------------------------------------------------------------\n";
    f << endl << endl << endl;

    f << "/// ---------------------------------------- Konstruktor --------------------------------------------------------\n";
    f << projName << "::" << projName << "()";
        f << " : application(" << windowArea->width << ", " <<windowArea->height <<  ")\n";
        f << "{\n\tthis->eventHotKeys = [=] (int keyCode, bool Control, bool Alt)\n";
        f << "\t\t{ return this->handleHotKeys(keyCode, Control, Alt); };\n}\n";
    f << "/// ------------------------------------------------------------------------------------------------------------\n";
    f << endl << endl << endl;

    f << "/// ---------------------------- Mezõk feltöltése, tulajdonások beállítása -------------------------------------\n";
    f << "void " << projName << "::" << "init()\n{\n";
    f << "\n\t ///---- Csomópontok ----\n";
    for ( int i = 0; i < groupList->getCount(); i++)
        f << "\t" << groupList->getItem(i) << " = newGroup();\n";

    f << "\n\t ///---- Widgetek ----\n";
    for ( unsigned int i = 17; i < widgets.size(); i++ )
    {
        property temp = *((property*)widgets[i]->data);
        f << "\t // -- " << temp.name << " --\n";
        if ( temp.type == "scrollBar_v" )
        {
            f << "\tadd( " << temp.name << " = new scrollBar(";
                f << widgets[i]->x << ", " << widgets[i]->y << ", " << widgets[i]->height << ", vertical, 0, 100) );\n";
        }
        else if ( temp.type == "scrollBar_h" )
        {
            f << "\tadd( " << temp.name << " = new scrollBar(";
                f << widgets[i]->x << ", " << widgets[i]->y << ", " << widgets[i]->width << ", horizontal, 0, 100) );\n";
        }
        else if ( temp.type == "radioButton" )
        {
            f << "\tadd( " << temp.name << " = new " << temp.type << "(";
                f << widgets[i]->x << ", " << widgets[i]->y << ", " << widgets[i]->width << ", " << widgets[i]->height;
                f << ", " << temp.group << ") );\n";
        }
        else
        {
            f << "\tadd( " << temp.name << " = new " << temp.type << "(";
                f << widgets[i]->x << ", " << widgets[i]->y << ", " << widgets[i]->width << ", " << widgets[i]->height << ") );\n";
        }
        unsigned int j = 0;
        for ( auto propName : temp.propertyNames )
        {
            if ( temp.properties[j] == "" ) j++;
            else if ( propName == "Text" || propName == "Range" || propName == "ValueWidth" )
            {
                f << "\t" << temp.name << "->set" << propName << "(";
                f << temp.properties[j++] << ");\n";
            }
            else if ( propName == "items" )
            {
                f << "\t" << temp.name << "->addItems( { ";
                f << temp.properties[j++] << " } );\n";
            }
            else
            {
                f << "\t" << temp.name << "->" <<  propName << " = ";
                f << temp.properties[j++] << ";\n";
            }
        }
        f << "\t" << temp.name << "->eventClick = [=] " << "(int _x, int _y, char Button, widget* me)\n";
            f << "\t\t\t\t{ this->" << temp.name << "_Click(_x, _y, Button, " << temp.name << "); };\n";
        f << "\t" << temp.name << "->eventMouseMove = [=] " << "(int _x, int _y, char Button, widget* me)\n";
            f << "\t\t\t\t{ this->" << temp.name << "_MouseMove(_x, _y, Button, " << temp.name << "); };\n";
        f << "\t" << temp.name << "->eventMouseUp = [=] " << "(int _x, int _y, char Button, widget* me)\n";
            f << "\t\t\t\t{ this->" << temp.name << "_MouseUp(_x, _y, Button, " << temp.name << "); };\n";
        f << "\t" << temp.name << "->eventKeyPress = [=] " << "(int keyCode, widget* me)\n";
            f << "\t\t\t\t{ this->" << temp.name << "_KeyPress(keyCode, " << temp.name << "); };\n";
        f << "\t" << temp.name << "->eventGroupChanged = [=] " << "(widget* me)\n";
            f << "\t\t\t\t{ this->" << temp.group << "_groupChanged(" << temp.name << "); };\n";
        if ( temp.type == "checkBox" )
            {f << "\t" << temp.name << "->eventChanged = [=] " << "(bool newValue, widget* me)\n";
                f << "\t\t\t\t{ this->" << temp.name << "_Changed(newValue, " << temp.name << "); };\n";}
        else if ( temp.type == "textBox" )
            {f << "\t" << temp.name << "->eventChanged = [=] " << "(std::string newValue, widget* me)\n";
                f << "\t\t\t\t{ this->" << temp.name << "_Changed(newValue, " << temp.name << "); };\n";}
        else if ( temp.type == "listBox" || temp.type == "scrollBar" )
            {f << "\t" << temp.name << "->eventChanged = [=] " << "(int newValue, widget* me)\n";
                f << "\t\t\t\t{ this->" << temp.name << "_Changed(newValue, " << temp.name << "); };\n";}
        else if ( temp.type == "radioButton" )
            {f << "\t" << temp.name << "->eventChanged = [=] " << "(widget* me)\n";
                f << "\t\t\t\t{ this->" << temp.name << "_Changed(" << temp.name << "); };\n";}

        f << "\t*" << temp.group << " << " << temp.name << ";\n";
        f << endl;
    }
    f << "}\n/// ------------------------------------------------------------------------------------------------------------\n";
    f << endl << endl << endl;

    f.close();
}

void codeGenerator::saveButton_Click(int _x, int _y, char Button, widget* me)
{
    saveXML();
    saveH();
    saveCPP();
}

string getBetween(string file, string openTag, string closeTag, int &pos, bool inc = true)
{
    int begining = file.find(openTag, pos) + openTag.length();
    int ending = file.find(closeTag, pos);
    if ( ending < 1 || ending >= (signed)file.length() ) return "";
    string temp = file.substr(begining, ending - begining);
    if ( inc ) pos = file.find(closeTag, pos) + closeTag.length();
    return temp;
}

void codeGenerator::loadXML(string projName)
{
    ifstream f(projName + ".xml");
    string file;
    getline(f, file, (char)EOF);
    f.close();

    int pos = 0;

    projectName->setText(getBetween(file, "<name>", "</name>", pos));
    windowArea->width = convertI(getBetween(file, "<width>", "</width>", pos));
    windowArea->height = convertI(getBetween(file, "<height>", "</height>", pos));

    groupList->clearList();
    string nodes = getBetween(file, "<nodes>", "</nodes>", pos);
    stringstream ss; string t;
    ss << nodes;
    getline(ss, t, '"');
    getline(ss, t, '"');
    while ( t != "" )
    {
        groupList->addItem(t);
        getline(ss, t, '"');
        getline(ss, t, '"');
    }

    getBetween(file, "<", ">", pos);

    int limit = 0; // végtelen ciklus elkerülése végett
    while ( getBetween(file, "<", ">", pos, false) != "/widgets" && limit++ < 100 )
    {
        property * p = new property;
        p->type = getBetween(file, "<", ">", pos);
        p->name = getBetween(file, "<name>", "</name>", pos);
        p->group = getBetween(file, "<group>", "</group>", pos);

        int _x, _y, _width, _height;
            _x = convertI(getBetween(file, "<x>", "</x>", pos));
            _y = convertI(getBetween(file, "<y>", "</y>", pos));
            _width = convertI(getBetween(file, "<width>", "</width>", pos));
            _height = convertI(getBetween(file, "<height>", "</height>", pos));

        widget *w;
        if ( p->type == "button" )
        {
            w = new button(_x, _y, _width, _height, "Button" );
            p->propertyNames = buttonProperties;
        }
        else if ( p->type == "checkBox" )
        {
            w = new checkBox(_x, _y, _width, _height);
            p->propertyNames = checkBoxProperties;
        }
        else if ( p->type == "frame" )
        {
            w = new frame(_x, _y, _width, _height);
            ((frame*)w)->filled = true;
            w->visible = true;
            p->propertyNames = frameProperties;
        }
        else if ( p->type == "label" )
        {
            w = new label(_x, _y, _width, _height, "Label" );
            p->propertyNames = labelProperties;
        }
        else if ( p->type == "listBox" )
        {
            w = new listBox(_x, _y, _width, _height, { "listBox" } );
            p->propertyNames = listBoxProperties;
        }
        else if ( p->type == "radioButton" )
        {
            w = new radioButton(_x, _y, _width, _height, groups[0] );
            p->propertyNames = radioButtonProperties;
        }
        else if ( p->type == "scrollBar_h" )
        {
            w = new scrollBar(_x, _y, _width, horizontal,0 , 100);
            p->propertyNames = scrollBarProperties;
        }
        else if ( p->type == "scrollBar_v" )
        {
            w = new scrollBar(_x, _y, _height, vertical,0 , 100);
            p->propertyNames = scrollBarProperties;
        }
        else if ( p->type == "textBox" )
        {
            w = new textBox(_x, _y, _width, _height, "TextBox" );
            p->propertyNames = textBoxProperties;
        }

        for ( auto element : p->propertyNames )
            p->properties.push_back(getBetween(file, "<" + element + ">", "</" + element + ">", pos));

        w->tabStop = true;
        w->eventClick = [=] (int _x, int _y, char button, widget* me) { this->widget_Click(_x, _y, button, w); };
        w->eventMouseMove = [=] (int _x, int _y, char button, widget* me) { this->resizeWidget(_x, _y, button, w); };
        w->eventKeyPress = [=] (int keyCode, widget * me) { this->widget_KeyPress(keyCode, w); };
        w->data = p;
        widgets.push_back(w);

        if ( getBetween(file, "<", ">", pos) != "/" + p->type )
            cerr << "Process does not reached the end of the widget's properties or went over it.\n";
    }
}

void codeGenerator::loadButton_Click(int _x, int _y, char Button, widget* me)
{
    // Eddig meglévő elemek törlése
    while ( 16 < widgets.size() ) removeLastWidget();

    windowArea->visible = true;

    loadXML(projectName->getText());
}

void codeGenerator::exitButton_Click(int _x, int _y, char Button, widget* me)
{
    shutdown();
}

void codeGenerator::reGenerate(string projName)
{
    loadXML(projName);
    saveH();
    saveCPP();
}
/// -------------------------------------------------------------------------------------------------



/// --------------------------------- Konstruktor ----------------------------------------------------
codeGenerator::codeGenerator(bool show, bool fullScreen) : application()
{
    if ( !show ) return;
    int horizontal, vertical;
    GetDesktopResolution(horizontal, vertical);
    initApp(horizontal, vertical, fullScreen);
    eventHotKeys = [] (char keyCode, bool Control, bool Alt) { return false; };
}

void codeGenerator::init()
{
    eventClick = [=] (int _x, int _y, int button) { this->window_Click(_x, _y, button); };

    add( windowArea = new frame(0, 0, 10, 10) );
    windowArea->visible = false;
    windowArea->filled = true;
    windowArea->eventClick = [=] (int _x, int _y, int button, widget* me)
                    { this->windowArea_Click(_x, _y, button, windowArea); };
    windowArea->eventMouseMove = resizeWindow;
    windowArea->tabStop = false;

    add( controlBar = new frame(X - 250, 0, 250, Y) );
    controlBar->filled = true;
    controlBar->tabStop = false;

    add ( projectName = new textBox(X - 230, 30, 210, 30, "projectName") );
    projectName->allowedChar = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

    add( controls = new listBox(X - 230, 70, 210, 180, {"button",
                                                    "checkBox",
                                                        "frame",
                                                        "label",
                                                        "listBox",
                                                        "radioButton",
                                                        "horizontal scrollBar",
                                                        "vertical scrollBar",
                                                        "textBox" }) );

    add( groupList = new listBox(X - 230, 260, 210, 100, { "Root" }) );
    groupList->eventChanged = [=] (int index, widget* me)
            { this->groupList_SelectionChanged(index, groupList); };

    add( newGroupName = new textBox(X - 230, 370, 170, 30, "newGroup") );

    add( addGroup = new button(X - 50, 370, 30, 30, "+",
                               [=] (int _x, int _y, char Button, widget* me)
                                    { this->addGroup_Click(_x, _y, Button, addGroup); } ) );

    add( name = new textBox(X - 230, 430, 210, 30, "name") );
    name->eventKeyPress = [=] (int keyCode, widget* me)
                                    { this->name_KeyPress(keyCode, name); };

    add( x_koord = new textBox(X - 230, 470, 100, 30, "x") );
    x_koord->eventKeyPress = [=] (int keyCode, widget* me)
                                    { this->x_koord_KeyPress(keyCode, x_koord); };

    add( y_koord = new textBox(X - 120, 470, 100, 30, "y") );
    y_koord->eventKeyPress = [=] (int keyCode, widget* me)
                                    { this->y_koord_KeyPress(keyCode, y_koord); };

    add( width_koord = new textBox(X - 230, 510, 100, 30, "width") );
    width_koord->eventKeyPress = [=] (int keyCode, widget* me)
                                    { this->width_koord_KeyPress(keyCode, width_koord); };

    add( height_koord = new textBox(X - 120, 510, 100, 30, "height") );
    height_koord->eventKeyPress = [=] (int keyCode, widget* me)
                                    { this->height_koord_KeyPress(keyCode, height_koord); };

    add( propertiesList = new listBox(X - 230, 550, 210, 180, {}) );
    propertiesList->eventChanged = [=] (int index, widget* me)
            { this->propertiesList_SelectionChanged(index, propertiesList); };

    add( newProperty = new textBox(X - 230, 740, 210, 30, "property") );
    newProperty->eventKeyPress = [=] (int keyCode, widget* me)
                    { this->newProperty_KeyPress(keyCode, newProperty); };

    add ( saveButton = new button(X - 230, 800, 100, 30, "Save",
                                  [=] (int _x, int _y, char Button, widget* me)
                                    { this->saveButton_Click(_x, _y, Button, saveButton); } ) );

    add ( loadButton = new button(X - 120, 800, 100, 30, "Load",
                                  [=] (int _x, int _y, char Button, widget* me)
                                    { this->loadButton_Click(_x, _y, Button, loadButton); } ) );

    add ( exitButton = new button(X - 230, 840, 210, 30, "Exit",
                                  [=] (int _x, int _y, char Button, widget* me)
                                    { this->exitButton_Click(_x, _y, Button, exitButton); } ) );

    newGroup();

    timeTick = [this] () { this->Tick(); };

    selectedWidget = 0;
}






