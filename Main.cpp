#include "Main.h"

int main()
{
	RenderWindow window(VideoMode(512, 512), "Pacman!");

	Board currentBoard = Board("maze0.txt");
	Pacman pacman = Pacman(500, (TILE_SIZE * .5f),Color::Yellow, Vector2i(1, 1), &currentBoard);

	const int NUM_GHOSTS = 3;
	Ghost ghosts[] = { Ghost(500, (TILE_SIZE * .5f), Color::Red, Vector2i(29, 26), &currentBoard, &pacman),
					   Ghost(500, (TILE_SIZE * .5f), Color::Cyan, Vector2i(1, 26), &currentBoard, &pacman),
					   Ghost(500, (TILE_SIZE * .5f), Color::Magenta, Vector2i(29, 1), &currentBoard, &pacman)};

	//CollisionManager cm(10, &pacman);


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

		//draw the board
		currentBoard.DrawBoard(window);

		//update entities
		pacman.Update(deltaTime);
		//for (int i = 0; i < NUM_GHOSTS; i++) ghosts[i].Update(deltaTime);
		//for(Ghost g : ghosts) g.Update(deltaTime);

		//draw entities
		window.draw(pacman.GetShape());
		//for (int i = 0; i < NUM_GHOSTS; i++) window.draw(ghosts[i].GetShape());

		//cm.Update(deltaTime);

		if (!currentBoard.StillHasPellets()) return 0;

		window.display();
	}
	return 0;
}
