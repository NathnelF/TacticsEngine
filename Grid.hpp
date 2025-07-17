
#ifndef GRID_H
#define GRID_H

#include "Tile.hpp"
#include <raylib.h>
#include <list>
#include "config.hpp"
#include "utils.hpp"

struct TileEdge {
    Tile tile;
    enum Direction { TOP, RIGHT, BOTTOM, LEFT } direction;
};

class Grid{
private:
	std::vector<std::vector <Tile>> tiles;



public:
	Grid(int gridWidth, int gridHeight);

	bool isValid(int x, int y);

	Tile& getTile(int x, int y);

	Tile* getTilePointer(int x, int y);

	Tile* getTileFromWorld(Vector3 worldPos);

	std::vector<Tile> getGridNeighbors(Tile t);
	std::list<Tile> getPath(Tile start, Tile end);
	std::list<Tile> getWaypointPath(Tile start, const std::vector<Tile>& waypoints);

	void RenderGrid();
	void RenderWaypoints(const Tile& waypoint, Color color);
	void RenderPath(const std::list<Tile>&, Color color);
	std::vector<TileEdge> getMovementRangeOutline(const std::vector<Tile>& movementRange);
	void RenderOutline(const std::vector<TileEdge>& outline);
};

#endif


