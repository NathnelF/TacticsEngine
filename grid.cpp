
#include "grid.hpp"
#include <queue>
#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;

}
namespace TacticalGrid {
	TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	MoveCell movementGrid[GRID_HEIGHT][GRID_WIDTH];

	std::vector<Unit> units;

	void initGrids(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
				unitGrid[y][x] = -1;
				movementGrid[y][x] = {-1.0f, {-1, -1}};
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
		units.push_back({1, {5,5}, 6.0f, 5, 30, 30, RED, false});
		units.push_back({2, {4,6}, 6.0f, 5, 28, 32, GREEN, false});
		units.push_back({3, {5,7}, 7.0f, 3, 33, 25, BLUE, false});

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
				movementGrid[y][x] = {-1.0f, {-1, -1}};
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

	Vector3 gridToWorldPosition(Vector2 gridPos, float yLevel){
		Vector3 worldPos = {gridPos.x * TILE_SIZE, yLevel, gridPos.y * TILE_SIZE};
		return worldPos;
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

	bool inRange(int x, int y, float range){
		if (movementGrid[y][x].cost < 0.0f || movementGrid[y][x].cost > range){
			return false;	
		} else {
			return true;
		}
	}

	
	float getTerrainMultiplier(int x, int y){
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return -1.0f;
		TileType terrainType = terrainGrid[y][x];
		if (terrainType == TILE_EMPTY) {
			return 1.0f;
		} else {
			return -1.0f;
		}
	}

	float getUnitMultiplier(int x, int y){
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return -1.0f;
		if (isUnitAt(x,y)){
			return -1.0f;
		} else {
			return 1.0f;
		}
	}
	
	void moveUnit(int unitId, int newX, int newY){
		//TBD	
	}

	void calculateMovementRange(int unitId) {
		clearMovementGrid();

		Unit* unit = nullptr;
		for (auto& u : units){
			if (unitId == u.id){
				unit = &u;
				break;
			}
		}
		if (!unit) return;

		int startX = (int)unit->gridPosition.x;
		int startY = (int)unit->gridPosition.y;

		std::priority_queue<std::pair<float, std::pair<int, int>>, std::vector<std::pair<float, std::pair<int,int>>>, std::greater<>> pq;

		movementGrid[startY][startX].cost = 0.0f;
		movementGrid[startY][startX].parent = unit->gridPosition;
		pq.push({0.0f, {startX, startY}});

		int dx[] = {-1, -1, -1,  0,  0,  1,  1,  1};
		int dy[] = {-1,  0,  1, -1,  1, -1,  0,  1};
		float costs[] = {
		    //  NW,  N, NE,  W,  E, SW,  S, SE
		    1.414f, 1.0f, 1.414f, 1.0f, 1.0f, 1.414f, 1.0f, 1.414f
		};
		while (!pq.empty()){
			float currentCost = pq.top().first;
			int x = pq.top().second.first;
			int y = pq.top().second.second;
			pq.pop();

			//check if the current cost is greater than speed.
			if (currentCost > movementGrid[y][x].cost) continue;

			//check neighbors
			for (int i = 0; i < 8; i++) {
				int neighborX = x + dx[i];
				int neighborY = y + dy[i];
				if (neighborX < 0 || neighborX >= GRID_WIDTH || neighborY < 0 || neighborY >= GRID_HEIGHT) continue;

				float terrainMultiplier = getTerrainMultiplier(neighborX,neighborY);
				if (terrainMultiplier < 0) continue;
				float unitMultiplier = getUnitMultiplier(neighborX,neighborY); 
				if (unitMultiplier < 0) continue;
			
				float moveCost = costs[i] * terrainMultiplier;
				float newCost = currentCost + moveCost;
				
				if (newCost > unit->speed){
					continue;
				}

				if (movementGrid[neighborY][neighborX].cost < 0 || newCost < movementGrid[neighborY][neighborX].cost){
					movementGrid[neighborY][neighborX].cost = newCost;
					movementGrid[neighborY][neighborX].parent = {(float)x, (float)y};
					pq.push({newCost, {neighborX, neighborY}});
				}
				
			} 

		}
		movementGrid[startY][startX].cost = -1.0f;
	
        }

	std::vector<Vector2> reconstructPath(int fromX, int fromY, int toX, int toY) {
		std::vector<Vector2> path;

		// Check if destination is reachable
		if (movementGrid[toY][toX].cost < 0 || movementGrid[toY][toX].cost > 7.0f) {
			return path; // Empty path = unreachable
		}

		// Reconstruct path by following parent pointers
		int currentX = toX;
		int currentY = toY;

		// Build path backwards from destination to start
		while (true) {
			path.push_back({(float)currentX, (float)currentY});

			Vector2 parent = movementGrid[currentY][currentX].parent;
			int parentX = (int)parent.x;
			int parentY = (int)parent.y;

			// Check if we reached the start (parent points to itself)
			if (parentX == currentX && parentY == currentY) {
				break;
			}

			// Safety check to prevent infinite loops
			if (parentX < 0 || parentX >= GRID_WIDTH || parentY < 0 || parentY >= GRID_HEIGHT) {
				break;
			}

			currentX = parentX;
			currentY = parentY;
		}

		// Reverse the path so it goes from start to destination
		std::reverse(path.begin(), path.end());

		return path;
	}


	void setSelectedHighlight(int unitId){
		for (auto& unit : units){
			if (unitId == unit.id){
				Vector3 pos = {unit.gridPosition.x * TILE_SIZE, 0.0f, unit.gridPosition.y * TILE_SIZE};
				DrawCubeWires(pos, TILE_SIZE, 0.1f, TILE_SIZE, SKYBLUE);
				break;
			}
		}
	}

	void drawHoverHighlight(int x, int y, Vector3 worldOrigin){
		Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y, worldOrigin.z + y * TILE_SIZE};
		DrawCubeWires(pos, TILE_SIZE, 0.15f, TILE_SIZE, GOLD);
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
			if (!unit.isMoving){
				Vector3 unitPos = {worldOrigin.x + unit.gridPosition.x * TILE_SIZE, worldOrigin.y + 1.0f, worldOrigin.z + unit.gridPosition.y * TILE_SIZE};
				DrawCube(unitPos, 1.5f, 1.0f, 1.5f, unit.color);
			}
				
		}
	}
	
	void drawMovementOverlay(Vector3 worldOrigin){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				float cost = movementGrid[y][x].cost;
				if (cost > 0 && cost < 7.0f){
					Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y , worldOrigin.z + y * TILE_SIZE};
					DrawCube(pos, 0.5f, 0.05f, 0.5f, SKYBLUE);

				}
			}
		}
	}

}

