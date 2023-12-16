#include "Board.h"

// ========================================
// ======== Private Board Functions =======
// ========================================

bool Board::LoadBoard(string fileName)
{
	ifstream boardFile(fileName);
	string outputString = "";

	bool pacman = false,
		blinky = false,
		pinky = false,
		inky = false,
		clyde = false;

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
			case 'o': //pellet
				cout << 'o';
				boardCharacters[col][currentRow] = 3;
				numPower++;
				break;

			case '-': //barrier
				cout << '-';
				boardCharacters[col][currentRow] = 4;
				break;

			case 'P': //Pacman
				cout << 'P';
				if (pacman) return false;
				pacman = true;
				startingPositions[0] = Vector2i(col, currentRow);
				boardCharacters[col][currentRow] = 1;
				break;
			case 'b': //Blinky
				cout << 'b';
				if (blinky) return false;
				blinky = true;
				startingPositions[1] = Vector2i(col, currentRow);
				boardCharacters[col][currentRow] = 1;
				break;
			case 'p': //Pinky
				cout << 'p';
				if (pinky) return false;
				pinky = true;
				startingPositions[2] = Vector2i(col, currentRow);
				boardCharacters[col][currentRow] = 1;
				break;
			case 'i': //Inky
				cout << 'i';
				if (inky) return false;
				inky = true;
				startingPositions[3] = Vector2i(col, currentRow);
				boardCharacters[col][currentRow] = 1;
				break;
			case 'c': //Clyde
				cout << 'c';
				if (clyde) return false;
				clyde = true;
				startingPositions[4] = Vector2i(col, currentRow);
				boardCharacters[col][currentRow] = 1;
				break;
			}
		}
		cout << endl;
		currentRow++;
	}
	cout << filename << ":" << endl;
	cout << "num pellets: " << to_string(numPellets) << endl;
	cout << "num power: " << to_string(numPower) << endl << endl;
	boardFile.close();

	return pacman && blinky && pinky && inky && clyde && 
			numPellets == NUM_PELLETS && numPower == NUM_POWER;
}


// ========================================
// ======== Public Board Functions ========
// ========================================

Board::Board(string filename, Color color, MyScore* score)
{
	this->filename = filename;
	this->color = color;

	numPellets = 0;
	numPower = 0;
	functionalBoard = LoadBoard(filename);

	this->score = score;
}

//void Board::SetGhosts(Ghost ghosts[4])
//{
//	ghosts = ghosts;
//}

bool Board::ResetBoard()
{
	numPellets = 0;
	numPower = 0;
	return LoadBoard(filename);
}

void Board::DrawBoard(RenderWindow& window, bool colored)
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
				tile.setSize(Vector2f(TILE_SIZE, TILE_SIZE));
				tile.setPosition(col * TILE_OFFSET, row * TILE_OFFSET);
				tile.setFillColor(colored ? color : Color::White);
				window.draw(tile);
				break;

			default: //empty space
				break;

			case 2: //pellet
				tile.setSize(Vector2f(TILE_SIZE / 4, TILE_SIZE / 4));
				tile.setPosition(col * TILE_OFFSET + (TILE_SIZE / 2),
					row * TILE_OFFSET + (TILE_SIZE / 2));
				tile.setFillColor(PELLET_COLOR);
				window.draw(tile);
				break;

			case 3: //power pellet
				tile.setSize(Vector2f(TILE_SIZE / 2, TILE_SIZE / 2));
				tile.setPosition(col * TILE_OFFSET + (TILE_SIZE / 4),
					row * TILE_OFFSET + (TILE_SIZE / 4));
				tile.setFillColor(POWER_COLOR);
				window.draw(tile);
				break;

			case 4: //barrier
				tile.setSize(Vector2f(TILE_SIZE, TILE_SIZE));
				tile.setPosition(col * TILE_OFFSET, row * TILE_OFFSET);
				tile.setFillColor(Color::Red);
				window.draw(tile);
				break;
			}
		}
	}
}


int Board::RemovePellet(Vector2i tile)
{
	int currentTile = boardCharacters[tile.x][tile.y];

	switch (currentTile)
	{
	case 2: //regular pellet
		score->IncreaseScore(PELLET_POINTS);
		numPellets--;
		boardCharacters[tile.x][tile.y] = 1;
		break;
	case 3: //power pellet
		score->IncreaseScore(POWER_POINTS);
		numPower--;
		boardCharacters[tile.x][tile.y] = 1;
		break;
	}
	return currentTile;
}

bool Board::StillHasPellets()
{
	bool status = numPellets + numPower > 0;
	if (!status) score->IncreaseScore(COMPLETE_POINTS);
	return status;
}

int Board::GetTile(int row, int col)
{
	row = clamp(row, 0, COLUMNS); //flipped because board is rotated 90
	col = clamp(col, 0, ROWS);

	return boardCharacters[row][col];
}

bool Board::GetFunctional()
{
	return functionalBoard;
}

Vector2i Board::GetStartingPosition(int index)
{
	return startingPositions[index];
}