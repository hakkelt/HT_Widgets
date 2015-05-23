#include "codeGenerator.h"
#include "sudoku.h"
#include <iostream>

using namespace std;

/// MODE = 1 >> Program dizájnolása a codeGenerator programmal
/// MODE = 2 >> Ha csak az xml-t változtattam, és szeretném újragenernálni a forrásfájlokat
/// MODE = bármi >> Új program futtatása
#define MODE 3
#define FULLSCREEN true // codeGenerator karbantartásakor nem teljes képernyõn futtatom

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

