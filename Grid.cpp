
#include "Grid.hpp"
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>

Grid::Grid(int gridWidth, int gridHeight):
	tiles(gridHeight, std::vector<Tile>(gridWidth))
{
	for (int row = 0; row < gridHeight; row++){
		for (int col = 0; col < gridWidth; col++){
			tiles[row][col].height = 0.0f; //keep flat for now
			tiles[row][col].gridPosition = (Vector2){(float)col, (float)row};
			tiles[row][col].worldPosition = (Vector3){
				(float)col * TILE_SIZE - TILE_SIZE / 2,
				0.2f,
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
			neighbors.push_back(getTile(neighborPos.x, neighborPos.y));	
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
			if (neighbor.hasUnit || !neighbor.traversable){
				//skip
			}
			else { 
				if (visited.find(neighbor) == visited.end()){
					queue.push(neighbor);
					visited.insert(neighbor);
					parentMap[neighbor] = current;
				}
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



void Grid::RenderGrid(){
	  for (int row = 0; row < GRID_HEIGHT; row++ ){
		for (int col = 0; col < GRID_WIDTH; col++){
			Tile& currentTile = getTile(row, col);
			currentTile.DrawTile();
		}
	}
}

void Grid::RenderWaypoints(const Tile& waypoint, Color color){
	Vector3 pos = waypoint.worldPosition;
	Vector3 tilePos = (Vector3){0.0f, 0.5f, 0.0f};
	DrawCube(Vector3Subtract(pos, tilePos), TILE_SIZE/4, 0.09f, TILE_SIZE/4, color);
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


std::vector<TileEdge::Direction> Grid::getOppositeDirections(TileEdge::Direction dir) {
    switch (dir) {
	case TileEdge::TOP: return {TileEdge::LEFT, TileEdge::RIGHT};
	case TileEdge::RIGHT: return {TileEdge::TOP, TileEdge::BOTTOM};
	case TileEdge::BOTTOM: return {TileEdge::RIGHT, TileEdge::LEFT};
	case TileEdge::LEFT: return {TileEdge::TOP, TileEdge::BOTTOM};
    }
    return {TileEdge::TOP}; // fallback
}

TileEdge::Direction Grid::getDirectOppositeEdge(TileEdge::Direction dir){
	switch (dir){
		case TileEdge::TOP: return TileEdge::BOTTOM;
		case TileEdge::BOTTOM: return TileEdge::TOP;
		case TileEdge::RIGHT: return TileEdge::LEFT;
		case TileEdge::LEFT: return TileEdge::RIGHT;
	}
	return {TileEdge::TOP}; //fallback
}


std::vector<TileEdge> Grid::getMovementRangeOutline(const std::vector<Tile>& movementRange) {
    std::set<Tile> rangeSet(movementRange.begin(), movementRange.end());
    std::vector<TileEdge> outline;
    std::set<std::pair<int, int>> processedTiles;

    
    for (const auto& tile : movementRange) {
	std::pair<int, int> tileCoords = {tile.gridPosition.x, tile.gridPosition.y};
        if (processedTiles.find(tileCoords) != processedTiles.end()) {
            continue;
        }
        processedTiles.insert(tileCoords);

       std::vector<std::pair<std::pair<int, int>, TileEdge::Direction>> neighborCoords = {
            {{tile.gridPosition.x, tile.gridPosition.y - 1}, TileEdge::LEFT},
            {{tile.gridPosition.x + 1, tile.gridPosition.y}, TileEdge::TOP},
            {{tile.gridPosition.x, tile.gridPosition.y + 1}, TileEdge::RIGHT},
            {{tile.gridPosition.x - 1, tile.gridPosition.y}, TileEdge::BOTTOM}
        };
        
	std::vector<TileEdge::Direction> boundaryEdges;
        for (const auto& [coords, direction] : neighborCoords) {
		//Check if the coords are even in the grid.
		// TileEdge curEdge{tile, direction};
		// std::cout << curEdge;
		if (isValid(coords.first, coords.second)){
			//if the coordinates are within the grid we now need to check if it's in our range
			Tile neighbor = getTile(coords.first, coords.second);
			//this checks to see if the neighbor is outside of the range. If it is, then the current tile marks the boundary of movement.
			if (rangeSet.find(neighbor) == rangeSet.end()){
				//add all the edges to the boundary list
				boundaryEdges.push_back(direction);
				}
				
			} 			
			else {
				boundaryEdges.push_back(direction);
			//if the coords are not in grid, we need to outline that.
			}
	 }
	 if (boundaryEdges.size() == 1){
	 	if (!tile.hasUnit && tile.traversable ){
	 		for (const auto& direction : boundaryEdges){
				outline.push_back({tile, direction});
	 		}
		}
		else {
	 		// std::set<TileEdge::Direction> edgesToAdd;
			//if there is a unit we want to wrap around it, that means we get all the edges opposite of the current edge to wrap around that obstacle. 
			//unfortunately we also need to check if the neighboring tiles have units in them
			//for exampple: we find the left edge to be edge of movement range, but there is a unit in that tile
			// we know for sure that the right edge of that tile must go into the outline
			// if there is a unit above, then the top edge does NOT go in. Same for bottom.
	 		for (const auto& direction : boundaryEdges){
				
				TileEdge bound{tile, direction};
				// std::cout << "The boundary direction is : " << bound;

				TileEdge::Direction oppositeDir = getDirectOppositeEdge(direction); //direct opposite always goes in. I think.
				TileEdge opp{tile, oppositeDir};
				// std::cout << "Opposite edge is " << opp;
				outline.push_back({tile, oppositeDir});

	 			//let's check the neighbors perpendicular
	 			std::vector<TileEdge::Direction> neighborDirections = getOppositeDirections(direction);
	 			for (const auto& dir : neighborDirections){
					TileEdge currentEdge{tile, dir};
					// std::cout << "checking neighbor " << currentEdge;
	 				bool shouldAdd = true;
					switch (dir){
							case TileEdge::TOP:
								if (isValid(tile.gridPosition.x+1, tile.gridPosition.y)){
									// std::cout << "Valid tile at top neighbor\n";
									Tile neighborTop = getTile(tile.gridPosition.x+1, tile.gridPosition.y);
									if (!neighborTop.hasUnit && neighborTop.traversable){
										// std::cout << "Top neighbor is free, add the edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip top edge\n";
	 									shouldAdd = false;
									}
								}
							break;
							case TileEdge::BOTTOM:
								if (isValid(tile.gridPosition.x-1, tile.gridPosition.y)){
									// std::cout << "Valid tile at bottom neighbor\n";
									Tile neighborBottom = getTile(tile.gridPosition.x-1, tile.gridPosition.y);
									if (!neighborBottom.hasUnit && neighborBottom.traversable){
										// std::cout << "Bottom neighbor is free, add the edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip bottom edge\n";
	 									shouldAdd = false;
									}
								}
								break;
							case TileEdge::RIGHT:
								if (isValid(tile.gridPosition.x, tile.gridPosition.y+1)){
									// std::cout << "Valid tile at right neighbor\n";
									Tile neighborRight = getTile(tile.gridPosition.x, tile.gridPosition.y+1);
									if (!neighborRight.hasUnit && neighborRight.traversable){
										// std::cout << "Right neighbor is free, add edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip right edge\n";
										shouldAdd = false;
									}
								}
								break;

							case TileEdge::LEFT:
								if (isValid(tile.gridPosition.x, tile.gridPosition.y-1)){
									// std::cout << "Valid tile at left neighbor\n";
									Tile neighborLeft = getTile(tile.gridPosition.x, tile.gridPosition.y-1);
									if (!neighborLeft.hasUnit && neighborLeft.traversable){
										// std::cout << "Left neighbor is free, add edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip left edge\n";
										shouldAdd = false;
									}
								}
								break;
						}
					if (shouldAdd) {
						outline.push_back({tile, dir});
					}
	 			}
	 		}
	 	}
	} 
	if (boundaryEdges.size() > 1){
		if (!tile.hasUnit && tile.traversable ){
	 		for (const auto& direction : boundaryEdges){
				outline.push_back({tile, direction});
	 		}
		} else {
		//boundary edges are more than 1. This is a corner.
		// we need to always remove the boundary edges.
		// check both opposite directions, and only add the one without a neighbor!
		// std::cout << "Two boundary edges identified!\n";
		for (const auto& direction : boundaryEdges){
			TileEdge boundary{tile, direction};
			// std::cout << "Boundary edge : " << boundary;
			TileEdge::Direction opposite = getDirectOppositeEdge(direction);
			TileEdge opp{tile, opposite};
			// std::cout << "Opposite edge is : " << opp;
			bool shouldAdd = true;
					switch (opposite){
							case TileEdge::TOP:
								if (isValid(tile.gridPosition.x+1, tile.gridPosition.y)){
									// std::cout << "Valid tile at top neighbor\n";
									Tile neighborTop = getTile(tile.gridPosition.x+1, tile.gridPosition.y);
									if (!neighborTop.hasUnit && neighborTop.traversable){
										// std::cout << "Top neighbor is free, add the edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip top edge\n";
	 									shouldAdd = false;
									}
								}
								break;
							case TileEdge::BOTTOM:
								if (isValid(tile.gridPosition.x-1, tile.gridPosition.y)){
									// std::cout << "Valid tile at bottom neighbor\n";
									Tile neighborBottom = getTile(tile.gridPosition.x-1, tile.gridPosition.y);
									if (!neighborBottom.hasUnit && neighborBottom.traversable){
										// std::cout << "Bottom neighbor is free, add the edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip bottom edge\n";
	 									shouldAdd = false;
									}
								}
								break;
							case TileEdge::RIGHT:
								if (isValid(tile.gridPosition.x, tile.gridPosition.y+1)){
									// std::cout << "Valid tile at right neighbor\n";
									Tile neighborRight = getTile(tile.gridPosition.x, tile.gridPosition.y+1);
									if (!neighborRight.hasUnit && neighborRight.traversable){
										// std::cout << "Right neighbor is free, add edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip right edge\n";
										shouldAdd = false;
									}
								}
								break;

							case TileEdge::LEFT:
								if (isValid(tile.gridPosition.x, tile.gridPosition.y-1)){
									// std::cout << "Valid tile at left neighbor\n";
									Tile neighborLeft = getTile(tile.gridPosition.x, tile.gridPosition.y-1);
									if (!neighborLeft.hasUnit && neighborLeft.traversable){
										// std::cout << "Left neighbor is free, add edge to the outline\n";
									} else {
										// std::cout << "Not free. Skip left edge\n";
										shouldAdd = false;
									}
								}
								break;

				}
				if (shouldAdd){
					outline.push_back({tile, opposite});
				}
		}
	}
	}
}
	return outline;
}

void Grid::RenderOutline(const std::vector<TileEdge>& outline){
    for (const auto& edge : outline){
        Vector3 worldPos = edge.tile.worldPosition;
        float tileSize = TILE_SIZE;
        float borderHeight = 0.01f;
        Vector3 start, end;
        switch (edge.direction) {
            case TileEdge::TOP:
                start = {worldPos.x + tileSize/2, worldPos.y + borderHeight, worldPos.z + tileSize/2};
                end = {worldPos.x + tileSize/2, worldPos.y + borderHeight, worldPos.z - tileSize/2};
                break;
                case TileEdge::RIGHT:
                        start = {worldPos.x + tileSize/2, worldPos.y + borderHeight, worldPos.z + tileSize/2};
                        end = {worldPos.x - tileSize/2, worldPos.y + borderHeight, worldPos.z + tileSize/2};
                        break;
                case TileEdge::BOTTOM:
                        start = {worldPos.x - tileSize/2, worldPos.y + borderHeight, worldPos.z + tileSize/2};
                end = {worldPos.x - tileSize/2, worldPos.y + borderHeight, worldPos.z - tileSize/2};
                break;
                case TileEdge::LEFT:
                start = {worldPos.x + tileSize/2, worldPos.y + borderHeight, worldPos.z - tileSize/2};
                end = {worldPos.x - tileSize/2, worldPos.y + borderHeight, worldPos.z - tileSize/2};
                break;
        }

        DrawLine3D(start, end, DARKBROWN);
    }
}


std::vector<Tile> Grid::getPathFromRange(Tile& start, Tile& end, std::unordered_map<Tile, TileNode, TileHash>& range){
	std::vector<Tile> path;

	if (range.find(end) == range.end()){
		return path;
	} else {
		Tile current = end;

		while (current.gridPosition != start.gridPosition){
			path.push_back(current);
			current = range[current].parentTile;
		}
		path.push_back(start);
		std::reverse(path.begin(), path.end());
		return path;
	}
	
}

void Grid::RenderPathRange(const std::vector<Tile>& path, Color color){
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
