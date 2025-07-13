#include "Tile.hpp"
#include <raylib.h>

Tile::Tile(){
		height = 0.0f; //keep flat for now
		gridPosition = (Vector2){0.0f, 0.0f};
		worldPosition = (Vector3){
			0.0f * TILE_SIZE - TILE_SIZE / 2,
			0.0f,
			0.0f * TILE_SIZE - TILE_SIZE / 2
		};
		traversable = true;
		hasUnit = false;
		color = tileDefaultColor;
		bounds = {
			(Vector3){0.0f * TILE_SIZE - TILE_SIZE, 0.0f, 0.0f * TILE_SIZE - TILE_SIZE},
			(Vector3){0.0f * TILE_SIZE, 0.0f, 0.0f * TILE_SIZE},
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



void Tile::DrawTile(){
		DrawCubeWires(worldPosition, TILE_SIZE, 0.1f, TILE_SIZE, color);
}
