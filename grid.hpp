
#pragma once
#include <raylib.h>
#include <vector>

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

	int health;
	int aim;
	int resolve;

	Color color;

	//eventually they need a mesh / collider I think?
	//	// maybe not on collider that might all be grid math.
};

namespace TacticalGrid {
	extern TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	extern int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	extern bool movementGrid[GRID_HEIGHT][GRID_WIDTH];
	// extern bool highlightGrid[GRID_HEIGHT][GRID_WIDTH];

	extern std::vector<Unit> units; 

	void initGrids();
	void clearTerrainGrid();
	void clearMovementGrid();
	void clearUnitGrid();
	// void clearHighlightGrid();

	void printMovementGrid();

	bool isPassable(int x, int y);

	Unit* getUnitAt(int x, int y);
	bool isUnitAt(int x, int y);
	void moveUnit(int unitId, int newX, int newY);

	void calculateMovementRange(int unitId);
	void showMovementPath(int fromX, int fromY, int toX, int toY); 
	
	void setHighlight(int unitId);
	
	void drawHighlight(Vector3 worldOrigin);
	void drawTerrain(Vector3 worldOrigin);
	void drawUnits(Vector3 worldOrigin);
	void drawMovementOverlay(Vector3 worldOrigin);
}
