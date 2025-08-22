
#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "cover.hpp"
#include "elevation.hpp"
#include "types.hpp"

const int GRID_WIDTH = 50;
const int GRID_HEIGHT = 50;

const int TILE_SIZE = 4.0f;

enum TileType {
	TILE_EMPTY = 0,
	TILE_WALL = 1,
	TILE_ROCK = 2,
	TILE_TREE = 4,
	TILE_BOX = 5
};


struct GridUnit {

	//bool playerControlled?
	//grid stuff
	int id;
	GridLocation gridPosition;
	bool playerControlled;

	//stats
	float speed;

	//render stuff
	Color color;
	bool isMoving;

	//turn stuff
	int movePointsRemaining;
	int actionPointsRemaining;
	bool turnComplete; // used up all actions for this turn.
	// bool isStunned


	//eventually they need a mesh / collider I think?
	//	// maybe not on collider that might all be grid math.
};

struct MoveCell {
	//should only be 12 bytes of storage.
	float cost;
	GridLocation parent;
};

struct PathData {
	std::vector<GridLocation> path;
	float totalCost;
	bool isReachable;
}; 


std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::ostream& operator<<(std::ostream& os, const Vector2& v);
std::ostream& operator<<(std::ostream& os, const std::vector<Vector2>& v);

namespace TacticalGrid {
	extern TileType terrainGrid[3][GRID_HEIGHT][GRID_WIDTH];
	extern int unitGrid[3][GRID_HEIGHT][GRID_WIDTH];
	extern int movementGrid[3][GRID_HEIGHT][GRID_WIDTH];
	extern MoveCell pathGrid[3][GRID_HEIGHT][GRID_WIDTH];
	extern CoverData coverGrid[3][GRID_HEIGHT][GRID_WIDTH];
	// extern bool highlightGrid[GRID_HEIGHT][GRID_WIDTH];
	//
	extern int currentLayer;

	extern std::vector<MoveCell> waypoints;

	void initGrids();
	void clearTerrainGrid();
	void clearMovementGrid();
	void clearUnitGrid();
	void clearPathGrid();
	void clearCoverGrid();
	CoverType calculateCoverFromTerrain(TileType terrain);
	void calculateCoverGrid();
	void addUnitToGrid(GridUnit unit);
	// void clearHighlightGrid();

	Vector3 gridToWorldPosition(GridLocation gridPos, float yLevel);


	bool isPassable(int x, int y);
	bool isGridUnitAt(int x, int y);

	float getTerrainMultiplier(int x, int y, int layer);
	float getGridUnitMultiplier(int x, int y, int layer);

 	void calculateCostsFrom(int startX, int startY, int startZ, float maxRange = 999.0f);
	std::vector<GridLocation> reconstructPath(int fromX, int fromY, int fromZ, int toX, int toY, int toZ);

	float getMovementCost(int fromX, int fromY, int fromZ, int toX, int toY, int toZ);
	float getMovementCost(const GridUnit* unit, int toX, int toY, int toZ);
	bool isReachable(int fromX, int fromY, int fromZ, int toX, int toY, int toZ, float maxMovement);
	PathData getPathInfo(int fromX, int fromY, int fromZ, int toX, int toY, int toZ, float maxMovement = 999.0f);	
	int checkMoveDistance(int x, int y, int layer);
	PathData calculateWaypointPath(const GridUnit* unit, GridLocation finalDestination);

	void setMovementDisplayFull(GridUnit* unit);
	void setMovementDisplayFull(int fromX, int fromY, int fromZ, float remainingScootRange, float remainingDashRange);
	void setMovementDisplayDash(GridUnit* unit);	
	void setMovementDisplayDash(int fromX, int fromY, int fromZ, float remainingDashRange);

	void drawHoverHighlight(int x, int y, Vector3 worldOrigin, Color hoverColor);
	void drawTerrain(Vector3 worldOrigin);
	void drawMovementOverlay(Vector3 worldOrigin);
	void drawPathPreview(std::vector<Vector2> path, Color color);
}
