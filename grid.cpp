
#include "grid.hpp"
#include <queue>
#include <iostream>

namespace TacticalGrid {
	TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	bool movementGrid[GRID_HEIGHT][GRID_WIDTH];

	std::vector<Unit> units;

	void initGrids(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
				unitGrid[y][x] = -1;
				movementGrid[y][x] = false;
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
	
		units.clear();
		units.push_back({1, {5,5}, 6.0f, 5, 30, 30, RED});
		units.push_back({2, {4,6}, 6.0f, 5, 28, 32, GREEN});
		units.push_back({3, {5,7}, 7.0f, 3, 33, 25, BLUE});

		for (auto& unit : units){
			unitGrid[(int)unit.gridPosition.y][(int)unit.gridPosition.x] = unit.id;
		}
	}

	void clearTerrainGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
			}
		}

	}

	void clearMovementGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				movementGrid[y][x] = false;
			}
		}
	}
	void clearUnitGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				unitGrid[y][x] = -1;
			}
		}
	}

	bool isPassable(int x, int y){
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT){
			return false;
		}
		if (terrainGrid[y][x] == TILE_EMPTY){
			return true;
		} else {
			return false;
		}
	}

	Unit* getUnitAt(int x, int y){
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return nullptr;
		int unitId = unitGrid[y][x];
		if (unitId == -1) return nullptr;
		
		for (auto& unit : units){
			if (unitId == unit.id) return &unit;
		}
		return nullptr;
	}

	bool isUnitAt(int x, int y){
		return getUnitAt(x, y) != nullptr;
	}
	
	void moveUnit(int unitId, int newX, int newY){
		if (!isPassable(newX, newY)) return;
		
		for (auto& unit : units){
			if (unitId == unit.id){
				unitGrid[(int)unit.gridPosition.y][(int)unit.gridPosition.x] = -1;

				unit.gridPosition = {(float)newY, (float)newX};
				unitGrid[newY][newX] = unitId;
				break;
			}
		}
	}

	void calculateMovementRange(int unitId) {
		clearMovementGrid();
		
		Unit* unit = nullptr;
		for (auto& u : units) {
		    if (u.id == unitId) {
			unit = &u;
			break;
		    }
		}
		if (!unit) return;
		
		// Simple flood-fill for movement range
		std::queue<std::pair<Vector2, int>> toVisit;
		bool visited[GRID_HEIGHT][GRID_WIDTH] = {false};
		
		int startX = (int)unit->gridPosition.x;
		int startY = (int)unit->gridPosition.y;
		
		toVisit.push({{(float)startX, (float)startY}, 0});
		visited[startY][startX] = true;
		
		int dx[] = {0, 1, 0, -1};
		int dy[] = {-1, 0, 1, 0};
		
		while (!toVisit.empty()) {
		    auto current = toVisit.front();
		    toVisit.pop();
		    
		    Vector2 pos = current.first;
		    int distance = current.second;
		    
		    if (distance < unit->speed) {
			for (int i = 0; i < 4; i++) {
			    int newX = (int)pos.x + dx[i];
			    int newY = (int)pos.y + dy[i];
			    
			    if (newX >= 0 && newX < GRID_WIDTH && newY >= 0 && newY < GRID_HEIGHT 
				&& !visited[newY][newX]) {
				visited[newY][newX] = true;
				
				if (isPassable(newX, newY) || (newX == startX && newY == startY)) {
				    movementGrid[newY][newX] = true;
				    toVisit.push({{(float)newX, (float)newY}, distance + 1});
				}
			    }
			}
		    }
		}
		
		// Don't highlight current position
		movementGrid[startY][startX] = false;
        }


	void setHighlight(int unitId){
		for (auto& unit : units){
			if (unitId == unit.id){
				Vector3 Pos = {unit.gridPosition.x * TILE_SIZE, 0.0f, unit.gridPosition.y * TILE_SIZE};
				DrawCubeWires(Pos, TILE_SIZE, 0.1f, TILE_SIZE, GOLD);
				break;
			}
		}
	}

	void printMovementGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				std::cout << movementGrid[y][x] << std::endl;
			}
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
	
	void drawUnits(Vector3 worldOrigin){
		for (auto& unit : units){
			Vector3 unitPos = {worldOrigin.x + unit.gridPosition.x * TILE_SIZE, worldOrigin.y + 1.0f, worldOrigin.z + unit.gridPosition.y * TILE_SIZE};
			DrawCube(unitPos, 1.5f, 1.0f, 1.5f, unit.color);
		}
	}
	
	void drawMovementOverlay(Vector3 worldOrigin){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				if (movementGrid[y][x]) {
					Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y, worldOrigin.z + y * TILE_SIZE};
					DrawCube(pos, 0.5f, 0.05f, 0.5f, SKYBLUE);
				}
			}
		}
	}

}

