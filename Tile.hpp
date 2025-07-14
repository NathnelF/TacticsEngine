#ifndef TILE_H
#define TILE_H

#include <raylib.h>
#include <raymath.h>
#include <vector>

class GameEntity;

class Tile{
public:
	Vector3 worldPosition;
	Vector2 gridPosition;
	float height;
	bool hasUnit;
	bool traversable;
	Color color;
	BoundingBox bounds;
	GameEntity* entity;

	Tile(int x, int y);
	Tile();
	bool operator<(const Tile& other) const;
	void DrawTile();
	GameEntity* getEntity();
	void addEntity(GameEntity* entity);
};
#endif
