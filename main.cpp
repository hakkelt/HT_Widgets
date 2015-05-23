#include "codeGenerator.h"
#include "sudoku.h"
#include <iostream>

using namespace std;

/// MODE = 1 >> Program diz�jnol�sa a codeGenerator programmal
/// MODE = 2 >> Ha csak az xml-t v�ltoztattam, �s szeretn�m �jragenern�lni a forr�sf�jlokat
/// MODE = b�rmi >> �j program futtat�sa
#define MODE 3
#define FULLSCREEN true // codeGenerator karbantart�sakor nem teljes k�perny�n futtatom

/// ---- Main ----
int main ()
{
    #if MODE == 1
        codeGenerator App(true, FULLSCREEN);
        App.init();
        App.run();
    #elif MODE == 2
        codeGenerator App(false);
        App.init();
        cout << "Name of project: ";
        string projName;
        cin >> projName;
        App.reGenerate(projName);
        cout << "Regeneration was successful!\n";
    #else
        sudoku App;
        App.init();
        App.run();
    #endif // MODE

    return 0;
}

