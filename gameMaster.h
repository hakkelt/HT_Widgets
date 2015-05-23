#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "label.h"
#include <vector>

namespace game
{

enum levels { easy, medium, hard };

class gameMaster
{
protected:
    const unsigned int N = 20;
    std::vector< std::vector<char> > solution;
    void shuffle(); // Új megoldást állít elő a meglévő keverésével
    void swapRows();
    void swapColumns();
    void swapBlockRows();
    void swapBlockColumns();
    void transposeMain();
    void transposeAnti();

public:
    // Ellenõrzi, hogy nincs-e ütkézés az újonnan beírt számnál
    bool check(std::vector< std::vector<label*> > &fields);

    // Új rejtvényt generál,  megjegyzi, és a paraméterként átadott label-mátrixba betölti
    void generateSudoku(std::vector< std::vector<label*> > &fields, levels level);

    gameMaster();
    virtual ~gameMaster() {}
};

}

#endif // GAMEMASTER_H
