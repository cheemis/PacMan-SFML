#pragma once
#include "Dependencies.h"
#include "Board.h"

class Pacman
{
private:

	//movement variables
	float speed;
	Vector2i direction;
	Vector2f position;

	//traversing variables
	Vector2i currentTile;
	Vector2i targetTile;
	float percentage;



public:
	Pacman(float speed, Vector2i startingTile) :
		speed(speed),
		direction(Vector2i(1,0)),
		position(Vector2f(startingTile.x* TILE_OFFSET, startingTile.y* TILE_OFFSET)),
		currentTile(startingTile),
		targetTile(startingTile),
		percentage(0) {}

	void Update(float deltaTime, Board& currentBoard)
	{
		ChangeDirection(currentBoard);
		MovePacman(deltaTime, currentBoard);
	}

	void ChangeDirection(Board& currentBoard)
	{
		int newX = 0;
		int newY = 0;


		//go up
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			newY = -1;
		}
		//go down
		else if (Keyboard::isKeyPressed(Keyboard::S))
		{
			newY = 1;
		}

		//go right
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			newX = 1;
			newY = 0;
		}
		//go left
		else if (Keyboard::isKeyPressed(Keyboard::A))
		{
			newX = -1;
			newY = 0;
		}

		//if the player pressed a key, change direction
		if (newX || newY)
		{
			direction = Vector2i(newX, newY);
		}

	}

	void MovePacman(float deltaTime, Board& currentBoard)
	{

		//get percentage
		percentage += (speed * deltaTime) / 100; //divided by 100 to turn into a percentage

		//update target cell
		if (percentage >= 1) ChangeTargetCell(currentBoard);

		//convert target position to world position
		Vector2f targetWorldPosition = Vector2f(TileToWorldPosition(targetTile.x),
												TileToWorldPosition(targetTile.y));

		Vector2f currentWorldPosition = Vector2f(TileToWorldPosition(currentTile.x),
												 TileToWorldPosition(currentTile.y));

		//get distance between points
		Vector2f distance(targetWorldPosition.x - currentWorldPosition.x,
						  targetWorldPosition.y - currentWorldPosition.y);


		//move player a percentage between those two points
		position = Vector2f(currentWorldPosition.x + (distance.x * percentage),
							currentWorldPosition.y + (distance.y * percentage));
		/*
		cout << "position: " << position.x << ", " << position.y << endl <<
			"\t-- %: " << percentage << endl <<
			"\t-- currentWP: " << currentWorldPosition.x << ", " << currentWorldPosition.y << endl <<
			"\t-- targetWP: " << targetWorldPosition.x << ", " << targetWorldPosition.y << endl <<
			"\t-- distance: " << distance.x << ", " << distance.y << endl <<
			"\t== TRAVEL: " << (distance.x * percentage) << ", " << (distance.y * percentage) << endl;
		*/
	}

	void ChangeTargetCell(Board& currentBoard)
	{
		Vector2f futureTarget(targetTile.x + direction.x, targetTile.y + direction.y);
		percentage = 0;

		cout << "future target: (" << futureTarget.x << ", " << futureTarget.y << ") - " <<
			(currentBoard.GetCell(futureTarget.x, futureTarget.y) ? "valid" : "wall") << endl;

		currentTile = targetTile;
		if (currentBoard.GetCell(futureTarget.x, futureTarget.y))
		{
			targetTile = Vector2i(targetTile.x + direction.x, targetTile.y + direction.y);
		}

	}

	float TileToWorldPosition(int tile)
	{
		return tile * TILE_OFFSET;
	}

	CircleShape GetShape()
	{
		CircleShape shape(TILE_SIZE/2);
		shape.setPosition(position);
		shape.setFillColor(Color::Yellow);
		return shape;
	}
};