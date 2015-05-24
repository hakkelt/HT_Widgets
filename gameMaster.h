#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include "label.h"
#include <vector>

namespace game
{

enum levels { easy = 0, medium, hard };

class gameMaster
{
protected:
    const unsigned int N = 20;
    std::vector< std::vector<char> > solution;
    std::vector< std::vector<char> > puzzle;

    void shuffle(); // Új megoldást állít elő a meglévő keverésével
    void swapRows();
    void swapColumns();
    void swapBlockRows();
    void swapBlockColumns();
    void transposeMain();
    void transposeAnti();

    void eraseCells(levels level);
    char good(int i, int j);
    void countErased();

public:
    // Ellenõrzi, hogy nincs-e ütkézés az újonnan beírt számnál
    void check(std::vector< std::vector<label*> > &fields);

    // Új rejtvényt generál,  megjegyzi, és a paraméterként átadott label-mátrixba betölti
    void generateSudoku(std::vector< std::vector<label*> > &fields, levels level);

    // Leellenőrzi, hogy jó-e a megoldás
    void show(std::vector< std::vector<label*> > &fields);
    // Megoldja a sudokut
    void solve(std::vector< std::vector<label*> > &fields);
    // Ki van-e töltve a rejtvény
    bool isFull(std::vector< std::vector<label*> > &fields);

    gameMaster();
    virtual ~gameMaster() {}
};

}

#endif // GAMEMASTER_H
