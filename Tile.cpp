#include "Tile.hpp"
#include <raylib.h>
#include "config.hpp"

Tile::Tile(){
		height = 0.0f; //keep flat for now
		gridPosition = (Vector2){0.0f, 0.0f};
		worldPosition = (Vector3){
			0.0f * TILE_SIZE - TILE_SIZE / 2,
			0.2f,
			0.0f * TILE_SIZE - TILE_SIZE / 2
		};
		traversable = true;
		hasUnit = false;
		currentColor = tileDefaultColorFaded;
		bounds = {
			(Vector3){worldPosition.x - TILE_SIZE / 2.0f, worldPosition.y - 0.1f / 2.0f, worldPosition.z - TILE_SIZE / 2.0f},
			(Vector3){worldPosition.x + TILE_SIZE / 2.0f, worldPosition.y + 0.1f / 2.0f, worldPosition.z + TILE_SIZE / 2.0f}
		};
		entity = NULL;
}

Tile::Tile(int x, int y){
		height = 0.0f; //keep flat for now
		gridPosition = (Vector2){(float)x, (float)y};
		worldPosition = (Vector3){
			(float)x * TILE_SIZE - TILE_SIZE / 2,
			0.2f,
			(float)y * TILE_SIZE - TILE_SIZE / 2
		};
		traversable = true;
		hasUnit = false;
		currentColor = tileDefaultColorFaded;
		bounds = {
			(Vector3){(float)x * TILE_SIZE - TILE_SIZE, 0.0f, (float)y * TILE_SIZE - TILE_SIZE},
			(Vector3){(float)x * TILE_SIZE, 0.0f, (float)y * TILE_SIZE},
		};
	}

bool Tile::operator<(const Tile& other) const {
	if (gridPosition.x != other.gridPosition.x){
		return gridPosition.x < other.gridPosition.x;
	}
	return gridPosition.y < other.gridPosition.y; 
}

bool Tile::operator==(const Tile& other) const {
        return gridPosition.x == other.gridPosition.x && 
               gridPosition.y == other.gridPosition.y;
}

GameEntity* Tile::getEntity(){
	if (entity != NULL){
		return entity;
	}
	else {
		return nullptr;
	}
}

void Tile::addEntity(GameEntity* newEntity){
	if (newEntity != NULL && newEntity != nullptr){ //should we check that there are no entities on this Tile?
		entity = newEntity;
	}
}


void Tile::DrawTile(Color color){
		DrawCubeWires(worldPosition, TILE_SIZE, 0.01f, TILE_SIZE, color);
}
