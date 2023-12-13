#pragma once
#include "Entity.h"

// ========================================= //
// =========== Private Functions =========== //
// ========================================= //

// ========== Overriding Functions ========= //

void Ghost::ChangeTargetCell()
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

void Ghost::FindValidTileNear(Vector2i origin)
{
	bool foundValid = false;
	while (!foundValid)
	{
	}
}



// ========================================= //
// ============ Public Functions =========== //
// ========================================= //

void Ghost::Update(float deltaTime)
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
	}

	UpdatePosition(deltaTime);
	CheckCollision();


}
