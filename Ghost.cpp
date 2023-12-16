#include "Entity.h"

// ========================================= //
// =========== Private Functions =========== //
// ========================================= //

// ========== Overriding Functions ========= //

void Ghost::ChangeTargetCell()
{
	Vector2i newTarget;

	if (path.size() <= 0) //edge case: can't route to player
	{
		//recreate a new path
		Vector2i newTargetTile = GetGoal();
		bool foundPath = AStar(targetTile, newTargetTile);
		isDead = false;

		while(!foundPath)
		{
			foundPath = AStar(targetTile, GetRandomSpace());
		}

		percentage = clamp(percentage, 0.0f, 1.0f);
	}
	else
	{
		newTarget = path.top();

		//pop an element from the queue

		path.pop();

		//set that popped element to the target cell
		currentTile = targetTile;
		targetTile = newTarget;
		percentage = 0;
	}
}



// ========= Path Finding Functions ======== //

bool Ghost::AStar(Vector2i start, Vector2i goal)
{
	//instantiate data structures
	priority_queue<QueueElement> openSet;
	map<Vector2i, vector<Vector2i>> cameFrom;
	map<string, Node> visited;

	//push first elements
	openSet.push(QueueElement(start, GetFScore(start, goal))); //push first element
	visited[TileToString(start)] = Node(start, 0, 0);

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
			if (currentBoard->GetTile(neighbor.x, neighbor.y))
			{
				int neighboreScore = GetFScore(neighbor, goal) + currentElement.score;
				string neighborID = TileToString(neighbor);

				//if haven't already traversed this space
				if (!visited.contains(neighborID))
				{
					//add this space to the queue

					//set fscore
					visited[neighborID] = Node(neighbor, &visited[currentID], 0);

					//push this space to the queue
					openSet.push(QueueElement(neighbor, neighboreScore));
					validNeighbors++;
				}
			}
		}
		visited[currentID].neighbors = validNeighbors;
	}
	return false;
}

int Ghost::GetFScore(Vector2i current, Vector2i goal)
{
	return abs(current.x - goal.x) + abs(current.y - goal.y);
}

void Ghost::FindPath(Vector2i goal, map<string, Node>& fScores)
{
	Node current = fScores[TileToString(goal)];
	stack<Node> nodeStack;

	//if ghost should complete full path search
	if (ghostType == 2 || isDead || fleeingTime > 0)
	{
		while (current.previous)
		{
			path.push(current.tile);
			current = *current.previous;
		}
		return;
	}

	//reverse list of nodes
	while (current.previous)
	{
		nodeStack.push(current);
		current = *current.previous;
	}

	//add appropriate number of nodes to queue - sorry not sorry I had a week
	while (!nodeStack.empty())
	{
		Node currentNode = nodeStack.top();
		nodeStack.pop();
		path.push(currentNode.tile);
		if (currentNode.neighbors > 0)
		{
			return;
		}
	}
}

Vector2i Ghost ::GetGoal()
{
	int pathing;
	float randInt = ((1.0f * rand()) / RAND_MAX);

	//check if blinking
	if (fleeingTime > 0) pathing = 3;
	else
	{
		//regular pathing
		if (ghostType == 3) //for inky
		{
			pathing = (int)randInt % 3;
		}
		else //change to go random
		{
			if (randInt * 100 < CHANCE_TO_RANDOM) pathing = 2;
			else pathing = ghostType;
		}
	}

	


	switch (pathing)
	{
	case(0): //Blinky (red)
		return pacman->GetCurrentTile();
		break;
	case(1): //Pinky (pink)
		return FindValidTileNear(pacman->GetCurrentTile(),
								 -pacman->GetDirection(), 5);
		break;
	case(2): //Clyde (orange)
		return GetRandomSpace();
		
	case(3): //fleeing
		return GetRandomSpace();
		break;
	}
}

void Ghost::ResetEntity(Vector2i newHome, Board* newBoard)
{
	Entity::ResetEntity(newHome, newBoard);
	while (!path.empty()) path.pop();
	isDead = false;
	fleeingTime = 0;
}

Vector2i Ghost::GetRandomSpace()
{
	int row = 0;
	int col = 0;

	do
	{
		row = (1.0f * rand() / RAND_MAX) * COLUMNS;
		col = (1.0f * rand() / RAND_MAX) * ROWS;
	} while (!currentBoard->GetTile(row, col) &&
			(row != currentTile.x && col != currentTile.y));
	return Vector2i(row, col);
}


// ========== Collision Functions ========== //

void Ghost::CheckCollision()
{
	//only do collision checks if not dead
	if (!isDead)
	{
		Vector2f pacLocation = pacman->GetPosition();
		float pacRadius = pacman->GetRadius();
		float distanceSquared = pow(pacLocation.x - position.x, 2) + pow(pacLocation.y - position.y, 2);
		if (distanceSquared - pow(pacRadius + radius, 2) < 0)
		{
			//if not fleeing, kill pacman
			if (fleeingTime <= 0)
			{
				pacman->SetIsAlive(false);
			}
			//else, route back to spawn
			else
			{
				fleeingTime = 0;
				isDead = true;
				while (!path.empty()) path.pop();
				AStar(targetTile, home);

				score->IncreaseScore(GHOST_POINTS);
			}
		}
	}
}

// =========== Fleeing Functions =========== //

void Ghost::CheckFleeing(float deltaTime)
{
	if (!isDead)
	{
		if (pacman->GetRecentlyEaten() == 3) //if pacman ate a power pellet
		{
			StartFleeing();
		}
		else
		{
			fleeingTime -= deltaTime;
		}
	}
}

void Ghost::StartFleeing()
{
	fleeingTime = FLEEING_TIME;
}



// ============ Helper Functions =========== //

string Ghost::TileToString(Vector2i tile)
{
	return to_string(tile.x) + "," + to_string(tile.y);
}

Vector2i Ghost::FindValidTileNear(Vector2i pacLocation, Vector2i offset, int multiplier)
{
	Vector2i target = pacLocation + offset * multiplier;

	while (!currentBoard->GetTile(target.x, target.y))
	{
		if (targetTile == target) break; //base case
		target -= offset;
	}
	return target;
}



// ========================================= //
// ============ Public Functions =========== //
// ========================================= //

void Ghost::Update(float deltaTime)
{
	UpdatePosition(deltaTime);
	CheckCollision();
	CheckFleeing(deltaTime);
}


CircleShape Ghost::GetShape()
{
	CircleShape shape(radius);

	//setting color
	Color ghostColor = color;


	//if is dead
	if (isDead) ghostColor = DEAD_COLOR;

	//if fleeing
	else if (fleeingTime > 0)
	{
		//if blinking
		if (fleeingTime < FLASHING_TIME)
		{
			//blink
			if (((int)(fleeingTime * FLASHING_TIME) % 2)) ghostColor = Color::White;
			else ghostColor = FLEEING_COLOR;
		}
		else
		{
			ghostColor = FLEEING_COLOR;
		}
	}

	shape.setFillColor(ghostColor);

	shape.setPosition(position);
	return shape;
}
