
#pragma once

#include <raylib.h>
#include <raymath.h>
#include "grid.hpp"

struct MovingUnit{
	int unitId;
	Vector3 position;
	std::vector<Vector2> path;
	int currentWaypointIndex;
	float moveProgress;
};

namespace Movement{

	extern float moveSpeed;
	extern std::vector<MovingUnit> movingUnits;

	void setPath(Unit* unit, std::vector<Vector2> path);
	void updateMove(float deltaTime);

	bool IsUnitMoving(int unitId);
	void stopMovement(int unitId);
	void drawMovingUnits(Vector3 worldOrigin);

}
