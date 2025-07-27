
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
	Color hoverColor;
	bool showRange = true;

	std::vector<Vector2> pathPreview;
	bool showPreview = false;

	float lastFrame = GetTime();

	while (!WindowShouldClose()){
		float currentFrame = GetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		Input::updateInput(camera, worldOrigin, GRID_WIDTH, GRID_HEIGHT);
		updateCamera(camera, Input::getCameraInput());

		const auto& mouseInput = Input::getMouseInput();
		int x = (int)mouseInput.gridPosition.x;
		int y = (int)mouseInput.gridPosition.y;

		if (mouseInput.hasValidGridPos && selectedUnit){
			showRange = !selectedUnit->isMoving;
			int moveCost = TacticalGrid::checkMoveDistance(x, y);
			if (moveCost != -1){
				showHover = true;
				if (selectedUnit->isMoving){
					showHover = false;
				}
				if (moveCost == 1){
					hoverColor = SKYBLUE;
				}
				if (moveCost == 2){
					hoverColor = GOLD;
				}
				if (TacticalGrid::waypoints.empty()){
					pathPreview = TacticalGrid::reconstructPath(selectedUnit->gridPosition.x, selectedUnit->gridPosition.y, x, y);
					showPreview = !pathPreview.empty();
					if (selectedUnit->isMoving){
						showPreview = false;
					}
				}
				else {
					PathData waypointPath = TacticalGrid::calculateWaypointPath(selectedUnit, mouseInput.gridPosition);
					pathPreview = waypointPath.path;
					
				}
				if (IsKeyPressed(KEY_M) && !pathPreview.empty()){
					Movement::setPath(selectedUnit, pathPreview);
					TacticalGrid::waypoints.clear();
				}
			
				if (IsKeyPressed(KEY_O)){
				    if (TacticalGrid::waypoints.empty()){
					float cost = TacticalGrid::getMovementCost(selectedUnit, x, y);
					TacticalGrid::waypoints.push_back({cost, mouseInput.gridPosition});
					
					// Remaining movement = total speed - cost to reach this waypoint
					// check for remaining scoot + dash range
					float remainingMovement = selectedUnit->speed - cost;
					float scootRange;
					float dashRange;
					if (remainingMovement < 0){
							//used up all scoot range
							scootRange = 0;
							dashRange = selectedUnit->speed*1.5 - cost;
					}
					else {
						scootRange = selectedUnit->speed - cost;
						dashRange = selectedUnit->speed * 1.5 - cost;
					}
					TacticalGrid::setMovementDisplay(x, y, scootRange, dashRange);
				    } else {
					int wayX = TacticalGrid::waypoints.back().parent.x;
					int wayY = TacticalGrid::waypoints.back().parent.y;
					float totalCost = TacticalGrid::waypoints.back().cost;
					float cost = TacticalGrid::getMovementCost(wayX, wayY, x, y);
					totalCost += cost;
					TacticalGrid::waypoints.push_back({totalCost, mouseInput.gridPosition});
					
					// Remaining movement = total speed - total cost to reach this waypoint
					float remainingMovement = selectedUnit->speed - totalCost;
					float scootRange;
					float dashRange;
					if (remainingMovement < 0){
						scootRange = 0;
						dashRange = selectedUnit->speed*1.5 - totalCost;
					} else {
						scootRange = selectedUnit->speed - totalCost;
						dashRange = selectedUnit->speed * 1.5 - totalCost;
					}
					TacticalGrid::setMovementDisplay(x, y, scootRange, dashRange);
				    }
				}				

			} else showHover = false;
		}
			
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			if (selectedUnit){
				std::cout << "It will cost " << TacticalGrid::getMovementCost(selectedUnit, x, y) << " to move to ( " << x << " , " << y << ")\n";
			}
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr){
				selectedUnit = TacticalGrid::getUnitAt(x,y);
				TacticalGrid::setMovementDisplay(selectedUnit);

			}
		}
		if (IsKeyPressed(KEY_P) && selectedUnit){
				TacticalGrid::waypoints.clear();
				TacticalGrid::setMovementDisplay(selectedUnit);
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

		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				TacticalGrid::drawTerrain(worldOrigin);
				TacticalGrid::setSelectedHighlight(selectedUnit->id);
				TacticalGrid::drawUnits(worldOrigin);
				Movement::drawMovingUnits(worldOrigin);	
				if (showRange) TacticalGrid::drawMovementOverlay(worldOrigin);
				if (showHover) TacticalGrid::drawHoverHighlight(x, y, worldOrigin, hoverColor);
				if (showPreview) TacticalGrid::drawPathPreview(pathPreview, hoverColor);
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

