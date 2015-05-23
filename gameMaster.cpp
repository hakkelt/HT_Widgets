#include "gameMaster.h"
#include "application.h"
#include <math.h>
#include <iostream>

using namespace std;

gameMaster::gameMaster()
{

}

bool gameMaster::check(std::vector< std::vector<label*> > &fields)
{
    for ( unsigned int i = 0; i < 9; i++) // Az �sszes elemet megvizsg�lom soronk�nt
    {
        for (unsigned int j = 0; j < 9 ; j++) // Az �sszes elemet megvizsg�lom oszloponk�nt
        {
            bool good = true;
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
}

