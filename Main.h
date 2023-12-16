#pragma once

#include "Dependencies.h"
#include "MyUI.h"
#include "Entity.h"
#include "Board.h"



void StartScreen(RenderWindow& window, int& gameState, MyUI& title, MyUI& subTitle)
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		gameState = 5;

	}
	else
	{
		window.draw(title.GetText());
		window.draw(subTitle.GetText());
	}
}

bool StartGame(Board* currentBoard, Pacman* pacman, Ghost ghosts[])
{
	if (!currentBoard->ResetBoard())
	{
		return false;
	}
	pacman->ResetEntity(currentBoard->GetStartingPosition(0), currentBoard);
	for (int i = 0; i < 4; i++)
	{
		ghosts[i].ResetEntity(currentBoard->GetStartingPosition(i + 1), currentBoard);
	}
	return true;
}


// Return values:
// 0: nothing significant happened
// 1: pacman died
// 2: the board ran out of pellets
// 
int PlayGame(RenderWindow& window, Board* currentBoard, Pacman* pacman, Ghost ghosts[], float deltaTime)
{
	//check if pacman died
	if (!pacman->GetIsAlive())
	{
		return 1;
	}

	//draw the board
	currentBoard->DrawBoard(window, true);

	//update entities
	pacman->Update(deltaTime);
	for (int i = 0; i < 4; i++) ghosts[i].Update(deltaTime);

	//draw entitiesa
	for (int i = 0; i < 4; i++) window.draw(ghosts[i].GetShape());
	window.draw(pacman->GetShape());

	if (!currentBoard->StillHasPellets()) return 2; //temp
	return 0;
}

bool GameOutcome(int& gameState, int gameStatus, float& timing,
				int& currentBoard, Board boards[],
				Pacman* pacman, Ghost ghosts[])
{
	switch (gameStatus)
	{
	case 0: //nothing significant happened
		break;
	case 1: //pacman dies
		timing = DEATH_TIME;
		gameState = 3;
		break;
	case 2: // all pellets collected
		timing = LOADING_TIME;
		gameState = 4;
		break;
	default:
		//something broke
		return false;
		break;
	}
	return true;
}

bool SetLives(int newLives, int& lives, MyUI* livesText)
{
	lives = newLives;
	livesText->SetText("Lives:\n" + to_string(lives));
	return lives; //see if the player is out of lives
}

void Dying(RenderWindow& window, Board* board, int& gameState, int& lives, MyUI* livesText, Pacman* pacman, Ghost ghosts[], float& timeToDeath, float deltaTime)
{
	//update management
	timeToDeath -= deltaTime;

	//update pacman
	pacman->Dying(deltaTime);

	//update entities
	window.draw(pacman->GetShape());

	if (timeToDeath <= 0)
	{
		//check if ran out of lives
		if (!SetLives(lives - 1, lives, livesText))
		{
			gameState = 6;
			timeToDeath = READY_TIME;
		}
		else
		{
			//reset entities
			pacman->ResetEntity(board->GetStartingPosition(0), board);
			for (int i = 0; i < 4; i++)
			{
				ghosts[i].ResetEntity(board->GetStartingPosition(i + 1), board);
			}
			timeToDeath = LOADING_TIME; //set up for blinking
			gameState = 5;
		}
	}
}

bool Blinking(RenderWindow& window, Board boards[], int& currentBoard, int& gameState, Pacman* pacman, Ghost ghosts[], float& timeToChange, float deltaTime)
{
	//decriment time
	timeToChange -= deltaTime;

	//draw background
	if (((int)(timeToChange*2) % 2)) //every other second, change color
	{
		(&boards[currentBoard])->DrawBoard(window, false);
	}
	else
	{
		(&boards[currentBoard])->DrawBoard(window, true);
	}
	

	//draw entities
	window.draw(pacman->GetShape());
	for (int i = 0; i < 4; i++) window.draw(ghosts[i].GetShape());

	if (timeToChange <= 0)
	{
		currentBoard = (currentBoard + 1) % 3;
		if (!StartGame(&boards[currentBoard], pacman, ghosts)) return false; //break if one of the boards has been modified
		gameState = 5;
		timeToChange = READY_TIME;
	}
	return true; //nothing eventful
}



void ReadyScreen(RenderWindow* window, Board* board, MyUI* centerText)
{
	board->DrawBoard(*window, true);
	window->draw(centerText->GetText());
}


