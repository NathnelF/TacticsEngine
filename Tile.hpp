#ifndef TILE_H
#define TILE_H

#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "config.hpp"


class Tile{
public:
	Vector3 worldPosition;
	Vector2 gridPosition;
	float height;
	bool hasUnit;
	bool traversable;
	Color color;
	BoundingBox bounds;

	Tile(int x, int y);
	Tile();
	void DrawTile();
};
#endif
