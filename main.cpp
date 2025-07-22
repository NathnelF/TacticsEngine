
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

	bool showHover = false;
	bool showRange = false;
	bool showWaypoints = false;

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

		if (selectedUnit){	
			if (!TacticalGrid::waypoints.empty()){
				std::cout << "waypoint exists. Attempting to calc range!\n";
				TacticalGrid::calculateRangeFrom((int)TacticalGrid::waypoints.back().x, (int)TacticalGrid::waypoints.back().y, selectedUnit->movePoints);	
				// TacticalGrid::calculateWaypointRange(x, y, selectedUnit->speed);	
				std::cout << "range successfully calculated.\n";
				showWaypoints = !selectedUnit->isMoving;
				std::cout << "show waypoints " << showWaypoints << std::endl;
				showRange = !selectedUnit->isMoving;
			} else {
				TacticalGrid::calculateMovementRange(selectedUnit->id);
				showRange = !selectedUnit->isMoving;
				showWaypoints = false;
			}
		}

		if (mouseInput.hasValidGridPos && selectedUnit){
			if (TacticalGrid::inRange(x,y, selectedUnit->speed)){
				showHover = TacticalGrid::inRange(x, y, selectedUnit->speed);
				if (IsKeyPressed(KEY_O)){
						TacticalGrid::waypoints.push_back({(float)x, (float)y});
						float cost = TacticalGrid::movementGrid[y][x].cost;
						selectedUnit->movePoints -= cost;
						std::cout << "Attempted to add waypoint\n";
						std::cout << TacticalGrid::waypoints[0] << std::endl;
				}
				if (TacticalGrid::waypoints.empty()){
					pathPreview = TacticalGrid::reconstructPath((int)selectedUnit->gridPosition.x, (int)selectedUnit->gridPosition.y, x, y, selectedUnit->speed);
					showPreview = !pathPreview.empty();
					if (showPreview && IsKeyPressed(KEY_M)){
						Movement::setPath(selectedUnit, pathPreview);
						std::cout << "Started unit moving!\n";
					} 
				} else {
					std::cout<<"waypoint exits\n";					
				}
							

			} else pathPreview.clear();
				
		} else pathPreview.clear();
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr){
				selectedUnit = TacticalGrid::getUnitAt(x,y);
				TacticalGrid::clearWaypoints();
				selectedUnit->movePoints = selectedUnit->speed;
			}
		}



		if (IsKeyPressed(KEY_T)){
			std::cout << selectedUnit->gridPosition.x << " , " << selectedUnit->gridPosition.y << std::endl;
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
				if (showHover) TacticalGrid::drawHoverHighlight(x, y, worldOrigin);
				if (showPreview) TacticalGrid::drawPathPreview(pathPreview, SKYBLUE);
				if (showWaypoints) TacticalGrid::drawWaypoints(worldOrigin);
		

			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

