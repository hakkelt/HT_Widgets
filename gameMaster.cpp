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

