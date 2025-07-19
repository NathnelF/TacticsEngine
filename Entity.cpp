
#include "Entity.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "utils.hpp"
#include <set>
#include <queue>
#include <algorithm>

// constructor 
GameEntity::GameEntity(Grid& grid, Vector3 initialPosition, Color initialColor, Tile* initialTile, float playerSpeed){
	parentGrid = &grid;
	size = 2.0f;
	mesh = GenMeshCube(size, size, size);
	model = LoadModelFromMesh(mesh);
	collider = GetMeshBoundingBox(mesh);
	selected = false;
	isMoving = false;
	position = initialPosition;
	currentColor = initialColor;
	defaultColor = initialColor;
	collider.min = (Vector3){ position.x - size / 2.0f,
	     position.y - size / 2.0f,
	     position.z - size / 2.0f };
	collider.max = (Vector3){ position.x + size / 2.0f,
	     position.y + size / 2.0f,
	     position.z + size / 2.0f };
	currentTile = initialTile;
	currentTile->hasUnit = true;
	prevTile = NULL;
	speed = playerSpeed;


}
// add destructor?
GameEntity::~GameEntity() {
	UnloadModel(model);
}


void GameEntity::SetPath(std::vector<Tile> path){
	if (!path.empty()){
		currentPath = path;
		pathIterator = currentPath.begin();
		// std::cout << "Path received. First tile gridPos: ("
			  // << pathIterator->gridPosition.x << ", "
			  // << pathIterator->gridPosition.y << ")\n";
		isMoving = true;
	}
}

	//methods
void GameEntity::UpdateMove(float moveSpeed, float deltaTime){
	if (currentPath.empty() || pathIterator == currentPath.end()){
		return;
	}

	Tile destinationTile = *pathIterator;
	// std::cout << "Destination tile: (" << destinationTile.worldPosition.x << " , " << destinationTile.worldPosition.z << std::endl;
	
	Vector3 direction = Vector3Subtract(destinationTile.worldPosition, position);
	// std::cout << "Current direction: " << direction << std::endl;

	float travelDist = Vector3Length(direction); 
	// std::cout << "Current travel dist: " << travelDist << std::endl;

	if (travelDist < 0.05f) {
		//we are there
		
		position = destinationTile.worldPosition;
		// std::cout << "Attempting to update currentTile with gridPos: ("
		//                 << destinationTile.gridPosition.x << " , "
		//                 << destinationTile.gridPosition.y << ")\n";

		prevTile = currentTile;
		currentTile = parentGrid->getTilePointer(destinationTile.gridPosition.x, destinationTile.gridPosition.y);
		prevTile->hasUnit = false;
		currentTile->hasUnit = true;
		pathIterator++;

		if (pathIterator == currentPath.end()){
			currentPath.clear();
			// std::cout << "Reached destination!\n";
			movementRange.clear();
			isMoving = false;
		}

	}
	else {
		Vector3 normalizedDirection = Vector3Normalize(direction);
		float moveRate = moveSpeed * deltaTime;
		position = Vector3Add(position, Vector3Scale(normalizedDirection, moveRate));
		// std::cout << "Position after moving: " << position << std::endl;

	}
	
	collider.min = (Vector3){ position.x - size / 2.0f, position.y - size / 2.0f, position.z - size / 2.0f };
	collider.max = (Vector3){ position.x + size / 2.0f, position.y + size / 2.0f, position.z + size / 2.0f };

}

