#include "gameMaster.h"
#include "application.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <set>

using namespace std;
using namespace game;

gameMaster::gameMaster()
{
    // Egy mûködõ megoldás készítése
    for ( unsigned int i = 0; i < 9; i++ )
    {
        solution.push_back( {} );
        for ( unsigned int j = 0; j < 9; j++ )
            solution[i].push_back((i*3 + j + (int)floor(i/3)) % 9 + 1);
    }
}

bool gameMaster::isFull(vector< vector<label*> > &fields)
{
    for ( unsigned int i = 0; i < 9; i++ )
        for ( unsigned int j = 0; j < 9; j++ )
            if ( puzzle[i][j] == 0 ) return false;

    for ( unsigned int i = 0; i < 9; i++)
        for (unsigned int j = 0; j < 9 ; j++)
            if ( fields[i][j]->bold ) continue;
            else if ( solution[i][j] == convertI(fields[i][j]->getText()) ) fields[i][j]->fontColor = Green;
            else fields[i][j]->fontColor = Red;

    return true;
}

void gameMaster::check(vector< vector<label*> > &fields)
{
    for ( unsigned int i = 0; i < 9; i++) // Az összes elemet megvizsgálom soronként
    {
        for (unsigned int j = 0; j < 9 ; j++) // Az összes elemet megvizsgálom oszloponként
        {
            bool good = true;
            int value1 = convertI(fields[i][j]->getText());

            for ( unsigned int k = 0; k < 9; k++) // Végigmegyek az elem során
            {
                if ( k == j ) continue;
                int value2 = convertI(fields[i][k]->getText());
                if ( value1 == value2 )
                {
                    fields[i][j]->fontColor = Red;
                    good = false;
                }
            }
            for ( unsigned int k = 0; k < 9; k++) // Végigmegyek az elem oszlopán
            {
                if ( k == i ) continue;
                int value2 = convertI(fields[k][j]->getText());
                if ( value1 == value2 )
                {
                    fields[i][j]->fontColor = Red;
                    good = false;
                }
            }
            for ( unsigned int k = floor(j/3)*3; k < floor(j/3)*3 + 3; k++) // Végigmegyek az elem celláján
            {
                if ( k == j ) continue;
                for ( unsigned int l = floor(i/3)*3; l < floor(i/3)*3 + 3; l++)
                {
                    if ( l == i ) continue;
                    int value2 = convertI(fields[l][k]->getText());
                    if ( value1 == value2 )
                    {
                        fields[i][j]->fontColor = Red;
                        good = false;
                    }
                }
            }

            if ( good && fields[i][j]->bold ) fields[i][j]->fontColor = Black;
            else if ( good ) fields[i][j]->fontColor = Blue;
        }
    }
}

void gameMaster::swapRows()
{
    int block = rand()%3 * 3;
    int row1 = block + rand()%3;
    int row2 = block + rand()%3;
    solution[row1].swap(solution[row2]);
}
void gameMaster::swapColumns()
{
    int block = rand()%3 * 3;
    int col1 = block + rand()%3;
    int col2 = block + rand()%3;
    for ( unsigned int i = 0; i < 9; i++ )
        swap(solution[i][col1],solution[i][col2]);
}
void gameMaster::swapBlockRows()
{
    int block1 = rand()%3 * 3;
    int block2 = rand()%3 * 3;
    solution[block1].swap(solution[block2]);
    solution[block1 + 1].swap(solution[block2 + 1]);
    solution[block1 + 2].swap(solution[block2 + 2]);
}
void gameMaster::swapBlockColumns()
{
    int block1 = rand()%3 * 3;
    int block2 = rand()%3 * 3;
    for ( unsigned int i = 0; i < 9; i++ )
    {
        swap(solution[i][block1],solution[i][block2]);
        swap(solution[i][block1 + 1],solution[i][block2 + 1]);
        swap(solution[i][block1 + 2],solution[i][block2 + 2]);
    }
}
void gameMaster::transposeMain()
{
    for ( unsigned int i = 0; i < 9; i++ )
        for ( unsigned int j = 0; j < i; j++ )
            swap(solution[i][j],solution[j][i]);
}
void gameMaster::transposeAnti()
{
    for ( unsigned int i = 0; i < 9; i++ )
        for ( unsigned int j = 0; j < 9 - i; j++ )
            swap(solution[i][j],solution[8 - j][8 - i]);
}

void gameMaster::shuffle()
{
    /* initialize random seed: */
    srand (time(NULL));

    for ( unsigned int i = 0; i < N; i++ )
    {
        int r = rand()%6;
        switch ( r )
        {
        case 0 :
            swapRows(); break;
        case 1 :
            swapColumns(); break;
        case 2 :
            swapBlockRows(); break;
        case 3 :
            swapBlockColumns(); break;
        case 4:
            transposeMain(); break;
        case 5 :
            transposeAnti(); break;
        }
    }
}

