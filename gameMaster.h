#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "label.h"
#include <vector>

class gameMaster
{
protected:

public:
    // Ellen�rzi, hogy nincs-e �tk�z�s az �jonnan be�rt sz�mn�l
    bool check(std::vector< std::vector<label*> > &fields);
    gameMaster();
    virtual ~gameMaster() {}
};

#endif // GAMEMASTER_H
