#include "Grid.hpp"
#include <stdexcept>

Grid::Grid(int gridWidth, int gridHeight):
	tiles(gridWidth, std::vector<Tile>(gridHeight))
{
	for (int x = 0; x < gridWidth; x++){
		for (int y = 0; y < gridHeight; y++){
			tiles[x][y].height = 0.0f; //keep flat for now
			tiles[x][y].gridPosition = (Vector2){(float)x, (float)y};
			tiles[x][y].worldPosition = (Vector3){
				(float)x * TILE_SIZE - TILE_SIZE / 2,
				0.0f,
				(float)y * TILE_SIZE - TILE_SIZE / 2
			};
			tiles[x][y].traversable = true;
			tiles[x][y].hasUnit = false;
			tiles[x][y].color = tileDefaultColor;
			tiles[x][y].bounds = {
				(Vector3){(float)x * TILE_SIZE - TILE_SIZE, 0.0f, (float)y * TILE_SIZE - TILE_SIZE},
				(Vector3){(float)x * TILE_SIZE, 0.0f, (float)y * TILE_SIZE},
			};	
		}
	}
}

bool Grid::isValid(int x, int y){
	if (x >= GRID_WIDTH || y >= GRID_HEIGHT){
		return false;
	} else {
		return true;
	}
}

Tile& Grid::getTile(int x, int y){
	if (!isValid(x, y)) {
        // If out of bounds, throw an exception
        throw std::out_of_range("Grid::getTile() coordinates (" +
                                std::to_string(x) + ", " + std::to_string(y) +
                                ") are out of bounds.");
     	}
	return tiles[x][y];
}

std::vector<Tile> Grid::getGridNeighbors(Tile t){
	Vector2 operations[4] = { (Vector2){0, 1}, (Vector2){1, 0}, (Vector2){0, -1}, (Vector2){-1, 0}}; //for diags (Vector2){1,1} (Vector2){1, -1} (Vector2){-1, 1} (Vector2){-1,-1}
	Vector2 gridPos = t.gridPosition;
	std::vector<Tile> neighbors;
	for (int i = 0; i < 4; i++){
		Vector2 neighborPos = Vector2Add(gridPos, operations[i]);
      		if (isValid(neighborPos.x, neighborPos.y)){
      			neighbors.push_back(getTile(neighborPos.x, neighborPos.y));	
      		}
	}
	return neighbors;

}

void Grid::RenderGrid(){
	  for (int x = 0; x < GRID_WIDTH; x++ ){
		for (int y = 0; y < GRID_HEIGHT; y++){
			Tile& currentTile = getTile(x,y);
			currentTile.DrawTile();
		}
	}
}
