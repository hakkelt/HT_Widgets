#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "label.h"
#include <vector>

class gameMaster
{
protected:

public:
    // Ellenõrzi, hogy nincs-e ütkézés az újonnan beírt számnál
    bool check(std::vector< std::vector<label*> > &fields);
    gameMaster();
    virtual ~gameMaster() {}
};

#endif // GAMEMASTER_H
