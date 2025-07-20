
#include "grid.hpp"

namespace TacticalGrid {
	TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];

	void initGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
			}
		}
		
		//Add some more interesting terrain
		terrainGrid[1][2] = TILE_WALL;
		terrainGrid[1][3] = TILE_WALL;
		terrainGrid[1][4] = TILE_WALL;
		terrainGrid[1][5] = TILE_WALL;
		terrainGrid[1][6] = TILE_WALL;
		terrainGrid[10][10] = TILE_TREE;
		terrainGrid[11][11] = TILE_TREE;
		terrainGrid[10][11] = TILE_TREE;
		terrainGrid[11][10] = TILE_TREE;
		terrainGrid[20][5] = TILE_ROCK;
		terrainGrid[14][26] = TILE_ROCK;
		terrainGrid[4][2] = TILE_BOX;
		terrainGrid[17][21] = TILE_ROCK;
		terrainGrid[9][6] = TILE_BOX;
	}

	void clearTerrainGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
			}
		}

	}

	bool isPassable(int x, int y){
		if (x < 0 || x <= GRID_WIDTH || y < 0 || y <= GRID_HEIGHT){
			return false;
		}
		if (terrainGrid[y][x] != TILE_EMPTY){
			return false;
		} else {
			return true;
		}
	}

	void drawTerrain(Vector3 worldOrigin){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				Vector3 terrainPos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y + 1.0f, worldOrigin.z + y * TILE_SIZE};
				Vector3 wirePos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y, worldOrigin.z + y * TILE_SIZE};

				if (terrainGrid[y][x] == TILE_EMPTY){
					DrawCubeWires(wirePos, TILE_SIZE, 0.1f, TILE_SIZE, BLACK);
					continue;
				}  

				Color color;	
				switch (terrainGrid[y][x]) {
					case TILE_WALL: color = DARKBROWN; break;
					case TILE_BOX: color = ORANGE; break;
					case TILE_TREE: color = DARKGREEN; break;
					case TILE_ROCK: color = DARKGRAY; break;
				}

				DrawCube(terrainPos, 2.0f, 2.0f, 2.0f, color);
				// DrawCubeWires(wirePos, TILE_SIZE, 0.1f, TILE_SIZE, BLACK);
			}
		
		}
	}
}

