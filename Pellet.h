/*
#pragma once
#include "Dependencies.h"
#include "Entity.h"


// ========================================
// ==== Pellet Manager Class Definition ===
// ========================================

class CollisionManager
{
private:
	//pellet variables
	float numPellets;
	//vector<vector<Pellet>> pellets;

	//pacman variables
	Pacman* pacman;

	int lastRegion;

public:
	CollisionManager(float numPellets, Pacman* pacman) :
		numPellets(numPellets),
		//pellets(COLLISION_REGION_SIZE, vector<Pellet>(ROWS/COLLISION_REGION_SIZE * COLUMNS/COLLISION_REGION_SIZE)),
		pacman(pacman),
		lastRegion(-1){}

	void Update(float deltaTime)
	{
		Vector2i p = pacman->GetCurrentTile();

		int regionSize = COLLISION_REGION_SIZE / 2;	//(remember, rows and cols are rotated)
		int xPos = (p.x / (COLUMNS/regionSize));	//if player is less than half of cols, then in first row
		int yPos = (p.y / (ROWS/regionSize));		//if player is less than half of rows, then in first col

		int region = xPos + yPos * regionSize;

		if (region != lastRegion)
		{
			lastRegion = region;
			cout << "pacman is in region: " << region << endl;
		}
	}

};


// ========================================
// ======== Pellet Class Definition =======
// ========================================

class Pellet
{
private:
	Vector2f position;
	int& numPellets;

public:
	Pellet(Vector2i position, int& numPellets) :
		position(position),
		numPellets(numPellets) {}

	CircleShape GetShape()
	{
		CircleShape c(TILE_SIZE / 4);
		c.setFillColor(Color::White);
		c.setPosition(position);
		return c;
	}


};
*/
