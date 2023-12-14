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
		randomPathing = true;

		while(!foundPath)
		{
			foundPath = AStar(targetTile, GetRandomSpace());
		}

		if (ghostType != 3) randomPathing = false;
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
		//cout << "(" << to_string(currentElement.tile.x) <<  ", " << to_string(currentElement.tile.y) << ") has " << to_string(validNeighbors) << " valid neighbors" << endl;
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
	if (randomPathing)
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

Vector2i Ghost::GetGoal()
{
	switch (ghostType)
	{
	case(0): //Blinky (red)
		return pacman->GetCurrentTile();
		break;
	case(1): //Pinky (pink)
		return FindValidTileNear(pacman->GetCurrentTile(),
								 -pacman->GetDirection(), 5);
		break;
	case(2): //Inky (cyan)
		return FindValidTileNear(pacman->GetCurrentTile(),
								 pacman->GetDirection(), 5);
	case(3): //Clyde (orange)
		return GetRandomSpace();
		break;
	}
}

void Ghost::ResetEntity(Vector2i newHome, Board* newBoard)
{
	Entity::ResetEntity(newHome, newBoard);
	while (!path.empty()) path.pop();

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
	Vector2f pacLocation = pacman->GetPosition();
	float pacRadius = pacman->GetRadius();
	float distanceSquared = pow(pacLocation.x - position.x, 2) + pow(pacLocation.y - position.y, 2);
	if (distanceSquared - pow(pacRadius + radius, 2) < 0)
	{
		pacman->SetIsAlive(false);
	}
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
}
