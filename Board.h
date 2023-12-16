#pragma once

#include "Dependencies.h"
#include "MyUI.h"

//dependencies

// ========================================
// ======== Board Class Definition ========
// ========================================

class Board
{
private:
	//set up variables
	string filename;
	Color color;

	//variables
	int boardCharacters[COLUMNS][ROWS];//fliped because SFML is rotated 90
	bool functionalBoard;
	int numPellets;

	//score board
	MyScore* score;

	//ghosts
	//Ghost ghosts[4];

	//location variables
	//						  [  0        1      2     3      4      5       6       7       8   ]
	//				indexing: [Pacman, blinky, pinky, inky, clyde, Power1, Power2, Power3, Power4]
	Vector2i startingPositions[9];

	//methods
	bool LoadBoard(string fileName);
public:
	//variables

	//methods
	Board(string filename, Color color, MyScore* score);
	//void SetGhosts(Ghost ghosts[4]);
	bool ResetBoard();
	void DrawBoard(RenderWindow& window, bool colored);
	int RemovePellet(Vector2i tile);
	bool StillHasPellets();
	bool GetTile(int row, int col);
	bool GetFunctional();
	Vector2i GetStartingPosition(int index);
};
