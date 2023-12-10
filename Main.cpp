#include "Main.h"

int main()
{
	RenderWindow window(VideoMode(512, 512), "SFML works!");

	Board currentBoard = Board("maze0.txt");
	Pacman pacman = Pacman(500, Vector2i(1, 1));



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
		//window.draw(shape);
		currentBoard.DrawBoard(window);

		pacman.Update(deltaTime, currentBoard);
		window.draw(pacman.GetShape());


		window.display();
	}
	return 0;
}
