
#ifndef GRID_H
#define GRID_H

#include "Tile.hpp"
#include <raylib.h>
#include <list>
#include "config.hpp"
#include "utils.hpp"
#include <set>
#include <iostream>

struct TileEdge {
	Tile tile;
	enum Direction { TOP, RIGHT, BOTTOM, LEFT } direction;

	friend std::ostream &operator<<(std::ostream &os, const TileEdge &tileEdge){
		switch (tileEdge.direction) {
			case Direction::TOP:
				return os << "Top edge of tile at ( " << tileEdge.tile.gridPosition.x << " , " << tileEdge.tile.gridPosition.y << ")\n";
			case Direction::RIGHT:
				return os << "Right edge of tile at ( " << tileEdge.tile.gridPosition.x << " , " << tileEdge.tile.gridPosition.y << ")\n";

			case Direction::BOTTOM:
				return os << "Bottom edge of tile at ( " << tileEdge.tile.gridPosition.x << " , " << tileEdge.tile.gridPosition.y << ")\n";
			case Direction::LEFT:
				return os << "Left edge of tile at ( " << tileEdge.tile.gridPosition.x << " , " << tileEdge.tile.gridPosition.y << ")\n";
			default:
				return os << "Unknown edge of tile at ( " << tileEdge.tile.gridPosition.x << " , " << tileEdge.tile.gridPosition.y << ")\n";
		}
	}
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
	std::list<Tile> getWaypointPath(Tile start, const std::vector<Tile>& waypoints, Tile& hoveredTile);

	void RenderGrid();
	void RenderWaypoints(const Tile& waypoint, Color color);
	void RenderPath(const std::list<Tile>&, Color color);
	std::vector<TileEdge> getMovementRangeOutline(const std::vector<Tile>& movementRange);
	std::vector<TileEdge::Direction> getOppositeDirections(TileEdge::Direction direction);
	TileEdge::Direction getDirectOppositeEdge(TileEdge::Direction direction);
	void RenderOutline(const std::vector<TileEdge>& outline);
};

#endif


