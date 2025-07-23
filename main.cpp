
#include <raylib.h>
#include "camera.hpp"
#include "input.hpp"
#include "grid.hpp"
#include <iostream>
#include "movement.hpp"



int main(){
	InitWindow(1200, 800, "Camera Test");
	Camera3D camera = {0};
	Vector3 initialCameraPos = {0.0f, 0.0f, 0.0f};
	initCamera(camera, initialCameraPos);
	SetTargetFPS(60);

	Vector3 worldOrigin = {0.0f, 0.0f, 0.0f};
	TacticalGrid::initGrids();

	Unit* selectedUnit = TacticalGrid::getUnitAt(5,5);
	TacticalGrid::setMovementDisplay(selectedUnit);

	bool showHover = false;
	bool showRange = true;

	std::vector<Vector2> pathPreview;
	bool showPreview = false;

	float lastFrame = GetTime();

	int lastX = -999;
	int lastY = -999;
	bool mousePosChanged = false;

	while (!WindowShouldClose()){
		float currentFrame = GetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		Input::updateInput(camera, worldOrigin, GRID_WIDTH, GRID_HEIGHT);
		updateCamera(camera, Input::getCameraInput());

		const auto& mouseInput = Input::getMouseInput();
		int x = (int)mouseInput.gridPosition.x;
		int y = (int)mouseInput.gridPosition.y;

		if (x != lastX || y != lastY){
			mousePosChanged = true;
		}

		

		if (mouseInput.hasValidGridPos && selectedUnit){
			showRange = !selectedUnit->isMoving;
			if (TacticalGrid::canUnitReach(selectedUnit, x, y)){
				showHover = true;
				if (TacticalGrid::waypoints.empty()){
					pathPreview = TacticalGrid::reconstructPath(selectedUnit->gridPosition.x, selectedUnit->gridPosition.y, x, y);
					showPreview = !pathPreview.empty();

				}
				else {
					PathData waypointPath = TacticalGrid::calculateWaypointPath(selectedUnit, mouseInput.gridPosition);
					if (waypointPath.isReachable){
						pathPreview = waypointPath.path;
					}
				}
				if (IsKeyPressed(KEY_M) && !pathPreview.empty()){
					Movement::setPath(selectedUnit, pathPreview);
					TacticalGrid::waypoints.clear();
				}
				if (IsKeyPressed(KEY_P)){
					TacticalGrid::waypoints.clear();
					TacticalGrid::setMovementDisplay(selectedUnit);
				}	
				if (IsKeyPressed(KEY_O)){
				    if (TacticalGrid::waypoints.empty()){
					float cost = TacticalGrid::getMovementCost(selectedUnit, x, y);
					TacticalGrid::waypoints.push_back({cost, mouseInput.gridPosition});
					
					// Remaining movement = total speed - cost to reach this waypoint
					float remainingMovement = selectedUnit->speed - cost;
					TacticalGrid::setMovementDisplay(x, y, remainingMovement);
				    } else {
					int wayX = TacticalGrid::waypoints.back().parent.x;
					int wayY = TacticalGrid::waypoints.back().parent.y;
					float totalCost = TacticalGrid::waypoints.back().cost;
					float cost = TacticalGrid::getMovementCost(wayX, wayY, x, y);
					totalCost += cost;
					TacticalGrid::waypoints.push_back({totalCost, mouseInput.gridPosition});
					
					// Remaining movement = total speed - total cost to reach this waypoint
					float remainingMovement = selectedUnit->speed - totalCost;
					TacticalGrid::setMovementDisplay(x, y, remainingMovement);
				    }
				}				
				if (IsKeyPressed(KEY_I) && !TacticalGrid::waypoints.empty()){
					PathData waypointPath = TacticalGrid::calculateWaypointPath(selectedUnit, mouseInput.gridPosition);
					if (waypointPath.isReachable){
						std::cout << "waypoint path reachable\n";
						pathPreview = waypointPath.path;
					}
				}
			} else showHover = false;
		}
			
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr){
				selectedUnit = TacticalGrid::getUnitAt(x,y);
				TacticalGrid::setMovementDisplay(selectedUnit);

			}
		}

		if (IsKeyPressed(KEY_TAB) && selectedUnit){
			int currentId = selectedUnit->id;
			if (TacticalGrid::getNextUnit(currentId) != nullptr){
				selectedUnit = TacticalGrid::getNextUnit(currentId);
				TacticalGrid::setMovementDisplay(selectedUnit);
			}
		}	

		if (IsKeyPressed(KEY_T)){
			std::cout << selectedUnit->gridPosition.x << " , " << selectedUnit->gridPosition.y << std::endl;
		}

		if (IsKeyPressed(KEY_L)){
			std::cout << "Current path preview:\n" << pathPreview << std::endl;
		}

		Movement::updateMove(deltaTime);

		lastX = x;
		lastY = y;


		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				TacticalGrid::drawTerrain(worldOrigin);
				TacticalGrid::setSelectedHighlight(selectedUnit->id);
				TacticalGrid::drawUnits(worldOrigin);
				Movement::drawMovingUnits(worldOrigin);	
				if (showRange) TacticalGrid::drawMovementOverlay(worldOrigin);
				if (showHover) TacticalGrid::drawHoverHighlight(x, y, worldOrigin);
				if (showPreview) TacticalGrid::drawPathPreview(pathPreview, SKYBLUE);
				for (auto& point : TacticalGrid::waypoints){
					Vector3 pos = TacticalGrid::gridToWorldPosition(point.parent, 0.1f);
					DrawCube(pos, .25f, .01f, .25f, MAGENTA);
			
				}
				pathPreview.clear();	
		
		

			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

