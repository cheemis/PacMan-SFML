#pragma once

#pragma once

#include "Dependencies.h"

// ========================================
// ======== Board Class Definition ========
// ========================================

class Board
{
private:
	//variables
	int boardCharacters[COLUMNS][ROWS];//fliped because SFML is rotated 90
	bool functionalBoard;
	int numPellets;

	//methods
	bool LoadBoard(string fileName);
public:
	//variables

	//methods
	Board(string filename);
	void DrawBoard(RenderWindow& window);
	void RemovePellet(Vector2i tile);
	bool StillHasPellets();
	bool GetTile(int row, int col);
};
