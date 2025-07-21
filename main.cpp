
#include <raylib.h>
#include "camera.hpp"
#include "input.hpp"
#include "grid.hpp"
#include <iostream>



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

	while (!WindowShouldClose()){
		
		Input::updateInput(camera, worldOrigin, GRID_WIDTH, GRID_HEIGHT);
		updateCamera(camera, Input::getCameraInput());

		const auto& mouseInput = Input::getMouseInput();
		int x = (int)mouseInput.gridPosition.x;
		int y = (int)mouseInput.gridPosition.y;


		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr) selectedUnit = TacticalGrid::getUnitAt(x,y);
		}

		if (selectedUnit){
			TacticalGrid::calculateMovementRange(selectedUnit->id);
		}

		if (selectedUnit && mouseInput.hasValidGridPos && IsKeyPressed(KEY_M)){
		     //move to location 
		}
		

		if (IsKeyPressed(KEY_T)){
			std::cout << selectedUnit->gridPosition.x << " , " << selectedUnit->gridPosition.y << std::endl;
		}


		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				TacticalGrid::drawTerrain(worldOrigin);
				TacticalGrid::setHighlight(selectedUnit->id);
				TacticalGrid::drawUnits(worldOrigin);
				TacticalGrid::drawMovementOverlay(worldOrigin);
			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

