#include "Main.h"

int main()
{
	// ============ Window Variables =========== //

	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pacman!");


	// ============== UI Variables ============= //

	MyUI titleText("Pacman",
		FONT_TITLE_SIZE,
		Color::Yellow,
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2.5));

	MyUI subTitle("Click Space To Play!", FONT_REGULAR_SIZE, Color::White, Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	MyScore scoreBoard("Score:\n0",
		FONT_REGULAR_SIZE,
		Color::White,
		Vector2f(COLUMNS * TILE_OFFSET + TEXT_SPACER, FONT_REGULAR_SIZE + TEXT_SPACER));

	MyUI livesText("Lives:\n" + to_string(MAX_LIVES),
		FONT_REGULAR_SIZE,
		Color::Green,
		Vector2f(COLUMNS * TILE_OFFSET + TEXT_SPACER, LIVES_SPACER));

	MyUI readyText("Ready?",
		FONT_TITLE_SIZE,
		Color::Yellow,
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2.5));

	MyUI gameOverText("Game Over",
		FONT_TITLE_SIZE,
		Color::Yellow,
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2.5));

	// ============= Game Entities ============= //

	Board boards[] = { Board("maze0.txt", WALL_COLOR1, &scoreBoard),
					   Board("maze1.txt", WALL_COLOR2, &scoreBoard),
					   Board("maze2.txt", WALL_COLOR3, &scoreBoard)};
	int currentBoard = -1;

	Pacman pacman = Pacman(PACMAN_SPEED,Color::Yellow, boards[0].GetStartingPosition(0), &boards[0]);

	Ghost ghosts[] = { Ghost(GHOST_SPEED, 0, Color::Red, boards[0].GetStartingPosition(1), &boards[0], &pacman, &scoreBoard),
					   Ghost(GHOST_SPEED, 1, Color::Magenta, boards[0].GetStartingPosition(3), &boards[0], &pacman, &scoreBoard),
					   Ghost(GHOST_SPEED, 2, Color(255, 184,71), boards[0].GetStartingPosition(4), &boards[0], &pacman, &scoreBoard),
					   Ghost(GHOST_SPEED, 3, Color::Cyan, boards[0].GetStartingPosition(2), &boards[0], &pacman, &scoreBoard)};

	// =========== Managing Variables ========== //

	int gameState = 1;
	float timing = READY_TIME;
	int lives = MAX_LIVES;

	// =========== Initializing work =========== //

	//check to see if all boards valid
	for (int i = 0; i < 3; i++)
	{
		if (!boards[i].GetFunctional()) return 0;
		//boards[i].SetGhosts(ghosts);
	}


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
			if (gameState == 5) //start the next round
			{
				currentBoard = (currentBoard + 1) % 3;
				if (!StartGame(&boards[currentBoard], &pacman, ghosts)) return 0; //break if one of the boards has been modified
				timing = READY_TIME;
			}
			break;
		case 2: //playing the game
			window.draw(scoreBoard.GetText());
			window.draw(livesText.GetText());
			gameStatus = PlayGame(window, &boards[currentBoard], &pacman, ghosts, deltaTime);
			gameWorking = GameOutcome(gameState, gameStatus, timing, currentBoard, boards, &pacman, ghosts);
			if (!gameWorking) return 0;
			break;
		case 3: //pacman is dying
			Dying(window, //window variables
				&boards[currentBoard], //board variables
				gameState, //game state variables -- setting gamestate to 5
				lives, &livesText, //lives variables
				&pacman, ghosts, //entities
				timing, deltaTime); //timing variabes
			break;
		case 4: //stage is blinking
			//if a map has been altered, close game
			if (!Blinking(window, //window variables
							boards, currentBoard, //board variables
							gameState, //game state variables -- setting gamestate to 2
							&pacman, ghosts, //entities
							timing, deltaTime)) //timing variabes
							return 0;
			break;

		case 5: //ready to play
			if (timing < 0)
			{
				gameState = 2;
			}
			else
			{
				timing -= deltaTime;
			}
			ReadyScreen(&window, &boards[currentBoard], &readyText);
			break;

		case 6: //game over
			if (timing < 0)
			{
				gameState = 1; //go back to title screen
				scoreBoard.ResetScore();
				SetLives(3, lives, &livesText);
				currentBoard = -1;
			}
			else
			{
				timing -= deltaTime;
			}
			ReadyScreen(&window, &boards[currentBoard], &gameOverText);
			break;
		}
		window.display();
	}
	return 0;
}


