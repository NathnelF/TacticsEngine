
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

	int timer = 0;
	bool showHover = false;

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
			TacticalGrid::calculateMovementRange(selectedUnit->id);
		}

		if (mouseInput.hasValidGridPos && selectedUnit){
			showHover = TacticalGrid::inRange(x, y, selectedUnit->speed);
			if (IsKeyPressed(KEY_M)){
				std::vector<Vector2> path = TacticalGrid::reconstructPath((int)selectedUnit->gridPosition.x, (int)selectedUnit->gridPosition.y, x, y);
				if (!path.empty()){
					for (auto& cord : path){
						std::cout << cord << " ";
					}
					std::cout << std::endl;
					Movement::setPath(selectedUnit, path);
					std::cout << "Started unit moving!\n";
				}
			}
		}
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr) selectedUnit = TacticalGrid::getUnitAt(x,y);
		}


		if (selectedUnit && mouseInput.hasValidGridPos && IsKeyPressed(KEY_M)){
		     //move to location 
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
				TacticalGrid::drawMovementOverlay(worldOrigin);
				if (showHover) TacticalGrid::drawHoverHighlight(x, y, worldOrigin);
		

			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

