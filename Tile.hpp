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
	Color currentColor;
	BoundingBox bounds;
	GameEntity* entity;

	Tile(int x, int y);
	Tile();
	bool operator<(const Tile& other) const;
	bool operator==(const Tile& other) const;
	void DrawTile(Color color);
	GameEntity* getEntity();
	void addEntity(GameEntity* entity);
};

struct TileNode{
	Tile tile;
	Tile parentTile;
	float cost;
	float trueCost;
};

struct NodeComparator {
    bool operator()(const std::tuple<Tile, float, float, Tile>& a, 
                   const std::tuple<Tile, float, float, Tile>& b) {
        return std::get<1>(a) > std::get<1>(b); // Compare by totalCost
    }
};

struct TileHash {
    std::size_t operator()(const Tile& tile) const {
        return std::hash<int>()(tile.gridPosition.x) ^ 
               (std::hash<int>()(tile.gridPosition.y) << 1);
    }
};

#endif
