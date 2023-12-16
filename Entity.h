#pragma once

#include "Dependencies.h"
#include "Board.h"
#include "MyUI.h"


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
	Vector2i home;
	
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
	Entity(float speed, Color color, Vector2i startingTile, Board* currentBoard) :
		speed(speed),
		radius(ENTITY_RADIUS),
		position(Vector2f(startingTile.x* TILE_OFFSET, startingTile.y* TILE_OFFSET)),
		currentTile(startingTile),
		targetTile(startingTile),
		percentage(0),
		color(color),
		currentBoard(currentBoard),
		home(startingTile){}

	virtual void ResetEntity(Vector2i newHome, Board* newBoard)
	{
		position = Vector2f(newHome.x * TILE_OFFSET, newHome.y * TILE_OFFSET);
		currentTile = newHome;
		targetTile = newHome;
		home = newHome;

		percentage = 0;

		currentBoard = newBoard;
	}


	//getters/setters
	virtual CircleShape GetShape()
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

	//life variables
	bool isAlive;
	float shrinkTime;

	//eating variables
	int recentlyEaten;

	//functions
	Vector2i ChangeDirection();
	void ChangeTargetCell();
	
public:
	Pacman(float speed, Color color, Vector2i startingTile, Board* currentBoard) :
		Entity(speed, color, startingTile, currentBoard),
		direction(Vector2i(0,0)),
		isAlive(true),
		shrinkTime(10 * DEATH_TIME/2),
		recentlyEaten(0) {}

	void Update(float deltaTime);
	void ResetEntity(Vector2i newHome, Board* newBoard);
	void Dying(float deltaTime);

	bool GetIsAlive();
	int GetRecentlyEaten();
	Vector2i GetDirection();

	void SetIsAlive(bool newState);
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
	int ghostType;
	bool randomPathing;

	//feeling variables
	float fleeingTime;
	bool isDead;

	//scoreboard variables
	MyScore* score;

	//prototypes
	void ChangeTargetCell();

	bool AStar(Vector2i start, Vector2i goal);
	int GetFScore(Vector2i current, Vector2i goal);
	void FindPath(Vector2i goal, map<string, Node>& fScores);
	Vector2i GetGoal();

	string TileToString(Vector2i tile);
	Vector2i FindValidTileNear(Vector2i pacLocation, Vector2i offset, int multiplier);

	void CheckCollision();
	void CheckFleeing(float deltaTime);
	void StartFleeing();

	
	

public:
	Ghost(float speed, int ghostType, Color color, Vector2i startingTile, Board* currentBoard, Pacman* pacman, MyScore* score) :
		Entity(speed, color, startingTile, currentBoard),
		pacman(pacman),
		direction(Vector2i(0, 0)),
		ghostType(ghostType),
		randomPathing(ghostType == 3),
		fleeingTime(0),
		isDead(false),
		score(score) {};

	void ResetEntity(Vector2i newHome, Board* newBoard);

	Vector2i GetRandomSpace();

	void Update(float deltaTime);

	CircleShape GetShape();
};






