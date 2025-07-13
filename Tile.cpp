#include "Tile.hpp"
#include <raylib.h>

Tile::Tile(){
		height = 0.0f; //keep flat for now
		gridPosition = (Vector2){0.0f, 0.0f};
		worldPosition = (Vector3){
			0.0f * TILE_SIZE - TILE_SIZE / 2,
			1.0f,
			0.0f * TILE_SIZE - TILE_SIZE / 2
		};
		traversable = true;
		hasUnit = false;
		color = tileDefaultColor;
		bounds = {
			(Vector3){worldPosition.x - TILE_SIZE / 2.0f, worldPosition.y - 0.1f / 2.0f, worldPosition.z - TILE_SIZE / 2.0f},
			(Vector3){worldPosition.x + TILE_SIZE / 2.0f, worldPosition.y + 0.1f / 2.0f, worldPosition.z + TILE_SIZE / 2.0f}
		};
}

Tile::Tile(int x, int y){
		height = 0.0f; //keep flat for now
		gridPosition = (Vector2){(float)x, (float)y};
		worldPosition = (Vector3){
			(float)x * TILE_SIZE - TILE_SIZE / 2,
			0.0f,
			(float)y * TILE_SIZE - TILE_SIZE / 2
		};
		traversable = true;
		hasUnit = false;
		color = tileDefaultColor;
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


void Tile::DrawTile(){
		Vector3 tilePos = (Vector3){0.0f, 0.5f, 0};
		DrawCubeWires(Vector3Subtract(worldPosition, tilePos), TILE_SIZE, 0.1f, TILE_SIZE, color);
}
