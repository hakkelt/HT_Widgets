#ifndef GROUP_H_INCLUDED
#define GROUP_H_INCLUDED

#include "widget.h"
#include <vector>

/// --------------------------- Csom�pont, melyen keresz�l widgetek el�rhetik egym�st --------------------------------
class node
{
public:
    /// ---- Mez�k ----
    bool visible;
    std::vector<widget*> members;

    /// ---- Konstruktor, Destruktor ----
    node() {}
    virtual ~node() {}

    /// ---- B�v�t�s ----
    void add(widget *newMember);

    widget &operator[] (int index);
};

node& operator<< (node &G, widget *newMember);

#endif // GROUP_H_INCLUDED
