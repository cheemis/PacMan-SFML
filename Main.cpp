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

	Pacman pacman = Pacman(500, (TILE_SIZE * .5f),Color::Yellow, boards[0].GetStartingPosition(0), &boards[0]);

	Ghost ghosts[] = { Ghost(100, (TILE_SIZE * .5f), 0, Color::Red, boards[0].GetStartingPosition(1), &boards[0], &pacman),
					   Ghost(100, (TILE_SIZE * .5f), 1,Color::Cyan, boards[0].GetStartingPosition(2), &boards[0], &pacman),
					   Ghost(100, (TILE_SIZE * .5f), 2, Color::Magenta, boards[0].GetStartingPosition(3), &boards[0], &pacman),
					   Ghost(100, (TILE_SIZE * .5f), 3, Color(255, 184,71), boards[0].GetStartingPosition(4), &boards[0], &pacman)};


	// ============== UI Variables ============= //

	MyUI titleText("Pacman",
					FONT_TITLE_SIZE,
					Color::Yellow,
					Vector2f(WINDOW_SIZE / 2, WINDOW_SIZE / 2.5));

	MyUI subTitle("Click Space To Play!", FONT_REGULAR_SIZE, Color::White, Vector2f(WINDOW_SIZE / 2, WINDOW_SIZE / 2));



	// =========== Managing Variables ========== //

	int gameState = 1;



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
		case 1:
			StartScreen(window, gameState, titleText, subTitle);
			if (gameState == 2) //start the next round
			{
				currentBoard = (currentBoard + 1) % 3;
				if (!StartGame(&boards[currentBoard], &pacman, ghosts)) return 0; //break if one of the boards has been modified
			}
			break;
		case 2:
			int gameStatus = PlayGame(window, &boards[currentBoard], &pacman, ghosts, deltaTime);
			switch (gameStatus)
			{
			case 1: //pacman dies
				return 0;
				break;
			case 2: // all pellets collected
				currentBoard = (currentBoard + 1) % 3;
				if (!StartGame(&boards[currentBoard], &pacman, ghosts)) return 0; //break if one of the boards has been modified
				break;
			}
			break;
		}


		
		/*

		*/

		window.display();
	}
	return 0;
}


