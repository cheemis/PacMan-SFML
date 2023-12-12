#include "Main.h"

int main()
{
	RenderWindow window(VideoMode(512, 512), "SFML works!");

	Board currentBoard = Board("maze0.txt");
	//Pacman pacman = Pacman(500, Color::Yellow,Vector2i(1, 1));
	Ghost ghost(250, Color::Red, Vector2i(1, 1));


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
		//pacman.Update(deltaTime, currentBoard);
		ghost.Update(deltaTime, currentBoard);

		//draw entities
		window.draw(ghost.GetShape());

		window.display();
	}
	return 0;
}
