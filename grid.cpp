
#include "grid.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <raymath.h>

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;

}
std::ostream& operator<<(std::ostream& os, const std::vector<Vector2>& v) {
	for (auto& coord : v ){
		os << "(" << coord.x << ", " << coord.y << ")\n";
	}
	return os;
}
namespace TacticalGrid {
	TileType terrainGrid[GRID_HEIGHT][GRID_WIDTH];
	int unitGrid[GRID_HEIGHT][GRID_WIDTH];
	int movementGrid[GRID_HEIGHT][GRID_WIDTH];
	MoveCell pathGrid[GRID_HEIGHT][GRID_WIDTH];
	CoverData coverGrid[GRID_HEIGHT][GRID_WIDTH];
	std::vector<Unit> units;
	std::vector<MoveCell> waypoints;

	void initGrids(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				terrainGrid[y][x] = TILE_EMPTY;
				unitGrid[y][x] = -1;
				movementGrid[y][x] = -1;
				pathGrid[y][x] = {-1.0f, {-1, -1}};
				coverGrid[y][x] = {COVER_NONE, COVER_NONE, COVER_NONE, COVER_NONE,};
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
		terrainGrid[4][3] = TILE_BOX;
		terrainGrid[3][2] = TILE_BOX;
		terrainGrid[17][21] = TILE_ROCK;
		terrainGrid[9][6] = TILE_BOX;
	
		units.clear();
		units.push_back({0, {5,5}, 6.0f, 5, 30, 30, RED, false, 2, 1, false});
		units.push_back({1, {4,6}, 6.0f, 5, 28, 32, GREEN, false, 2, 1, false});
		units.push_back({2, {5,7}, 7.0f, 3, 33, 25, BLUE, false, 2, 1, false});

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
				movementGrid[y][x] = -1;
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

	void clearPathGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				pathGrid[y][x] = {-1.0f, {-1, -1}};
			}
		}
	}

	void clearCoverGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				coverGrid[y][x] = {COVER_NONE, COVER_NONE, COVER_NONE, COVER_NONE};
			}
		}
	}

	CoverType calculateCoverFromTerrain(TileType terrain){
		switch (terrain){
			case TILE_WALL: return COVER_FULL;
			case TILE_TREE: return COVER_FULL;
			case TILE_EMPTY: return COVER_NONE;
			case TILE_ROCK: return COVER_HALF;
			case TILE_BOX: return COVER_HALF;
		}
	}
	void calculateCoverGrid(){
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				//check each direction (only if in bounds)
				//north
				if (y > 0){
					TileType northTerrain = terrainGrid[y-1][x];	
					CoverType coverVal = calculateCoverFromTerrain(northTerrain);
					coverGrid[y][x].north = coverVal;
				}
				//south
				if (y < GRID_HEIGHT-1){
					TileType southTerrain = terrainGrid[y+1][x];	
					CoverType coverVal = calculateCoverFromTerrain(southTerrain);
					coverGrid[y][x].south = coverVal;
				}
				//east
				if (x < GRID_WIDTH - 1){
					TileType eastTerrain = terrainGrid[y][x+1];	
					CoverType coverVal = calculateCoverFromTerrain(eastTerrain);
					coverGrid[y][x].east = coverVal;
				}
				//west
				if (x > 0){
					TileType westTerrain = terrainGrid[y][x-1];	
					CoverType coverVal = calculateCoverFromTerrain(westTerrain);
					coverGrid[y][x].west = coverVal;
				}
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
	
	Unit* getUnitById(int unitId){
		for (auto& unit : units){
			if (unitId == unit.id){
				return &unit;
			}
		}
		return nullptr;
	}
	Unit* getNextUnit(int currentId){
		int temp;
		temp = currentId + 1;
		if (temp > units.size() - 1){
			return getUnitById(0);
		}
		else {
			return getUnitById(temp);
		}
	}

	Unit* getUnitAt(int x, int y){
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return nullptr;
		int unitId = unitGrid[y][x];
		if (unitId == -1) return nullptr;
		
		return getUnitById(unitId);
	}

	bool isUnitAt(int x, int y){
		return getUnitAt(x, y) != nullptr;
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

     	void calculateCostsFrom(int startX, int startY, float maxRange)  {
		clearPathGrid();
		
		std::priority_queue<std::pair<float, std::pair<int, int>>, 
				   std::vector<std::pair<float, std::pair<int,int>>>, 
				   std::greater<>> pq;

		pathGrid[startY][startX].cost = 0.0f;
		pathGrid[startY][startX].parent = {(float)startX, (float)startY};
		pq.push({0.0f, {startX, startY}});

		int dx[] = {-1, -1, -1,  0,  0,  1,  1,  1};
		int dy[] = {-1,  0,  1, -1,  1, -1,  0,  1};
		float costs[] = {1.414f, 1.0f, 1.414f, 1.0f, 1.0f, 1.414f, 1.0f, 1.414f};

		while (!pq.empty()) {
		    float currentCost = pq.top().first;
		    int currentX = pq.top().second.first;
		    int currentY = pq.top().second.second;
		    pq.pop();

		    if (currentCost > pathGrid[currentY][currentX].cost) continue;

		    for (int i = 0; i < 8; i++) {
			int neighborX = currentX + dx[i];
			int neighborY = currentY + dy[i];
			
			if (neighborX < 0 || neighborX >= GRID_WIDTH || 
			    neighborY < 0 || neighborY >= GRID_HEIGHT) continue;

			float terrainMultiplier = getTerrainMultiplier(neighborX, neighborY);
			if (terrainMultiplier < 0) continue;
			
			float unitMultiplier = getUnitMultiplier(neighborX, neighborY); 
			if (unitMultiplier < 0) continue;
		    
			float moveCost = costs[i] * terrainMultiplier;
			float newCost = currentCost + moveCost;
			
			if (newCost > maxRange) continue;

			if (pathGrid[neighborY][neighborX].cost < 0 || 
			    newCost < pathGrid[neighborY][neighborX].cost) {
			    
			    pathGrid[neighborY][neighborX].cost = newCost;
			    pathGrid[neighborY][neighborX].parent = {(float)currentX, (float)currentY};
			    pq.push({newCost, {neighborX, neighborY}});
			}
		    }
		}
    	}

	std::vector<Vector2> reconstructPath(int fromX, int fromY, int toX, int toY) {
		std::vector<Vector2> path;

		if (pathGrid[toY][toX].cost < 0){
			return path;
		}
		int currentX = toX;
		int currentY = toY;

		// Build path backwards from destination to start
		while (true) {
			path.push_back({(float)currentX, (float)currentY});

			Vector2 parent = pathGrid[currentY][currentX].parent;
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


	float getMovementCost(int fromX, int fromY, int toX, int toY){
		calculateCostsFrom(fromX, fromY);
		return pathGrid[toY][toX].cost;	
	}
	float getMovementCost(const Unit* unit, int toX, int toY){
		calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y);
		return pathGrid[toY][toX].cost;
	}
	bool isReachable(int fromX, int fromY, int toX, int toY, float maxMovement){
		float cost = getMovementCost(fromX, fromY, toX, toY);
		return cost >= 0 && cost <= maxMovement;
	}
	bool inDashRange(const Unit* unit, int toX, int toY){
		return isReachable(unit->gridPosition.x, unit->gridPosition.y, toX, toY, unit->speed*2);
	}
	bool inScootRange(const Unit* unit, int toX, int toY){
		return isReachable(unit->gridPosition.x, unit->gridPosition.y, toX, toY, unit->speed);
	}

	PathData getPathInfo(int fromX, int fromY, int toX, int toY, float maxMovement){
		calculateCostsFrom(fromX, fromY, maxMovement);
		
		PathData result;
		result.totalCost = pathGrid[toY][toX].cost;
		result.isReachable = isReachable(fromX, fromY, toX, toY, maxMovement);

		if (result.isReachable){
			result.path = reconstructPath(fromX, fromY, toX, toY);
		}
		
		return result;
	}	
	std::vector<Vector2> getTilesInRange(int fromX, int fromY, float maxMovement){
		calculateCostsFrom(fromX, fromY, maxMovement);

		std::vector<Vector2> tilesInRange;
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				if (pathGrid[y][x].cost >= 0 && pathGrid[y][x].cost <= maxMovement){
					tilesInRange.push_back({(float)x, (float)y});
				}
			}
		}
		return tilesInRange;
	}

	std::vector<Vector2> getScootTiles(Unit* unit){
		std::vector<Vector2> scootTiles = getTilesInRange(unit->gridPosition.x, unit->gridPosition.y, unit->speed);
		return scootTiles;
	}

	std::vector<Vector2> getDashTiles(Unit* unit){
		std::vector<Vector2> dashTiles = getTilesInRange(unit->gridPosition.x, unit->gridPosition.y, unit->speed * 1.5);
		return dashTiles;
	}

	int checkMoveDistance(int x, int y){
		return movementGrid[y][x];
	}

	void setMovementDisplayFull(Unit* unit){
		clearMovementGrid();
		calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit->speed*1.5);

		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				float cost = pathGrid[y][x].cost;
				if (cost > 0 && cost <= unit->speed){
					movementGrid[y][x] = 1;
				}
				if (cost > unit->speed && cost <= unit->speed*1.5){
					movementGrid[y][x] = 2;
				}
			}
		}

	
	}

	void setMovementDisplayFull(int fromX, int fromY, float remainingScootRange, float remainingDashRange){
		clearMovementGrid();

		calculateCostsFrom(fromX, fromY, remainingDashRange);
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				float cost = pathGrid[y][x].cost;
				if (cost > 0 && cost <= remainingScootRange){
					movementGrid[y][x] = 1;
				}
				if (cost > remainingScootRange && cost <= remainingDashRange){
					movementGrid[y][x] = 2;
				}
			}
		}
	}


	void setMovementDisplayDash(Unit* unit){
		//used to set display after step movement.
		clearMovementGrid();
		calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit->speed*0.5);
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				float cost = pathGrid[y][x].cost;
				if (cost > 0 && cost <= unit->speed*0.5){
					movementGrid[y][x] = 2;
				}
			}
		}
	}
	void setMovementDisplayDash(int fromX, int fromY, float remainingDashRange){
		//used to set display after step movement.
		clearMovementGrid();
		calculateCostsFrom(fromX, fromY, remainingDashRange);
		for (int y = 0; y < GRID_HEIGHT; y++){
			for (int x = 0; x < GRID_WIDTH; x++){
				float cost = pathGrid[y][x].cost;
				if (cost > 0 && cost <= remainingDashRange){
					movementGrid[y][x] = 2;
				}
			}
		}
	}		


	// void setMovementDisplayDash(int fromX, int fromY, float remain){
	// 	//used to set display after step movement.
	// 	clearMovementGrid();
	// 	calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit->speed*0.5);
	// 	for (int y = 0; y < GRID_HEIGHT; y++){
	// 		for (int x = 0; x < GRID_WIDTH; x++){
	// 			float cost = pathGrid[y][x].cost;
	// 			if (cost > 0 && cost <= unit->speed*0.5){
	// 				movementGrid[y][x] = 2;
	// 			}
	// 		}
	// 	}
	// }	

	void setSelectedHighlight(int unitId){
		for (auto& unit : units){
			if (unitId == unit.id){
				Vector3 pos = {unit.gridPosition.x * TILE_SIZE, 0.0f, unit.gridPosition.y * TILE_SIZE};
				DrawCubeWires(pos, TILE_SIZE, 0.1f, TILE_SIZE, SKYBLUE);
				break;
			}
		}
	}

	void drawHoverHighlight(int x, int y, Vector3 worldOrigin, Color hoverColor){
		Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y, worldOrigin.z + y * TILE_SIZE};
		DrawCubeWires(pos, TILE_SIZE, 0.15f, TILE_SIZE, hoverColor);
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
				float height;
				switch (terrainGrid[y][x]) {
					case TILE_WALL: height = 4.0f; break;
					case TILE_BOX: height = 2.0f; break;
					case TILE_TREE: height = 4.0f; break;
					case TILE_ROCK: height = 2.0f; break;
				}
				DrawCube(terrainPos, 2.0f, height, 2.0f, color);
				
				// DrawCubeWires(wirePos, TILE_SIZE, 0.1f, TILE_SIZE, BLACK);
			}
		
		}
	}

	PathData calculateWaypointPath(const Unit* unit, Vector2 finalDestination) {
		PathData result;
		result.totalCost = 0.0f;
		result.isReachable = true;
		
		Vector2 currentPos = unit->gridPosition;
		float remainingMovement = unit->speed*1.5;
		
		// Path to first waypoint
		if (!waypoints.empty()) {
		    PathData segmentResult = getPathInfo((int)currentPos.x, (int)currentPos.y, 
							 (int)waypoints[0].parent.x, (int)waypoints[0].parent.y, remainingMovement);
		    if (!segmentResult.isReachable) {
			result.isReachable = false;
			return result;
		    }
		    
		    result.path = segmentResult.path;
		    result.totalCost += segmentResult.totalCost;
		    remainingMovement -= segmentResult.totalCost;
		    currentPos = waypoints[0].parent;
		}
		
		// Path through waypoints
		for (size_t i = 1; i < waypoints.size(); i++) {
		    PathData segmentResult = getPathInfo((int)currentPos.x, (int)currentPos.y, 
							 (int)waypoints[i].parent.x, (int)waypoints[i].parent.y, remainingMovement);
		    if (!segmentResult.isReachable) {
			result.isReachable = false;
			return result;
		    }
		    
		    // Remove first element to avoid duplicates
		    if (!segmentResult.path.empty()) {
			segmentResult.path.erase(segmentResult.path.begin());
			result.path.insert(result.path.end(), segmentResult.path.begin(), segmentResult.path.end());
		    }
		    
		    result.totalCost += segmentResult.totalCost;
		    remainingMovement -= segmentResult.totalCost;
		    currentPos = waypoints[i].parent;
		}
		
		// Path to final destination
		PathData finalSegment = getPathInfo((int)currentPos.x, (int)currentPos.y, 
						    (int)finalDestination.x, (int)finalDestination.y, remainingMovement);
		if (!finalSegment.isReachable) {
		    result.isReachable = false;
		    return result;
		}
		
		if (!finalSegment.path.empty()) {
		    finalSegment.path.erase(finalSegment.path.begin());
		    result.path.insert(result.path.end(), finalSegment.path.begin(), finalSegment.path.end());
		}
		
		result.totalCost += finalSegment.totalCost;
		
		return result;
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
				int cost = movementGrid[y][x];
				if (cost == 1){
					Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y , worldOrigin.z + y * TILE_SIZE};
					DrawCube(pos, 0.5f, 0.05f, 0.5f, SKYBLUE);

				}
				if (cost == 2){
					Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y , worldOrigin.z + y * TILE_SIZE};
					DrawCube(pos, 0.5f, 0.05f, 0.5f, GOLD);

				}
			}
		}
	}

	void drawPathPreview(std::vector<Vector2> path, Color color){
		if (path.size() < 3){
			return;
		}

		auto from = path.begin();
		auto to = from;
		to++;

		while (to != path.end()){
			Vector3 start = gridToWorldPosition(*from, 0.0f);
			Vector3 end = gridToWorldPosition(*to, 0.0f);

			auto temp = to;
			temp++;
			if (temp == path.end()){
				Vector3 direction = Vector3Normalize(Vector3Subtract(end, start));
				end = Vector3Subtract(end, Vector3Scale(direction, TILE_SIZE / 2.0f));
			}
			DrawLine3D(start, end, color);
			from++;
			to++;
		}
		
	}

}

