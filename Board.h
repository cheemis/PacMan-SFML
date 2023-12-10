#pragma once





// ========================================
// ======== Board Class Definition ========
// ========================================

class Board
{
private:
	//variables
	bool boardCharacters[COLUMNS][ROWS];//fliped because SFML is rotated 90
	bool functionalBoard;

	//methods
	bool LoadBoard(string fileName);
public:
	//variables

	//methods
	Board(string filename);
	void DrawBoard(RenderWindow& window);
	bool GetCell(int row, int col);
};


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
			//if there's a wall at this location
			if (outputString[col] == 'x')
			{
				boardCharacters[col][currentRow] = false;//fliped because SFML is rotated 90
			}
			//else, there's a free space at this location
			else
			{
				boardCharacters[col][currentRow] = true;//fliped because SFML is rotated 90
			}
		}
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
	functionalBoard = LoadBoard(filename);
}

void Board::DrawBoard(RenderWindow& window)
{
	if (!functionalBoard) return;
	
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			if (!boardCharacters[col][row]) //fliped because SFML is rotated 90
			{
				RectangleShape tile(Vector2(TILE_SIZE, TILE_SIZE));
				tile.setFillColor(WALL_COLOR);
				tile.setPosition(col * TILE_OFFSET, row * TILE_OFFSET);//fliped because SFML is rotated 90
				window.draw(tile);
			}
		}
	}
}

bool Board::GetCell(int row, int col)
{
	return boardCharacters[row][col];
}