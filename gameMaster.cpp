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
    // Egy m�k�d� megold�s k�sz�t�se
    for ( unsigned int i = 0; i < 9; i++ )
    {
        solution.push_back( {} );
        for ( unsigned int j = 0; j < 9; j++ )
            solution[i].push_back((i*3 + j + (int)floor(i/3)) % 9 + 1);
    }
}

bool gameMaster::check(std::vector< std::vector<label*> > &fields)
{
    bool good = true;
    for ( unsigned int i = 0; i < 9; i++) // Az �sszes elemet megvizsg�lom soronk�nt
    {
        for (unsigned int j = 0; j < 9 ; j++) // Az �sszes elemet megvizsg�lom oszloponk�nt
        {
            int value1 = convertI(fields[i][j]->getText());

            for ( unsigned int k = 0; k < 9; k++) // V�gigmegyek az elem sor�n
            {
                if ( k == j ) continue;
                int value2 = convertI(fields[i][k]->getText());
                if ( value1 == value2 )
                {
                    fields[i][j]->fontColor = Red;
                    good = false;
                }
            }
            for ( unsigned int k = 0; k < 9; k++) // V�gigmegyek az elem oszlop�n
            {
                if ( k == i ) continue;
                int value2 = convertI(fields[k][j]->getText());
                if ( value1 == value2 )
                {
                    fields[i][j]->fontColor = Red;
                    good = false;
                }
            }
            for ( unsigned int k = floor(j/3)*3; k < floor(j/3)*3 + 3; k++) // V�gigmegyek az elem cell�j�n
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
    return good;
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

char gameMaster::good(int i, int j) // megn�zi, hogy (i, j) helyre ker�lhet-e t�bb sz�m
{
    set<char> exclude; // Ebbe gy�jt�m azokat a sz�mokat, amik nem ker�lhetnek az (i, j) helyre

    for ( int k = 0; k < 9; k++ ) // V�gigmegy�nk az oszlopon
    {
        if ( k == i ) continue;
        exclude.insert(exclude.begin(), puzzle[k][j]);
    }
    for ( int k = 0; k < 9; k++ ) // V�gigmegy�nk a sorokon
    {
        if ( k == j ) continue;
        exclude.insert(exclude.begin(), puzzle[i][k]);
    }
    for ( int k = floor(j/3)*3; k < floor(j/3)*3 + 3; k++) // V�gigmegyek az elem cell�j�n
    {
        if ( k == j ) continue;
        for ( int l = floor(i/3)*3; l < floor(i/3)*3 + 3; l++)
        {
            if ( l == i ) continue;
            exclude.insert(exclude.begin(), puzzle[k][l]);
        }
    }

    if ( exclude.size() - exclude.count(0) == 8 )
        for ( unsigned char k = 1; k <= 9; k++ )
            if ( !exclude.count(k) ) return k;

    return 0;
}

void gameMaster::eraseCells(levels level)
{
    for ( unsigned int i = 0; i < 9; i++)
        for ( unsigned int j = 0; j < 9; j++)
            if ( good(i,j) != 0 )
                puzzle[i][j] = 0;
}

void gameMaster::generateSudoku(std::vector< std::vector<label*> > &fields, levels level)
{
    shuffle();

    puzzle = solution;

    eraseCells(level);
    eraseCells(level);

    // Gener�lt rejtv�ny bet�lt�se a mez�kbe
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








