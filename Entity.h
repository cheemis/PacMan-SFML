#pragma once
#include "Dependencies.h"
#include "Board.h"


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
	Entity(float speed, Color color, Vector2i startingTile, Board* currentBoard) :
		speed(speed),
		position(Vector2f(startingTile.x* TILE_OFFSET, startingTile.y* TILE_OFFSET)),
		currentTile(startingTile),
		targetTile(startingTile),
		percentage(0),
		color(color),
		currentBoard(currentBoard){}


	//getters/setters
	CircleShape GetShape()
	{
		CircleShape shape(TILE_SIZE / 2);
		shape.setPosition(position);
		shape.setFillColor(color);
		return shape;
	}

	Vector2i GetCurrentTile()
	{
		return currentTile;
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

		currentTile = targetTile;
		if (currentBoard->GetCell(futureTarget.x, futureTarget.y))
		{
			targetTile = Vector2i(targetTile.x + direction.x, targetTile.y + direction.y);
		}

	}

public:
	Pacman(float speed, Color color, Vector2i startingTile, Board* currentBoard) :
		Entity(speed, color, startingTile, currentBoard),
		direction(Vector2i(1,0)) {}

	void Update(float deltaTime)
	{
		ChangeDirection();
		UpdatePosition(deltaTime);
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


	//testing
	Vector2i testPoints[4];
	int testIndex;


	
	void ChangeTargetCell()
	{
		if (path.size() <= 0) return; //edge case: can't route to player

		//pop an element from the queue
		Vector2i newTarget = path.top();
		path.pop();

		//cout << "(" << to_string(targetTile.x) << ", " << to_string(targetTile.y) << ") ==> (" << to_string(newTarget.x) << ", " << to_string(newTarget.y) << ")" << endl;

		//set that popped element to the target cell
		currentTile = targetTile;
		targetTile = newTarget;
		percentage = 0;

	}
	
	bool AStar(Vector2i start, Vector2i goal)
	{
		//instantiate data structures
		priority_queue<QueueElement> openSet;
		map<Vector2i, vector<Vector2i>> cameFrom;
		map<string, Node> visited;

		//push first elements
		openSet.push(QueueElement(start, GetFScore(start, goal))); //push first element
		visited[TileToString(start)] = Node(start, 0);
		
		while (!openSet.empty())
		{
			int validNeighbors = 0;

			//get the leading element
			QueueElement currentElement = openSet.top();
			int currentScore = GetFScore(currentElement.tile, goal);
			string currentID = TileToString(currentElement.tile);
			openSet.pop();

			//cout << "Current: (" << currentElement.tile.x << ", " << currentElement.tile.y << "), Score: " << currentElement.score << endl;

			if (currentElement.tile.x == goal.x && currentElement.tile.y == goal.y)
			{
				//return the constructed path
				FindPath(goal, visited);
				return true;
			}
			
			//get valid neighbors
			vector<Vector2i> neighbors;
			neighbors.push_back(Vector2i(currentElement.tile.x + 1, currentElement.tile.y));
			neighbors.push_back(Vector2i(currentElement.tile.x - 1, currentElement.tile.y));
			neighbors.push_back(Vector2i(currentElement.tile.x, currentElement.tile.y + 1));
			neighbors.push_back(Vector2i(currentElement.tile.x, currentElement.tile.y - 1));

			//push neighbors to array
			for (int i = 0; i < neighbors.size(); i++)
			{
				Vector2i neighbor(neighbors[i]);
				if (currentBoard->GetCell(neighbor.x, neighbor.y))
				{
					int neighboreScore = GetFScore(neighbor, goal) + currentElement.score;
					string neighborID = TileToString(neighbor);

					//if haven't already traversed this space
					if (!visited.contains(neighborID))
					{
						//add this space to the queue

						//set fscore
						visited[neighborID] = Node(neighbor, &visited[currentID]);

						//push this space to the queue
						openSet.push(QueueElement(neighbor, neighboreScore));
						validNeighbors++;
					}
				}
			}

			if (validNeighbors > 2)
			{
				//return the constructed path
				FindPath(openSet.top().tile, visited);
				return true;
			}
			//cout << "(" << to_string(currentElement.tile.x) <<  ", " << to_string(currentElement.tile.y) << ") has " << to_string(validNeighbors) << " valid neighbors" << endl;
		}
		return false;
	}

	string TileToString(Vector2i tile)
	{
		return to_string(tile.x) + "," + to_string(tile.y);
	}
	
	int GetFScore(Vector2i current, Vector2i goal)
	{
		return abs(current.x - goal.x) + abs(current.y - goal.y);
	}

	void FindPath(Vector2i goal, map<string, Node> &fScores)
	{
		Node current = fScores[TileToString(goal)];
		while (current.previous)
		{
			//cout << "(" << current.tile.x << ", " << current.tile.y << ") -- ";
			path.push(current.tile);
			Node c = current;
			current = *current.previous;
		}
		//cout << "(" << current.tile.x << ", " << current.tile.y << ")" << endl;
	}

	Vector2i GetGoal()
	{
		switch (ghostType)
		{
		case(0): //Blinky (red)
			return pacman->GetCurrentTile();
			break;
		case(1): //Pinky (pink)
			return pacman->GetCurrentTile();
			break;
		case(2): //Inky (cyan)
			return pacman->GetCurrentTile();
			break;
		case(3): //Clyde (orange)
			return pacman->GetCurrentTile();
			break;
		}
	}

	Vector2i FindValidTileNear(Vector2i origin)
	{
		bool foundValid = false;
		while (!foundValid)
		{
		}
	}

public:
	Ghost(float speed, Color color, Vector2i startingTile, Board* currentBoard, Pacman* pacman) :
		Entity(speed, color, startingTile, currentBoard),
		pacman(pacman),
		direction(Vector2i(0,0))
	{
		testPoints[0] = Vector2i(29, 26);
		testPoints[1] = Vector2i(1,1); //these two points don't take most efficient path
		testPoints[2] = Vector2i(9,22);
		testPoints[3] = Vector2i(29,1);
		testIndex = 0;
	}

	void Update(float deltaTime)
	{
		if (path.size() <= 0)
		{
			//recreate a new path

			Vector2i newTargetTile = GetGoal();

			cout << "Astar being used!" << endl;

			

			if (!AStar(targetTile, newTargetTile))
			{
				//if a path couldn't be found
				cout << "couldn't find path!" << endl;
			}
			testIndex = (testIndex + 1) % 4;
		}

		UpdatePosition(deltaTime);
	}
};


// ========================================
// ==== Queue Element Class Definition ====
// ========================================





