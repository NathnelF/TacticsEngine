
#include <raylib.h>
#include "camera.hpp"
#include "input.hpp"
#include "grid.hpp"

int main(){
	InitWindow(1200, 800, "Camera Test");
	Camera3D camera = {0};
	Vector3 initialCameraPos = {0.0f, 0.0f, 0.0f};
	initCamera(camera, initialCameraPos);
	SetTargetFPS(60);

	Vector3 worldOrigin = {0.0f, 0.0f, 0.0f};
	TacticalGrid::initGrid();

	while (!WindowShouldClose()){
		
		Input::updateInput();
		updateCamera(camera, Input::getCameraInput());

		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				DrawPlane({0.0f, 0.0f, 0.0f}, {20.0f, 20.0f}, GREEN);
				TacticalGrid::drawTerrain(worldOrigin);
			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
