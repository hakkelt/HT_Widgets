#include "sudoku.h"
#include <iostream>
#include <math.h>
#include "graphics.hpp"

using namespace std;
using namespace game;

/// ---------------------------------------- Események ----------------------------------------------------------
bool sudoku::handleHotKeys (int keyCode, bool Control, bool Alt)
{
    switch ( keyCode )
    {
    case genv::key_left :
        setFocus(((signed)tabIndex + 74) % 81 + 6);
        return true;
    case genv::key_right :
        setFocus(((signed)tabIndex + 76) % 81 + 6);
        return true;
    case genv::key_up :
        setFocus(((signed)tabIndex + 66) % 81 + 6);
        return true;
    case genv::key_down :
        setFocus(((signed)tabIndex + 84) % 81 + 6);
        return true;
    case genv::key_f1 :
        GM.solve(fields);
        return true;
    }
    return false;
}

// -- buttonNewGame --
void sudoku::buttonNewGame_Click (int _x, int _y, char button, widget* me)
{
    if ( rbEasy->getValue() ) GM.generateSudoku(fields, easy);
    else if ( rbMedium->getValue() ) GM.generateSudoku(fields, medium);
    else GM.generateSudoku(fields, hard);
    labelSuccess->setText("");
    time = 0;
}

// -- buttonSolve --
void sudoku::buttonSolve_Click (int _x, int _y, char button, widget* me)
{
    GM.show(fields);
}

// -- field --
void sudoku::field_KeyPress (int keyCode, widget* me)
{
    if ( ((label*)me)->bold ) return;
    if ( 49 <= keyCode && keyCode <= 58 )
        ((label*)me)->setText(convertS((char)keyCode));
    else if ( keyCode == genv::key_backspace || keyCode == genv::key_delete )
        ((label*)me)->setText("");
    else return;
    GM.check(fields);
    if ( GM.isFull(fields) ) labelSuccess->setText("Congratulations, you solved this sudoku!");
}

void sudoku::Tick()
{
    static int buffer = 0;
    if ( 25 < ++buffer )
    {
        time++;
        buffer = 0;
    }
    if ( labelSuccess->getText() != "Congratulations, you solved this sudoku!" )
        labelSuccess->setText(convertS(floor(time/3600)) + ":" + convertS(floor(time/60)) + ":" + convertS(time%60));
}

/// ------------------------------------------------------------------------------------------------------------



/// ---------------------------------------- Konstruktor --------------------------------------------------------
sudoku::sudoku() : application(450, 440)
{
	this->eventHotKeys = [=] (int keyCode, bool Control, bool Alt)
		{ return this->handleHotKeys(keyCode, Control, Alt); };
}
/// ------------------------------------------------------------------------------------------------------------



/// ---------------------------- Mezõk feltöltése, tulajdonások beállítása -------------------------------------
void sudoku::init()
{
    timeTick = [=] () { this->Tick(); };
    time = 0;

	 ///---- Csomópontok ----
	groupRB = newGroup();
	groupFields = newGroup();

	 ///---- Widgetek ----
	 // -- frameHeader --
	add( frameHeader = new frame(1, 1, 448, 40) );
	frameHeader->filled = true;
	frameHeader->style = high;

	 // -- buttonNewGame --
	add( buttonNewGame = new button(5, 5, 100, 30) );
	buttonNewGame->setText("New game");
	buttonNewGame->eventClick = [=] (int _x, int _y, char Button, widget* me)
				{ this->buttonNewGame_Click(_x, _y, Button, buttonNewGame); };
    buttonNewGame->tabStop = false;

	 // -- buttonSolve --
	add( buttonSolve = new button(115, 5, 100, 30) );
	buttonSolve->setText("Help!");
	buttonSolve->eventClick = [=] (int _x, int _y, char Button, widget* me)
				{ this->buttonSolve_Click(_x, _y, Button, buttonSolve); };
    buttonSolve->tabStop = false;

	 // -- rbEasy --
	add( rbEasy = new radioButton(230, 13, 80, 18, groupRB) );
	rbEasy->setText("Easy");
	rbEasy->setValue(true);
    rbEasy->tabStop = false;

	 // -- rbMedium --
	add( rbMedium = new radioButton(295, 13, 90, 18, groupRB) );
	rbMedium->setText("Medium");
    rbMedium->tabStop = false;

	 // -- rbHard --
	add( rbHard = new radioButton(375, 13, 75, 18, groupRB) );
	rbHard->setText("Hard");
	rbHard->tabStop = false;

    // -- field --
    for ( unsigned int i = 0; i < 9; i++ )
    {
        fields.push_back( {} );
        for ( unsigned int j = 0; j < 9; j++ )
        {
            label * temp;
            add( temp = new label(85 + j*31 + floor(j/3)*3, 90 + i*31 + floor(i/3)*3, 30, 30) );
            temp->backgroundColor = White;
            temp->background = true;
            temp->style = flat;
            temp->tabStop = true;
            temp->bold = true;
            temp->eventKeyPress = [=] (int keyCode, widget* me)
                        { this->field_KeyPress(keyCode, temp); };
            *groupFields << temp;

            fields[i].push_back(temp);
        }
    }
    GM.generateSudoku(fields, easy);

    // ---- labelSuccess ----
    add( labelSuccess = new label(15, 400, 420, 30) );
    labelSuccess->setText("");
    labelSuccess->align = align_center;

}
/// ------------------------------------------------------------------------------------------------------------



