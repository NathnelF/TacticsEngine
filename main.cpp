
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
			TacticalGrid::calculateMovementRange(selectedUnit->id);
			showRange = !selectedUnit->isMoving;
		}

		if (mouseInput.hasValidGridPos && selectedUnit){
			if (TacticalGrid::inRange(x,y, selectedUnit->speed)){
				showHover = TacticalGrid::inRange(x, y, selectedUnit->speed);
				pathPreview = TacticalGrid::reconstructPath((int)selectedUnit->gridPosition.x, (int)selectedUnit->gridPosition.y, x, y, selectedUnit->speed);
				showPreview = !pathPreview.empty();
				if (showPreview && IsKeyPressed(KEY_M)){
					Movement::setPath(selectedUnit, pathPreview);
					std::cout << "Started unit moving!\n";
				} 
				

			} else pathPreview.clear();
				
		} else pathPreview.clear();
		if (mouseInput.leftClicked && mouseInput.hasValidGridPos){
			printf("Clicked grid tile: (%d, %d)\n", x, y);
			std::cout << TacticalGrid::isUnitAt(x,y) << " unit at ( " << x << " , " << y << ")\n";
			if (TacticalGrid::getUnitAt(x,y) != nullptr) selectedUnit = TacticalGrid::getUnitAt(x,y);
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
		

			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