std::unordered_map<Tile, TileNode, TileHash> GameEntity::movementPreviewWithCost(Tile& start){
    std::unordered_map<Tile, TileNode, TileHash> nodesInRange;
    std::unordered_map<Tile, float, TileHash> distances; // Track best known distances
    std::priority_queue<std::tuple<Tile, float, float, Tile>, 
                       std::vector<std::tuple<Tile, float, float, Tile>>, 
                       NodeComparator> pq;
    
    // Initialize start node
    pq.push({start, 0.0f, 0.0f, start});
    distances[start] = 0.0f;
    
    while (!pq.empty()){
        auto [currentPos, totalCost, trueCost, parentPos] = pq.top();
        pq.pop();
        
        // Skip if we've already found a better path to this tile
        if (distances.find(currentPos) != distances.end() && 
            distances[currentPos] < totalCost) {
            continue;
        }
        
        // Add to results
        TileNode currentNode{currentPos, parentPos, totalCost, trueCost};
        nodesInRange[currentPos] = currentNode;
        
        // Only explore neighbors if within movement range
        if (totalCost < speed){
            std::vector<Tile> neighbors = parentGrid->getGridNeighbors(currentPos);
            for (const auto& neighbor : neighbors){
                float moveCost = (neighbor.gridPosition.x != currentPos.gridPosition.x && 
                                neighbor.gridPosition.y != currentPos.gridPosition.y) ? 1.414f : 1.0f;
                
                if (neighbor.hasUnit || !neighbor.traversable){
                    // Add unreachable tiles with high cost for outline generation
                    float newTotal = 999.0f;
                    float newTrue = trueCost + moveCost;
                    
                    // Only add if we haven't processed this tile or found a better path
                    if (distances.find(neighbor) == distances.end() || 
                        distances[neighbor] > newTrue) {
                        distances[neighbor] = newTrue;
                        pq.push({neighbor, newTotal, newTrue, currentPos});
                    }
                } else {
                    float newTotal = totalCost + moveCost;
                    float newTrue = trueCost + moveCost;
                    
                    // Only add if within range and we haven't found a better path
                    if (newTotal <= speed && 
                        (distances.find(neighbor) == distances.end() || 
                         distances[neighbor] > newTotal)) {
                        distances[neighbor] = newTotal;
                        pq.push({neighbor, newTotal, newTrue, currentPos});
                    }
                }
            }
        }
    }
    return nodesInRange;
}

std::vector<Tile> GameEntity::getPath(Tile& start, Tile& target, std::unordered_map<Tile, TileNode, TileHash>& movementRange) {
    std::vector<Tile> path;
    
    // Check if target is reachable
    auto targetIt = movementRange.find(target);
    if (targetIt == movementRange.end() || targetIt->second.cost >= 999.0f) {
        return path; // Empty path if unreachable
    }
    
    // Trace back from target to start
    Tile current = target;
    Tile startTile = start; // Your entity's current position
    
    while (current.gridPosition != startTile.gridPosition) {
        path.push_back(current);
        
        auto currentIt = movementRange.find(current);
        if (currentIt == movementRange.end()) {
            // Error in path reconstruction
            return {};
        }
        
        current = currentIt->second.parentTile;
    }
    
    path.push_back(startTile); // Add start position
    std::reverse(path.begin(), path.end()); // Reverse to get start->target order
    
    return path;
}


std::vector<Tile> GameEntity::getWaypointPath(Tile& start, std::vector<Tile>& waypoints, Tile& hoveredTile){
	std::vector<Tile> path;
	if (waypoints.empty()) return path;

	Tile curStart = start; 

	for (auto& point : waypoints){

		std::unordered_map<Tile, TileNode, TileHash> curRange = movementPreviewWithCost(curStart);
		std::vector<Tile> curPath = getPath(curStart, point, curRange);

		if (curRange.find(hoveredTile) == curRange.end()){
			return path;
		}
		// std::cout << "checking path\n";

		if (curPath.empty()){
			return path;
		}

		if (path.empty()){
			path.insert(path.end(), curPath.begin(), curPath.end());
		} else {
			path.insert(path.end(), std::next(curPath.begin()), curPath.end());
		}
		curStart = point;
	}
	//path from last waypoint to hoveredTile
	
	std::unordered_map<Tile, TileNode, TileHash> finalRange = movementPreviewWithCost(curStart);
    	std::vector<Tile> finalPath = getPath(curStart, hoveredTile, finalRange);
    
    	if (finalPath.empty()){
        	return path;
	}

	if (finalRange.find(hoveredTile) == finalRange.end()){
		return path;
	}

    	if (!finalPath.empty()) {
        	path.insert(path.end(), std::next(finalPath.begin()), finalPath.end());
    	}

	return path;
}

void GameEntity::Draw(){
	DrawModel(model, position, 1.0f, currentColor);
	DrawBoundingBox(collider, GREEN);
}


void GameEntity::RenderMovementPreview(const std::unordered_map<Tile, TileNode, TileHash>& range){
	for (const auto& [tile, node] : range){
		Vector3 pos = tile.worldPosition;
		DrawCube(pos, 0.5f, 0.01f, 0.5f, YELLOW);
	}
}
