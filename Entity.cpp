
#include "Entity.hpp"
#include "Tile.hpp"
#include "Grid.hpp"
#include <cfloat>
#include <raylib.h>
#include <raymath.h>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


// constructor 
GameEntity::GameEntity(Grid& grid, Vector3 initialPosition, Color initialColor, Tile* initialTile){
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


}
// add destructor?
GameEntity::~GameEntity() {
	UnloadModel(model);
}


void GameEntity::SetPath(std::list<Tile> path){
	if (!path.empty()){
		currentPath = path;
		pathIterator = currentPath.begin();
		std::cout << "Path received. First tile gridPos: ("
			  << pathIterator->gridPosition.x << ", "
			  << pathIterator->gridPosition.y << ")\n";
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
		std::cout << "Attempting to update currentTile with gridPos: ("
                  << destinationTile.gridPosition.x << " , "
                  << destinationTile.gridPosition.y << ")\n";

		prevTile = currentTile;
		currentTile = parentGrid->getTilePointer(destinationTile.gridPosition.x, destinationTile.gridPosition.y);
		prevTile->hasUnit = false;
		currentTile->hasUnit = true;
		pathIterator++;

		if (pathIterator == currentPath.end()){
			currentPath.clear();
			std::cout << "Reached destination!\n";
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
void GameEntity::Draw(){
	DrawModel(model, position, 1.0f, currentColor);
	DrawBoundingBox(collider, GREEN);
}

