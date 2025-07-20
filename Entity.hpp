
#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>
#include "Tile.hpp"
#include "Grid.hpp"
#include <unordered_map>


class GameEntity{
public:
	// Fields
	Vector3 position;
	Tile* currentTile;
	Tile* prevTile;
	std::vector<Tile> currentPath;
	std::vector<Tile>::iterator pathIterator;
	Color currentColor;
	Color defaultColor;
	float size;
	Mesh mesh;
	Model model;
	BoundingBox collider;
	bool selected;
	bool isMoving;
	Grid* parentGrid;
	float speed;
	std::unordered_map<Tile, TileNode, TileHash> movementRange;

	GameEntity(Grid& grid, Vector3 initialPosition, Color initialColor, Tile* initialTile, float playerSpeed);

	~GameEntity();

	void SetPath(std::vector<Tile> path);
	void UpdateMove(float moveSpeed, float deltaTime);
	std::unordered_map<Tile, TileNode, TileHash> movementPreviewWithCost(Tile& start);
	std::vector<Tile> getPath(Tile& start, Tile& end, std::unordered_map<Tile, TileNode, TileHash>& movementRange);
	std::vector<Tile> getWaypointPath(Tile& start, std::vector<Tile>& waypoints, Tile& hoveredTile);
	void Draw();
};

#endif

