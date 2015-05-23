#ifndef FOCUS_H_INCLUDED
#define FOCUS_H_INCLUDED

#include "widget.h"

/// Widgetek fókuszának kezelésére szolgáló rekord

struct focus
{
    static unsigned int tabIndex;
    static void next()  // Fókusz továbbadása
    {
        widgets[tabIndex]->focus = false;
        // Legközelebbi fókuszálható elem keresése
        while(!widgets[(++tabIndex %= widgets.size())]->tabStop) if (tabIndex > widgets.size() * 2) return;
        widgets[tabIndex]->focus = true;
    }

    static bool setFocus(int newFocus)  // Fókusz állítása adott elemre
    {
        if (!widgets[newFocus]->tabStop) return false; // Ha nem állítható az elemre fókusz
        widgets[tabIndex]->focus = false;
        widgets[newFocus]->focus = true;
        tabIndex = newFocus;
        return true;
    }
};

unsigned int focus::tabIndex;

#endif // FOCUS_H_INCLUDED
