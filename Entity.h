#pragma once

#include "Dependencies.h"
#include "Board.h"


// ========================================
// ==== Queue Element Class Definition ====
// ========================================

struct QueueElement
{
	Vector2i tile;
	int score;

	bool operator<(const QueueElement& other) const
	{
		return score > other.score; //reversed to put smallest at front
	}
};

struct Node
{
	Vector2i tile;
	Node* previous;
	int neighbors;
};


// ========================================
// ======== Entity Class Definition =======
// ========================================

class Entity
{

protected:
	//movement variables
	float speed;
	Vector2f position;

	//shape variables
	float radius;

	//traversing variables
	Vector2i currentTile;
	Vector2i targetTile;
	float percentage;

	//board variables
	Board* currentBoard;
	
	//color
	Color color;

	void UpdatePosition(float deltaTime)
	{
		//get percentage
		percentage += (speed * deltaTime) / 100; //divided by 100 to turn into a percentage

		//update target cell
		if (percentage >= 1) ChangeTargetCell();

		MoveTowardsTargetCell();
	}
	
	virtual void ChangeTargetCell(){}

	void MoveTowardsTargetCell()
	{
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
	}

	float TileToWorldPosition(int tile)
	{
		return tile * TILE_OFFSET;
	}


public:
	//constructor
	Entity(float speed, float radius, Color color, Vector2i startingTile, Board* currentBoard) :
		speed(speed),
		radius(radius),
		position(Vector2f(startingTile.x* TILE_OFFSET, startingTile.y* TILE_OFFSET)),
		currentTile(startingTile),
		targetTile(startingTile),
		percentage(0),
		color(color),
		currentBoard(currentBoard){}


	//getters/setters
	CircleShape GetShape()
	{
		CircleShape shape(radius);
		shape.setPosition(position);
		shape.setFillColor(color);
		return shape;
	}

	Vector2i GetCurrentTile()
	{
		return currentTile;
	}

	Vector2f GetPosition()
	{
		return position;
	}

	float GetRadius()
	{
		return radius;
	}

	//virtual methods
	virtual void Update(float deltaTime) {}
};

// ========================================
// ======== Pacman Class Definition =======
// ========================================

class Pacman : public Entity
{
private:
	//movement variables
	Vector2i direction;

	//
	bool isAlive;


	//functions
	void ChangeDirection()
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

	void ChangeTargetCell()
	{
		Vector2f futureTarget(targetTile.x + direction.x, targetTile.y + direction.y);
		percentage = 0;

		//cout << "future target: (" << futureTarget.x << ", " << futureTarget.y << ") - " <<
		//	(currentBoard->GetCell(futureTarget.x, futureTarget.y) ? "valid" : "wall") << endl;

		currentBoard->RemovePellet(targetTile); //remove the pellet at that location

		currentTile = targetTile;
		if (currentBoard->GetTile(futureTarget.x, futureTarget.y))
		{
			targetTile = Vector2i(targetTile.x + direction.x, targetTile.y + direction.y);
		}

	}

public:
	Pacman(float speed, float radius, Color color, Vector2i startingTile, Board* currentBoard) :
		Entity(speed, radius, color, startingTile, currentBoard),
		direction(Vector2i(1,0)),
		isAlive(true) {}

	void Update(float deltaTime)
	{
		ChangeDirection();
		UpdatePosition(deltaTime);
	}

	bool GetIsAlive()
	{
		return isAlive;
	}

	void SetIsAlive(bool newState)
	{
		isAlive = newState;
	}
};

// ========================================
// ======== Ghost Class Definition ========
// ========================================

class Ghost : public Entity
{
private:
	//traversial
	stack<Vector2i> path;
	Vector2i direction;

	//other classes
	Pacman* pacman;

	//ghost variables
	int ghostType = 0;

	//prototypes
	void ChangeTargetCell();

	bool AStar(Vector2i start, Vector2i goal);
	int GetFScore(Vector2i current, Vector2i goal);
	void FindPath(Vector2i goal, map<string, Node>& fScores);
	Vector2i GetGoal();

	string TileToString(Vector2i tile);
	void FindValidTileNear(Vector2i origin);
	void CheckCollision();

public:
	Ghost(float speed, float radius, Color color, Vector2i startingTile, Board* currentBoard, Pacman* pacman) :
		Entity(speed, radius, color, startingTile, currentBoard),
		pacman(pacman),
		direction(Vector2i(0,0)) {}

	void Update(float deltaTime);
};






