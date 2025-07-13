
#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include "Tile.hpp"
#include "Grid.hpp"
#include <list>


class GameEntity{
public:
	// Fields
	Vector3 position;
	Vector3 targetPosition;
	float t;
	Tile* tile;
	std::list<Tile> currentPath;
	std::list<Tile>::iterator pathIterator;
	Color currentColor;
	Color defaultColor;
	float size;
	Mesh mesh;
	Model model;
	BoundingBox collider;
	bool selected;
	bool isMoving;
	Grid* parentGrid;

	GameEntity(Grid& grid, Vector3 initialPosition, Color initialColor, Tile* initialTile);

	~GameEntity();

	void SetPath(std::list<Tile> path);
	void UpdateMove(float moveSpeed, float deltaTime);
	void Draw();
};

#endif

