#include "Entity.h"

// ========================================= //
// =========== Private Functions =========== //
// ========================================= //


Vector2i Pacman::ChangeDirection()
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
	return Vector2i(newX, newY);

}

void Pacman::ChangeTargetCell()
{
	direction = ChangeDirection();
	Vector2f futureTarget(targetTile.x + direction.x, targetTile.y + direction.y);
	percentage = 0;

	recentlyEaten = currentBoard->RemovePellet(targetTile); //remove the pellet at that location

	currentTile = targetTile;
	int tileVal = currentBoard->GetTile(futureTarget.x, futureTarget.y);
	if (tileVal && tileVal != 4)
	{
		targetTile = Vector2i(targetTile.x + direction.x, targetTile.y + direction.y);
	}
}


// ========================================= //
// ============ Public Functions =========== //
// ========================================= //


void Pacman::Update(float deltaTime)
{
	ChangeDirection();
	UpdatePosition(deltaTime);
}

void Pacman::ResetEntity(Vector2i newHome, Board* newBoard)
{
	Entity::ResetEntity(newHome, newBoard);
	isAlive = true;
	radius = ENTITY_RADIUS;
}

void Pacman::Dying(float deltaTime)
{
	radius = clamp(radius - shrinkTime * deltaTime, 0.0f, TILE_SIZE);
}

Vector2i Pacman::GetDirection()
{
	return direction;
}

bool Pacman::GetIsAlive()
{
	return isAlive;
}

int Pacman::GetRecentlyEaten()
{
	return recentlyEaten;
}

void Pacman::SetIsAlive(bool newState)
{
	isAlive = newState;
}