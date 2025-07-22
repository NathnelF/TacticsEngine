
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
	TacticalGrid::calculateMovementRange(selectedUnit->id);

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
			 // std::cout <<"test\n";
			TacticalGrid::calculateMovementRange(selectedUnit->id);	
			if (TacticalGrid::inRange(x,y, selectedUnit->speed)){
				showHover = TacticalGrid::inRange(x, y, selectedUnit->speed);
				pathPreview = TacticalGrid::reconstructPath((int)selectedUnit->gridPosition.x, (int)selectedUnit->gridPosition.y, x, y);
				showPreview = !pathPreview.empty();
				if (showPreview && IsKeyPressed(KEY_M)){ 
					Movement::setPath(selectedUnit, pathPreview);
					std::cout << "Started unit moving!\n";
					selectedUnit->movePoints = 0.0f;
					TacticalGrid::calculateMovementRange(selectedUnit->id);
					pathPreview.clear();
				} 
			} else pathPreview.clear();
		} else pathPreview.clear();
		
		

							


			
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			std::cout << "move cost from og pos " << TacticalGrid::movementGrid[y][x].cost << std::endl;
			if (TacticalGrid::getUnitAt(x,y) != nullptr){
				selectedUnit = TacticalGrid::getUnitAt(x,y);

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
		

			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

