#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#include "application.h"
#include "gameMaster.h"
#include <vector>

class sudoku : public application
{
protected:
	 ///---- Csomópontok ----
	node * groupRB;
	node * groupFields;

	/// ---- Események ----
	bool handleHotKeys (int keyCode, bool Control, bool Alt);

	// -- buttonNewGame --
	void buttonNewGame_Click (int _x, int _y, char button, widget* me);

	// -- buttonSolve --
	void buttonSolve_Click (int _x, int _y, char button, widget* me);

	// -- field --
	void field_KeyPress (int keyCode, widget* me);


public:
    /// ---- Játékszabályok ----
    gameMaster GM;

    /// ---- Widgetek ----
	frame * frameHeader;
	button * buttonNewGame;
	button * buttonSolve;
	radioButton * rbEasy;
	radioButton * rbMedium;
	radioButton * rbHard;
	std::vector< std::vector<label*> > fields;

	/// ---- Konstruktor, destruktor ----
	sudoku();
	virtual ~sudoku() {}

	/// ---- Mezõk feltöltése, tulajdonások beállítása ----
	virtual void init();
};

#endif // SUDOKU_H_INCLUDED
