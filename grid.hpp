
#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>

const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 30;

const int TILE_SIZE = 4.0f;

enum TileType {
	TILE_EMPTY = 0,
	TILE_WALL = 1,
	TILE_ROCK = 2,
	TILE_TREE = 4,
	TILE_BOX = 5
};

struct Unit {
	int id;
	Vector2 gridPosition;

	float speed;
	float movePoints;
	int health;
	int aim;
	int resolve;

	Color color;
	bool isMoving;

	//eventually they need a mesh / collider I think?
	//	// maybe not on collider that might all be grid math.
};

struct MoveCell {
	//should only be 12 bytes of storage.
	float cost;
	Vector2 parent;
};


std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::ostream& operator<<(std::ostream& os, const Vector2& v);

namespace TacticalGrid {
	extern TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	extern int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	extern MoveCell movementGrid[GRID_HEIGHT][GRID_WIDTH];
	extern bool waypointGrid[GRID_HEIGHT][GRID_WIDTH];
	// extern bool highlightGrid[GRID_HEIGHT][GRID_WIDTH];

	extern std::vector<Unit> units; 
	extern std::vector<Vector2> waypoints;

	void initGrids();
	void clearTerrainGrid();
	void clearMovementGrid();
	void clearUnitGrid();
	void clearWaypoints();
	// void clearHighlightGrid();

	Vector3 gridToWorldPosition(Vector2 gridPos, float yLevel);

	void printMovementGrid();

	bool isPassable(int x, int y);

	Unit* getUnitAt(int x, int y);
	bool isUnitAt(int x, int y);
	void moveUnit(int unitId, int newX, int newY);

	bool inRange(int x, int y, float range);
	float getTerrainMultiplier(int x, int y);
	float getUnitMultiplier(int x, int y);

	void calculateMovementRange(int unitD);
	void calculateRangeFrom(int x, int y, float speed);
	std::vector<Vector2> reconstructPath(int fromX, int fromY, int toX, int toY, float speed);
	void showMovementPath(std::vector<Vector2>); 
	
	void setSelectedHighlight(int unitId);

	void drawHoverHighlight(int x, int y, Vector3 worldOrigin);
	void drawTerrain(Vector3 worldOrigin);
	void drawUnits(Vector3 worldOrigin);
	void drawMovementOverlay(Vector3 worldOrigin);
	void drawWaypoints(Vector3 worldOrigin);
	void drawPathPreview(std::vector<Vector2> path, Color color);
}
