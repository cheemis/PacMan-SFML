#pragma once
#include "Dependencies.h"
#include "Board.h"


struct QueueElement
{
	Vector2i tile;
	int score;

	bool operator<(const QueueElement& other) const
	{
		return score < other.score;
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
	
	//color
	Color color;

	void UpdatePosition(float deltaTime, Board& currentBoard)
	{
		//get percentage
		percentage += (speed * deltaTime) / 100; //divided by 100 to turn into a percentage

		//update target cell
		if (percentage >= 1) ChangeTargetCell(currentBoard);

		MoveTowardsTargetCell();
	}

	virtual void ChangeTargetCell(Board& currentBoard){}

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
	Entity(float speed, Color color, Vector2i startingTile) :
		speed(speed),
		position(Vector2f(startingTile.x* TILE_OFFSET, startingTile.y* TILE_OFFSET)),
		currentTile(startingTile),
		targetTile(startingTile),
		percentage(0),
		color(color) {}


	//getters/setters
	CircleShape GetShape()
	{
		CircleShape shape(TILE_SIZE / 2);
		shape.setPosition(position);
		shape.setFillColor(color);
		return shape;
	}

	//virtual methods
	virtual void Update(float deltaTime, Board& currentBoard) {}
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

public:
	Pacman(float speed, Color color, Vector2i startingTile) :
		Entity(speed, color, startingTile),
		direction(Vector2i(1,0)) {}

	void Update(float deltaTime, Board& currentBoard)
	{
		ChangeDirection(currentBoard);
		UpdatePosition(deltaTime, currentBoard);
	}

};

// ========================================
// ======== Ghost Class Definition ========
// ========================================

class Ghost : public Entity
{
private:
	queue<Vector2i> path;

	
	void ChangeTargetCell(Board& currentBoard)
	{
		if (path.size() <= 0)
		{
			//recreate a new path
			targetTile = Vector2i(29, 26);
			AStar(currentTile, targetTile, currentBoard);
		}
		else
		{
			//pop an element from the queue
			Vector2i newTarget = path.front();
			path.pop();

			//set that popped element to the target cell
			targetTile = newTarget;
			percentage = 0;

		}
	}
	
	bool AStar(Vector2i start, Vector2i goal, Board& currentBoard)
	{
		
		priority_queue<QueueElement> openSet;
		openSet.push(QueueElement(start, GetFScore(start, goal))); //push first element

		map<Vector2i, vector<Vector2i>> cameFrom;

		map<string, Node> visited;
		visited[TileToString(start)] = Node(start, 0);

		while (!openSet.empty())
		{
			int validNeighbors = 0;

			//get the leading element
			QueueElement currentElement = openSet.top();
			int currentScore = GetFScore(currentElement.tile, goal);
			string currentID = TileToString(currentElement.tile);
			openSet.pop();

			if (currentElement.tile.x == goal.x && currentElement.tile.y == goal.y)
			{
				//return the constructed path
				FindPath(goal, visited, currentBoard);
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
				Vector2i n(neighbors[i]);
				if (currentBoard.GetCell(n.x, n.y))
				{
					int neighboreScore = GetFScore(n, goal);
					string neighborID = TileToString(n);

					//if haven't already traversed this space
					if (!visited.contains(neighborID))
					{
						//add this space to the queue

						//set fscore
						visited[neighborID] = Node(n, &visited[currentID]);

						//push this space to the queue
						int f = GetFScore(n, goal);
						openSet.push(QueueElement(n, f));
						validNeighbors++;
					}
				}
			}
			cout << "(" << to_string(currentElement.tile.x) <<  ", " << to_string(currentElement.tile.y) << ") has " << to_string(validNeighbors) << " valid neighbors" << endl;
		}
		cout << "couldn't find target";
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

	void FindPath(Vector2i goal, map<string, Node> &fScores, Board& currentBoard)
	{
		Node current = fScores[TileToString(goal)];
		vector<Vector2i> newPath;
		while (current.previous)
		{
			cout << "(" << current.tile.x << ", " << current.tile.y << ") -- ";
			newPath.push_back(current.tile);
			current = *current.previous;
		}
		cout << "(" << current.tile.x << ", " << current.tile.y << ")" << endl;



		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				if (currentBoard.GetCell(j, i))
				{
					if (find(newPath.begin(), newPath.end(), Vector2i(j, i)) != newPath.end())
					{
						cout << "o";
					}
					else
					{
						cout << " ";
					}
				}
				else
				{
					cout << "X";
				}
			}
			cout << endl;
		}

	}

public:
	Ghost(float speed, Color color, Vector2i startingTile) :
		Entity(speed, color, startingTile) {}

	void Update(float deltaTime, Board& currentBoard)
	{
		UpdatePosition(deltaTime, currentBoard);
	}
};


// ========================================
// ==== Queue Element Class Definition ====
// ========================================





