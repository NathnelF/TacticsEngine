
#include "Entity.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "utils.hpp"
#include <set>
#include <queue>

// constructor 
GameEntity::GameEntity(Grid& grid, Vector3 initialPosition, Color initialColor, Tile* initialTile, float playerSpeed){
	parentGrid = &grid;
	size = 2.0f;
	t = 0.0f;
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

std::vector<Tile> GameEntity::movementPreview(){
	std::vector<Tile> tilesInRange;
	std::set<Tile> visited;
	std::queue<std::pair<Tile, int>> queue;

	queue.push({*currentTile, speed});
	visited.insert(*currentTile);

	while (!queue.empty()){
		auto [currentPos, remainingMoves] = queue.front();
		queue.pop();

		tilesInRange.push_back(currentPos);

		if (remainingMoves > 0){
			std::vector<Tile> neighbors = parentGrid->getGridNeighbors(currentPos);
			for (const auto& neighbor : neighbors){
				if (visited.find(neighbor) == visited.end()){
					queue.push({neighbor, remainingMoves - 1});
					visited.insert(neighbor);
				}
			}
		}
	}
	return tilesInRange;
}
std::unordered_map<Tile, TileNode, TileHash> GameEntity::movementPreviewWithCost(Tile& start){
	std::unordered_map<Tile, TileNode, TileHash> nodesInRange;
	std::set<Tile> visited;
	std::queue<std::tuple<Tile, float, Tile>> queue;

	queue.push({start, 0.0f, start}); //costs nothing to move to currentTile.
	visited.insert(start);

	while (!queue.empty()){
		auto [currentPos, totalCost, parentPos] = queue.front();
		queue.pop();
		TileNode currentNode{currentPos, parentPos, totalCost};

		nodesInRange[currentPos] = currentNode;
		if (totalCost < speed){
			std::vector<Tile> neighbors = parentGrid->getGridNeighbors(currentPos);
			for (const auto& neighbor : neighbors){
				if (neighbor.hasUnit || !neighbor.traversable){

				}
				else {
					if (visited.find(neighbor) == visited.end()){
						float moveCost;
						if (neighbor.gridPosition.x != currentPos.gridPosition.x && neighbor.gridPosition.y != currentPos.gridPosition.y){
							//diagonal
							moveCost = 1.414f;
						} else {
							//not diagonal
							moveCost = 1.0f;
						}
						float newTotal = totalCost + moveCost;
						if (newTotal <= speed){
							queue.push({neighbor, newTotal, currentPos});
							visited.insert(neighbor);
						}
					}

				}
			
			}
		}
	}
	return nodesInRange;
}


std::vector<Tile> GameEntity::getWaypointPath(Tile& start, std::vector<Tile>& waypoints, Tile& hoveredTile){
	std::vector<Tile> path;
	if (waypoints.empty()) return path;

	Tile curStart = start; 

	for (auto& point : waypoints){

		std::unordered_map<Tile, TileNode, TileHash> curRange = movementPreviewWithCost(curStart);
		std::vector<Tile> curPath = parentGrid->getPathFromRange(curStart, point, curRange);

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
    	std::vector<Tile> finalPath = parentGrid->getPathFromRange(curStart, hoveredTile, finalRange);
    
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
//
//We need to basically do the same thing just check each neighbor to see if it's diagonal or not. The add to the cost accordingly.  
//
//
//
//

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
