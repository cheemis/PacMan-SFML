#include "Main.h"

void StartScreen(RenderWindow& window, int& gameState, MyUI& title, MyUI& subTitle)
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		gameState = 2;

	}
	else
	{
		window.draw(title.GetText());
		window.draw(subTitle.GetText());
	}
}

bool StartGame(Board *currentBoard, Pacman *pacman, Ghost ghosts[])
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
	currentBoard->DrawBoard(window);

	//update entities
	pacman->Update(deltaTime);
	for (int i = 0; i < 4; i++) ghosts[i].Update(deltaTime);

	//draw entities
	window.draw(pacman->GetShape());
	for (int i = 0; i < 4; i++) window.draw(ghosts[i].GetShape());

	if (!currentBoard->StillHasPellets()) return 2; //temp
	return 0;
}

bool GameOutcome(int &gameState, int gameStatus, int &currentBoard,
				 float &timeToDeath,
				 Board boards[],
				 Pacman *pacman, Ghost ghosts[])
{
	switch (gameStatus)
	{
	case 0: //nothing significant happened
		break;
	case 1: //pacman dies
		timeToDeath = DEATH_TIME;
		gameState = 3;
		break;
	case 2: // all pellets collected
		currentBoard = (currentBoard + 1) % 3;
		if (!StartGame(&boards[currentBoard], pacman, ghosts)) return false; //break if one of the boards has been modified
		break;
	default:
		//something broke
		return false;
		break;
	}
	return true;
}

bool Dying(RenderWindow& window, Board* currentBoard, Pacman* pacman, Ghost ghosts[], float& timeToDeath,float deltaTime)
{
	//update management
	timeToDeath -= deltaTime;

	//update pacman
	pacman->Dying(deltaTime);

	//update entities
	window.draw(pacman->GetShape());

	if (timeToDeath <= 0)
	{
		//reset entities
		pacman->ResetEntity(currentBoard->GetStartingPosition(0), currentBoard);
		for (int i = 0; i < 4; i++)
		{
			ghosts[i].ResetEntity(currentBoard->GetStartingPosition(i + 1), currentBoard);
		}
		return true;
	}
	return false;
}

void GameOver()
{

}

int main()
{
	// ============ Window Variables =========== //

	RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Pacman!");


	// ============= Game Entities ============= //

	Board boards[] = { Board("maze0.txt", WALL_COLOR1),
					   Board("maze1.txt", WALL_COLOR2),
					   Board("maze2.txt", WALL_COLOR3)};
	int currentBoard = -1;

	Pacman pacman = Pacman(500,Color::Yellow, boards[0].GetStartingPosition(0), &boards[0]);

	Ghost ghosts[] = { Ghost(100, 0, Color::Red, boards[0].GetStartingPosition(1), &boards[0], &pacman),
					   Ghost(100, 1,Color::Cyan, boards[0].GetStartingPosition(2), &boards[0], &pacman),
					   Ghost(100, 2, Color::Magenta, boards[0].GetStartingPosition(3), &boards[0], &pacman),
					   Ghost(100, 3, Color(255, 184,71), boards[0].GetStartingPosition(4), &boards[0], &pacman)};


	// ============== UI Variables ============= //

	MyUI titleText("Pacman",
					FONT_TITLE_SIZE,
					Color::Yellow,
					Vector2f(WINDOW_SIZE / 2, WINDOW_SIZE / 2.5));

	MyUI subTitle("Click Space To Play!", FONT_REGULAR_SIZE, Color::White, Vector2f(WINDOW_SIZE / 2, WINDOW_SIZE / 2));



	// =========== Managing Variables ========== //

	int gameState = 1;
	float timeToDeath = 0;



	// =========== Initializing work =========== //

	//check to see if all boards valid
	for (int i = 0; i < 3; i++) if (!boards[i].GetFunctional()) return 0;

	//===== set time =====//
	Clock deltaSrc;
	float previousTime = deltaSrc.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		//find delta time
		Time time = deltaSrc.getElapsedTime();
		float currentTime = time.asSeconds();
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();

		switch (gameState)
		{
			int gameStatus;
			bool gameWorking;

		case 1: //title screen
			StartScreen(window, gameState, titleText, subTitle);
			if (gameState == 2) //start the next round
			{
				currentBoard = (currentBoard + 1) % 3;
				if (!StartGame(&boards[currentBoard], &pacman, ghosts)) return 0; //break if one of the boards has been modified
			}
			break;
		case 2: //play the game
			gameStatus = PlayGame(window, &boards[currentBoard], &pacman, ghosts, deltaTime);
			gameWorking = GameOutcome(gameState, gameStatus, currentBoard, timeToDeath, boards, &pacman, ghosts);
			if (!gameWorking) return 0;
			break;
		case 3: //dying
			if (Dying(window, &boards[currentBoard], &pacman, ghosts, timeToDeath, deltaTime))
			{
				gameState = 2;
			}
			break;

		}


		
		/*

		*/

		window.display();
	}
	return 0;
}


