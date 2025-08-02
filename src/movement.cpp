
#include "movement.hpp"
#include "grid.hpp"
#include "player_unit.hpp"
#include <iostream>
#include <algorithm>

namespace Movement{

	float movementSpeed = 3.0f;
	std::vector<MovingUnit> movingUnits;

	void setPath(GridUnit* unit, std::vector<Vector2> path){
		
		if (unit->isMoving){
			std::cout << "Unit already moving!\n";
			return;
		}

		if (path.size() < 2) {
			std::cout << "Path too short for unit " << unit->id << std::endl;
			return; // Need at least start and destination
		}

		unit->isMoving = true;
		Vector3 pos = TacticalGrid::gridToWorldPosition(path[0], 1.0f);
		movingUnits.push_back({unit->id, pos, path,  0, 0.0f});
		Vector2 oldPos = unit->gridPosition;
		Vector2 newPos = path.back();
		TacticalGrid::unitGrid[(int)oldPos.y][(int)oldPos.x] = -1;
		TacticalGrid::unitGrid[(int)newPos.y][(int)newPos.x] = unit->id;
		unit->gridPosition = newPos;
	}

	void updateMove(float deltaTime){
		for (auto& movingUnit : movingUnits){

			movingUnit.moveProgress += movementSpeed * deltaTime;
			// std::cout << "Current moveProgress: " << movingUnit.moveProgress << std::endl;
			Vector3 startPos = TacticalGrid::gridToWorldPosition(movingUnit.path[movingUnit.currentWaypointIndex], 1.0f);
			// std::cout << "Current position " << startPos << std::endl;
			Vector3 targetPos = TacticalGrid::gridToWorldPosition(movingUnit.path[movingUnit.currentWaypointIndex + 1], 1.0f);
			// std::cout << "Current target pos " << targetPos << std::endl;
		

			if (movingUnit.moveProgress >= 1.0f){
				// std::cout << "reached waypoint!\n";
				movingUnit.moveProgress = 0.0f;
				//we have reached the next waypoint
				movingUnit.currentWaypointIndex++;
				movingUnit.position = targetPos;
				if (movingUnit.currentWaypointIndex >= movingUnit.path.size() - 1) {
					std::cout << "Reached final destination!\n";
					//we have reached the final waypoint
					GridUnit* unit = nullptr;
					for (auto& u : PlayerUnits::playerUnits){
						if (movingUnit.unitId == u.id) unit = &u.gridUnit;
					}
					if (unit){
						unit->isMoving = false;
					}
					movingUnit.currentWaypointIndex = -1;
				}
			} else {
				// std::cout << "Not at waypoint yet.\n";
				//move to next waypoint
				Vector3 newPos = Vector3Lerp(startPos, targetPos, movingUnit.moveProgress);
				// std::cout << "New position will be " << newPos << std::endl;
				movingUnit.position = newPos;
				// std::cout << "Position updated to " << movingUnit.position << std::endl;
			}

		}
			movingUnits.erase(
			std::remove_if(movingUnits.begin(), movingUnits.end(),
		  		[](const MovingUnit& unit) {return unit.currentWaypointIndex == -1; }),
			movingUnits.end()
		);
	}

	void drawMovingUnits(Vector3 worldOrigin){
		for (const auto& movingUnit : movingUnits){
			for (const auto& unit : PlayerUnits::playerUnits){
				if (movingUnit.unitId == unit.id) {
					// std::cout << "attempting draw for moving unit " << movingUnit.unitId << "!\n";
					DrawCube(movingUnit.position, 1.5f, 1.0f, 1.5f, unit.gridUnit.color);
					break;
				}
			}
		}
	}

}
