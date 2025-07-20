
#pragma once
#include <raylib.h>

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

namespace TacticalGrid {
	extern TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];

	void initGrid();
	void clearTerrainGrid();

	bool isPassable(int x, int y);
	
	void drawTerrain(Vector3 worldOrigin);
}
