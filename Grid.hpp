
#ifndef GRID_H
#define GRID_H

#include "Tile.hpp"
#include <raylib.h>
#include <list>
#include "config.hpp"
#include "utils.hpp"
#include <set>
#include <iostream>
#include <unordered_map>


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

	std::vector<TileEdge> getMovementRangeOutline(const std::unordered_map<Tile, TileNode, TileHash>& movementRange, float speed);
	std::vector<TileEdge::Direction> getOppositeDirections(TileEdge::Direction direction);
	TileEdge::Direction getDirectOppositeEdge(TileEdge::Direction direction);
	float checkNeighborCost(const TileNode& start, Tile& neighbor, bool ignoreImpassable);

	void RenderPathRange(const std::vector<Tile>&, Color color);
	void RenderGrid();
	void RenderWaypoints(const Tile& waypoint, Color color);
	void RenderOutline(const std::vector<TileEdge>& outline);

};

#endif


