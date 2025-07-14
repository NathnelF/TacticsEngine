
#ifndef GRID_H
#define GRID_H

#include "Tile.hpp"
#include <raylib.h>
#include <list>
#include "config.hpp"


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

	void RenderGrid();

};

#endif


