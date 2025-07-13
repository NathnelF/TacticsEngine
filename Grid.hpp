#include "Tile.hpp"
#include <raylib.h>


class Grid{
private:
	std::vector<std::vector <Tile>> tiles;


public:
	Grid(int gridWidth, int gridHeight);

	bool isValid(int x, int y);

	Tile& getTile(int x, int y);

	std::vector<Tile> getGridNeighbors(Tile t);

	void RenderGrid();

};
