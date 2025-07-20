
#include "Grid.hpp"
#include "config.hpp"
#include <raylib.h>
#include <raymath.h>
#include <stdexcept>
#include <set>
#include <iostream>
#include <unordered_map>

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
			tiles[row][col].currentColor = tileDefaultColorFaded;
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
	return TileEdge::TOP; //fallback
}

float Grid::checkNeighborCost(const TileNode& start, Tile& neighbor, bool ignoreImpassable){
	//checks if the neighbor is in range of the tile
	//flag to ignore if that tile would normally be impassable.
	if (!ignoreImpassable){
		if (neighbor.hasUnit || !neighbor.traversable){
			return 999.0f;
		} else {
			Vector2 startPos = (Vector2){start.tile.gridPosition.x, start.tile.gridPosition.y};
			float currentCost = start.cost;
			if (startPos.x != neighbor.gridPosition.x && startPos.y != neighbor.gridPosition.y){
				//diagonal
				float neighborCost = currentCost + 1.414f; 
				return neighborCost;
			} else {
				//not diagonal
				float neighborCost = currentCost + 1.0f;
				return neighborCost;
			}
		}
	}
	else {
		Vector2 startPos = (Vector2){start.tile.gridPosition.x, start.tile.gridPosition.y};
		float currentCost = start.cost;
		if (startPos.x != neighbor.gridPosition.x && startPos.y != neighbor.gridPosition.y){
			//diagonal
			float neighborCost = currentCost + 1.414f; 
			return neighborCost;
		} else {
			//not diagonal
			float neighborCost = currentCost + 1.0f;
			return neighborCost;
		}

	}
}


std::vector<TileEdge> Grid::getMovementRangeOutline(const std::unordered_map<Tile, TileNode, TileHash>& movementRange, float speed) {
    std::vector<TileEdge> outline;
    std::set<std::pair<int, int>> processedTiles;

    
    for (const auto& [tile, node] : movementRange) {
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
        
		for (const auto& [coords, direction] : neighborCoords) {
			//Check if the coords are even in the grid.
			// TileEdge curEdge{tile, direction};
			// std::cout << curEdge;
			if (isValid(coords.first, coords.second)){
				//if the coordinates are within the grid we now need to check if it's in our range
				Tile neighbor = getTile(coords.first, coords.second);
				//this checks to see if the neighbor is outside of the range. If it is, then the current tile marks the boundary of movement.
				if (movementRange.find(neighbor) == movementRange.end()){
					// check if the boundary would otherwise be traversable
					// float neighborCost = node.trueCost;
					// std::cout << "Neighbor cost of: " << neighborCost << " at (" << neighbor.gridPosition.x << " , " << neighbor.gridPosition.y << " )\n"; 
					// if (neighborCost < speed) {
						outline.push_back({tile, direction});
					// }
				
				}
					
			} 			
			else {
				outline.push_back({tile, direction});
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

void Grid::HighlightMovementRange(const std::unordered_map<Tile, TileNode, TileHash>& range){
	for (auto& [tile, node] : range){
		Tile& mutTile = getTile(tile.gridPosition.x, tile.gridPosition.y);
		mutTile.DrawTile(tileScootColor);
	}
}

void Grid::HideMovementRange(const std::unordered_map<Tile, TileNode, TileHash>& range){
	for (auto& [tile, node] : range){
		Tile& mutTile = getTile(tile.gridPosition.x, tile.gridPosition.y);
		mutTile.currentColor = tileDefaultColorFaded;
	}
}


void Grid::RenderWaypoints(const Tile& waypoint, Color color){
	Vector3 pos = waypoint.worldPosition;
	Vector3 tilePos = (Vector3){0.0f, 0.5f, 0.0f};
	DrawCube(Vector3Subtract(pos, tilePos), TILE_SIZE/4, 0.09f, TILE_SIZE/4, color);
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
