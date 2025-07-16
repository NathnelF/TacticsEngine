
#include "Grid.hpp"
#include <stdexcept>
#include <queue>
#include <set>
#include <map>
#include <iostream>

Grid::Grid(int gridWidth, int gridHeight):
	tiles(gridHeight, std::vector<Tile>(gridWidth))
{
	for (int row = 0; row < gridHeight; row++){
		for (int col = 0; col < gridWidth; col++){
			tiles[row][col].height = 0.0f; //keep flat for now
			tiles[row][col].gridPosition = (Vector2){(float)col, (float)row};
			tiles[row][col].worldPosition = (Vector3){
				(float)col * TILE_SIZE - TILE_SIZE / 2,
				1.2f,
				(float)row * TILE_SIZE - TILE_SIZE / 2
			};
			tiles[row][col].traversable = true;
			tiles[row][col].hasUnit = false;
			tiles[row][col].color = tileDefaultColor;
			tiles[row][col].bounds = {
				(Vector3){(float)col * TILE_SIZE - TILE_SIZE, 0.0f, (float)row * TILE_SIZE - TILE_SIZE},
				(Vector3){(float)col * TILE_SIZE, 0.0f, (float)row * TILE_SIZE},
			};	
		}
	}
}

bool Grid::isValid(int x, int y){
	return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

Tile& Grid::getTile(int x, int y){
	if (!isValid(x, y)) {
        // If out of bounds, throw an exception
        throw std::out_of_range("Grid::getTile() coordinates (" +
                                std::to_string(x) + ", " + std::to_string(y) +
                                ") are out of bounds.");
     	}
	return tiles[y][x];
}

Tile* Grid::getTilePointer(int x, int y){
	if (!isValid(x,y)){
		throw std::out_of_range("Grid::getTile() coordinates (" +
                                std::to_string(x) + ", " + std::to_string(y) +
                                ") are out of bounds.");
	}
	return &tiles[y][x];
}

std::vector<Tile> Grid::getGridNeighbors(Tile t){
	Vector2 operations[8] = { (Vector2){0, 1}, (Vector2){1, 0}, (Vector2){0, -1}, (Vector2){-1, 0}, (Vector2){1,1}, (Vector2){1, -1}, (Vector2){-1, 1}, (Vector2){-1,-1}}; //for diags 
	Vector2 gridPos = t.gridPosition;
	std::vector<Tile> neighbors;
	for (int i = 0; i < 8; i++){
		Vector2 neighborPos = Vector2Add(gridPos, operations[i]);
      		if (isValid(neighborPos.x, neighborPos.y)){
			if (getTile(neighborPos.x, neighborPos.y).traversable && !getTile(neighborPos.x, neighborPos.y).hasUnit){
				neighbors.push_back(getTile(neighborPos.x, neighborPos.y));	
			}
      			
      		}
	}
	return neighbors;

}

std::list<Tile> Grid::getPath(Tile start, Tile end){
	std::queue<Tile> queue;
	std::set<Tile> visited;
	std::map<Tile, Tile> parentMap;
	std::list<Tile> path;
	bool pathFound = false;

	if (end.hasUnit || !end.traversable){
		// std::cout << "Tile is not available!" << std::endl;
		return path;
	}

	queue.push(start);
	visited.insert(start);
	while (!queue.empty()){
		Tile current = queue.front();
		queue.pop();

		if (current.gridPosition == end.gridPosition){
			pathFound = true;
			break;
		}

		std::vector<Tile> neighbors = getGridNeighbors(current);
		for (Tile neighbor : neighbors){
			if (visited.find(neighbor) == visited.end()){
				queue.push(neighbor);
				visited.insert(neighbor);
				parentMap[neighbor] = current;
			}
		}
	}
	if (pathFound == false){
		// std::cout << "No path to target\n";
		return path;
	}
	else {
		Tile current = end;
		while (current.gridPosition != start.gridPosition){
			path.push_front(current);
			current = parentMap[current];
		}
		path.push_front(start);
		// std::cout << "Found path to target\n";
		return path;
	}
	
}  

std::list<Tile> Grid::getWaypointPath(Tile start, const std::vector<Tile>& waypoints){
	std::list<Tile> path;
	if (waypoints.empty()) return path;

	for (const auto& point : waypoints){
		std::list<Tile> curPath = getPath(start, point);
		std::cout << "checking path\n";

		if (curPath.empty()){
			return path;
		}

		if (path.empty()){
			path.insert(path.end(), curPath.begin(), curPath.end());
		} else {
			path.insert(path.end(), std::next(curPath.begin()), curPath.end());
		}
		start = point;
	}
	std::cout << "found path through waypoints\n";
	return path;
}

void Grid::RenderGrid(){
	  for (int row = 0; row < GRID_HEIGHT; row++ ){
		for (int col = 0; col < GRID_WIDTH; col++){
			Tile& currentTile = getTile(row, col);
			currentTile.DrawTile();
		}
	}
}

void Grid::RenderPath(const std::list<Tile>& path, Color color){
	if (path.size() < 2){
		return;
	}

	auto it = path.begin();
	auto next = it;
	++next;

	while(next != path.end()){
		Vector3 start = it->worldPosition;
		// std::cout << start << std::endl;
		start.y -= 1.01f;
		Vector3 end = next->worldPosition;
		// std::cout << end << std::endl;
		end.y -= 1.01f;
		
		auto temp = next;
		++temp;
		if (temp == path.end()){
			Vector3 direction = Vector3Normalize(Vector3Subtract(end, start));
			end = Vector3Subtract(end, Vector3Scale(direction, TILE_SIZE / 2.0f));
		}

		DrawLine3D(start, end, color);
		++it;
		++next;
		}
}
