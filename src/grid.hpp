
#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "cover.hpp"

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

	//grid stuff
	int id;
	Vector2 gridPosition;

	//stats
	float speed;
	int health;
	int aim;
	int resolve;

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
	Vector2 parent;
};

struct PathData {
	std::vector<Vector2> path;
	float totalCost;
	bool isReachable;
}; 


std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::ostream& operator<<(std::ostream& os, const Vector2& v);
std::ostream& operator<<(std::ostream& os, const std::vector<Vector2>& v);

namespace TacticalGrid {
	extern TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	extern int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	extern int movementGrid[GRID_HEIGHT][GRID_WIDTH];
	extern MoveCell pathGrid[GRID_HEIGHT][GRID_WIDTH];
	extern CoverData coverGrid[GRID_HEIGHT][GRID_WIDTH];
	// extern bool highlightGrid[GRID_HEIGHT][GRID_WIDTH];

	extern std::vector<Unit> units; 
	extern std::vector<MoveCell> waypoints;

	void initGrids();
	void clearTerrainGrid();
	void clearMovementGrid();
	void clearUnitGrid();
	void clearPathGrid();
	void clearCoverGrid();
	CoverType calculateCoverFromTerrain(TileType terrain);
	void calculateCoverGrid();
	// void clearHighlightGrid();

	Vector3 gridToWorldPosition(Vector2 gridPos, float yLevel);


	bool isPassable(int x, int y);
	Unit* getUnitAt(int x, int y);
	bool isUnitAt(int x, int y);
	void moveUnit(int unitId, int newX, int newY);
	Unit* getUnitById(int unitId);
	Unit* getNextUnit(int currentId);

	float getTerrainMultiplier(int x, int y);
	float getUnitMultiplier(int x, int y);

     	void calculateCostsFrom(int startX, int startY, float maxRange = 999.0f);
	std::vector<Vector2> reconstructPath(int fromX, int fromY, int toX, int toY);

	float getMovementCost(int fromX, int fromY, int toX, int toY);
	float getMovementCost(const Unit* unit, int toX, int toY);
	bool isReachable(int fromX, int fromY, int toX, int toY, float maxMovement);
	bool inScootRange(const Unit* unit, int toX, int toY);
	bool inDashRange(const Unit* unit, int toX, int toY);
	PathData getPathInfo(int fromX, int fromY, int toX, int toY, float maxMovement = 999.0f);	
	std::vector<Vector2> getTilesinRange(int fromX, int fromY, float maxMovement);
	std::vector<Vector2> getScootTiles(Unit* unit);
	std::vector<Vector2> getDashTiles(Unit* unit);
	int checkMoveDistance(int x, int y);
	PathData calculateWaypointPath(const Unit* unit, Vector2 finalDestination);
	void setSelectedHighlight(int unitId);

	void setMovementDisplayFull(Unit* unit);
	void setMovementDisplayFull(int fromX, int fromY, float remainingScootRange, float remainingDashRange);
	void setMovementDisplayDash(Unit* unit);	
	void setMovementDisplayDash(int fromX, int fromY, float remainingDashRange);

	void drawHoverHighlight(int x, int y, Vector3 worldOrigin, Color hoverColor);
	void drawTerrain(Vector3 worldOrigin);
	void drawUnits(Vector3 worldOrigin);
	void drawMovementOverlay(Vector3 worldOrigin);
	void drawPathPreview(std::vector<Vector2> path, Color color);
}
