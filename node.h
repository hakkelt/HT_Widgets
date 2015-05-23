#ifndef GROUP_H_INCLUDED
#define GROUP_H_INCLUDED

#include "widget.h"
#include <vector>

/// --------------------------- Csomópont, melyen kereszül widgetek elérhetik egymást --------------------------------
class node
{
public:
    /// ---- Mezõk ----
    bool visible;
    std::vector<widget*> members;

    /// ---- Konstruktor, Destruktor ----
    node() {}
    virtual ~node() {}

    /// ---- Bõvítés ----
    void add(widget *newMember);

    widget &operator[] (int index);
};

node& operator<< (node &G, widget *newMember);

#endif // GROUP_H_INCLUDED