char gameMaster::good(int i, int j) // megnézi, hogy (i, j) helyre kerülhet-e több szám
{
    set<char> exclude; // Ebbe gyûjtöm azokat a számokat, amik nem kerülhetnek az (i, j) helyre

    for ( int k = 0; k < 9; k++ ) // Végigmegyünk az oszlopon
    {
        if ( k == i ) continue;
        exclude.insert(exclude.begin(), puzzle[k][j]);
    }
    for ( int k = 0; k < 9; k++ ) // Végigmegyünk a sorokon
    {
        if ( k == j ) continue;
        exclude.insert(exclude.begin(), puzzle[i][k]);
    }
    for ( int k = floor(j/3)*3; k < floor(j/3)*3 + 3; k++) // Végigmegyek az elem celláján
    {
        if ( k == j ) continue;
        for ( int l = floor(i/3)*3; l < floor(i/3)*3 + 3; l++)
        {
            if ( l == i ) continue;
            exclude.insert(exclude.begin(), puzzle[l][k]);
        }
    }

    if ( exclude.size() - exclude.count(0) == 8 )
        for ( unsigned char k = 1; k <= 9; k++ )
            if ( !exclude.count(k) ) return k;

    return 0;
}

void gameMaster::eraseCells(levels level)
{
    switch ( level )
    {
    case easy :
        {int erased = 0;
        int limit = 0;
        while ( erased < 31 || 1000 < limit++ )
        {
            int i = rand()%9;
            int j = rand()%9;
            if ( puzzle[i][j] && good(i,j) != 0 )
            {
                puzzle[i][j] = 0;
                erased++;
            }
        }}
        break;
    case medium :
        {int erased = 0;
        int limit = 0;
        while ( erased < 41 || 1000 < limit++ )
        {
            int i = rand()%9;
            for ( unsigned int j = 0; j < 9; j++ )
            {
                if ( puzzle[i][j] && good(i,j) != 0 )
                {
                    puzzle[i][j] = 0;
                    erased++;
                }
            }
        }}
        break;
    case hard :
        for ( unsigned int i = 0; i < 9; i++)
            for ( unsigned int j = 0; j < 9; j++)
                if ( good(i,j) != 0 )
                    puzzle[i][j] = 0;
        break;
    }

}

void gameMaster::countErased()
{
    int erased = 0;
    for ( unsigned int i = 0; i < 9; i++)
        for ( unsigned int j = 0; j < 9; j++)
            if ( puzzle[i][j] == 0 ) erased++;
    cout << "Remainders: " << 81 - erased << endl;
}

void gameMaster::solve(vector< vector<label*> > &fields)
{
    bool change = true;
    int limit = 0;
    while ( change && limit++ < 100 )
    {
        change = false;
        for ( unsigned int i = 0; i < 9 ; i ++ )
            for ( unsigned int j = 0; j < 9 ; j ++ )
            {
                if ( puzzle[i][j] != 0 ) continue;
                char temp = good(i,j);
                if ( temp != 0 )
                {
                    puzzle[i][j] = temp;
                    change = true;
                }
            }
    }

    for ( unsigned int i = 0; i < 9; i++)
        for (unsigned int j = 0; j < 9 ; j++)
            if ( puzzle[i][j] != 0 )
                fields[i][j]->setText(convertS((int)puzzle[i][j]));
}

void gameMaster::show(vector< vector<label*> > &fields)
{
    for ( unsigned int i = 0; i < 9; i++)
        for (unsigned int j = 0; j < 9 ; j++)
            if ( fields[i][j]->bold ) continue;
            else if ( solution[i][j] == convertI(fields[i][j]->getText()) ) fields[i][j]->fontColor = Green;
            else fields[i][j]->fontColor = Red;

    if ( isFull(fields) ) return;
    int i = rand()%9;
    int j = rand()%9;
    while ( fields[i][j]->getText() != "" )
    {
        i = rand()%9;
        j = rand()%9;
    }
    fields[i][j]->setText(convertS((int)solution[i][j]));
}

void gameMaster::generateSudoku(vector< vector<label*> > &fields, levels level)
{
    shuffle();

    puzzle = solution;

    eraseCells(level);

    // Generált rejtvény betöltése a mezõkbe
    for ( unsigned int i = 0; i < 9; i++)
        for (unsigned int j = 0; j < 9 ; j++)
        {
            if ( puzzle[i][j] == 0 )
            {
                fields[i][j]->setText("");
                fields[i][j]->bold = false;
            }
            else
            {
                fields[i][j]->setText(convertS((int)puzzle[i][j]));
                fields[i][j]->bold = true;
            }
        }

    check(fields);
}








