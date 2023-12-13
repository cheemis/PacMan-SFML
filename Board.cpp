#include "Board.h"



// ========================================
// ======== Private Board Functions =======
// ========================================

bool Board::LoadBoard(string fileName)
{
	ifstream boardFile(fileName);
	string outputString = "";

	//make sure doesnt load out of bounds
	int currentRow = 0;

	if (!boardFile.is_open())
	{
		cout << "filename was not found: " << fileName << endl;
		return false;
	}

	// Use a while loop together with the getline() function to read the file line by line
	while (getline(boardFile, outputString))
	{
		//too many rows - bad board file
		if (currentRow >= ROWS)
		{
			cout << "current row was greater than max rows: " << currentRow << " > " << ROWS
				<< endl;
			return false;
		}
		//row size isn't the same - bad board file
		if (outputString.length() != COLUMNS)
		{
			cout << "columns not the proper length: " << outputString.length() << " != " << COLUMNS << " (" << outputString << ")" << endl;
			return false;
		}

		//if proper file, fill out board
		for (int col = 0; col < COLUMNS; col++)
		{
			char c = outputString[col];
			switch (c)
			{
			case 'x': //wall
				cout << 'x';
				boardCharacters[col][currentRow] = 0;//fliped because SFML is rotated 90
				break;

			default: //empty space
				cout << ' ';
				boardCharacters[col][currentRow] = 1;
				break;

			case '.': //pellet
				cout << '.';
				boardCharacters[col][currentRow] = 2;
				numPellets++;
				break;
			}
		}
		cout << endl;
		currentRow++;
	}
	boardFile.close();

	cout << "loaded file!" << endl;

	return true;
}


// ========================================
// ======== Public Board Functions ========
// ========================================

Board::Board(string filename)
{
	numPellets = 0;
	functionalBoard = LoadBoard(filename);
}

void Board::DrawBoard(RenderWindow& window)
{
	if (!functionalBoard) return;

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			int cell = boardCharacters[col][row];
			RectangleShape tile;

			switch (cell)
			{
			case 0: //wall
				//cout << 'x';
				tile.setSize(Vector2f(TILE_SIZE, TILE_SIZE));
				tile.setPosition(col * TILE_OFFSET, row * TILE_OFFSET);
				tile.setFillColor(WALL_COLOR);
				window.draw(tile);
				break;

			default: //empty space
				break;

			case 2: //pellet
				//cout << '.';
				tile.setSize(Vector2f(TILE_SIZE / 4, TILE_SIZE / 4));
				tile.setPosition(col * TILE_OFFSET + (TILE_SIZE / 2),
					row * TILE_OFFSET + (TILE_SIZE / 2));
				tile.setFillColor(PELLET_COLOR);
				window.draw(tile);
				break;
			}
		}
	}
}

void Board::RemovePellet(Vector2i tile)
{
	if (boardCharacters[tile.x][tile.y] == 2)
	{
		numPellets--;
		boardCharacters[tile.x][tile.y] = 1;
	}
}

bool Board::StillHasPellets()
{
	return numPellets > 0;
}

bool Board::GetTile(int row, int col)
{
	return boardCharacters[row][col];
}