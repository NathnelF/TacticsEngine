
#include "Entity.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>



bool operator<(const Vector2& a, const Vector2& b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

Tile* previouslyHoveredTile = NULL;

int main() {
	Grid grid(GRID_WIDTH, GRID_HEIGHT);

	InitWindow(800, 600, "Window");


	std::vector<std::unique_ptr<GameEntity>> AllEntities;
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(1,1).worldPosition.x, 1.2f, grid.getTile(1,1).worldPosition.z }, MAGENTA, grid.getTilePointer(1,1)));
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(1,0).worldPosition.x, 1.2f, grid.getTile(1,0).worldPosition.z }, BLUE, grid.getTilePointer(1,0)));
	GameEntity *currentlySelected = nullptr;
	std::unique_ptr<GameEntity>& firstEntityPtr = AllEntities.at(0);
	Vector3 initialCameraPos = (Vector3){(float)firstEntityPtr->position.x, 1.0f, (float)firstEntityPtr->position.z};

		
	Camera3D camera = {0};
	camera.position = (Vector3){10.0f, 50.0f, 10.f};
	camera.target = initialCameraPos; 
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 35.0f;
	camera.projection = CAMERA_PERSPECTIVE;


	float cameraPitch = 45.0f;
	float cameraYaw = 45.0f;
	float cameraDistance = 45.0f;
	Vector3 cameraTarget = initialCameraPos;

	float rotationSpeed = 0.2f;
	float moveSpeed = 10.0f;
	float t = 0.0f;
	float panSpeed = 3.0f;
	float zoomSpeed = 2.0f;
	float snapRotation = 4.0f;
	SetTargetFPS(60);

	float lastFrame = GetTime();

	while (!WindowShouldClose()) {
		// std::cout << camera.position.x << "\n";
		float currentFrame = GetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		Ray mousePos = GetMouseRay(GetMousePosition(), camera);
		Tile* hoveredTile = NULL;
		
		if (previouslyHoveredTile != NULL){
				previouslyHoveredTile->color = tileDefaultColor;
				previouslyHoveredTile = NULL;
			}
		for (int x = 0; x < GRID_WIDTH; x++){
			for (int y = 0; y < GRID_HEIGHT; y++){
				Tile& currentTile = grid.getTile(x,y);
				RayCollision gridCollision = GetRayCollisionBox(mousePos, currentTile.bounds);

				if (gridCollision.hit){
					hoveredTile = &currentTile;
					break;
				}

					
				}
				if (hoveredTile != NULL){
					break;
				}
			}
		if (hoveredTile != NULL){
			hoveredTile->color = YELLOW;
		}

		previouslyHoveredTile = hoveredTile;
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
			RayCollision collision;
			if (currentlySelected != nullptr) {
				currentlySelected->currentColor = currentlySelected->defaultColor;
			}
			for (const auto &entityPtr : AllEntities) {
				// go through each selectable to check if there is a collision.
				collision = GetRayCollisionBox(mouseRay, entityPtr->collider);
				if (collision.hit) {
					currentlySelected = entityPtr.get();
					entityPtr->currentColor = BLACK;
					break;
				} else {
					currentlySelected = nullptr;
				}
			}
		}
		if (currentlySelected != nullptr) {
			if (IsKeyPressed(KEY_M) && hoveredTile != NULL && currentlySelected != NULL) { 
				std::list<Tile> path = grid.getPath(*currentlySelected->tile, *hoveredTile); 
				currentlySelected->SetPath(path);
			}
		}
		// Update position loop for each entity
		for (const auto &entityPtr : AllEntities) {
			entityPtr->UpdateMove(moveSpeed, deltaTime);
		}
		// ADD TAB KEY TO CYCLE THROUGH PLAYER CHARACTERS


		if (IsKeyDown(KEY_W)) {
			Vector3 flatForward = Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
						     camera.target.z - camera.position.z});
			cameraTarget = Vector3Add(
			  cameraTarget,
			  Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_S)) {
			Vector3 flatForward =
			  Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
						     camera.target.z - camera.position.z});
			cameraTarget = Vector3Subtract(
			  cameraTarget,
			  Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_A)) {
			Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
			  Vector3Subtract(camera.target, camera.position), camera.up));
			cameraTarget = Vector3Subtract(
			  cameraTarget,
			  Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_D)) {
			Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
			  Vector3Subtract(camera.target, camera.position), camera.up));
			cameraTarget = Vector3Add(
			  cameraTarget,
			  Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_K)) {
			cameraDistance -= zoomSpeed;
		if (cameraDistance < 5.0f)
			cameraDistance = 5.0f;
		}
		if (IsKeyDown(KEY_J)) {
			cameraDistance += zoomSpeed;
		if (cameraDistance > 60.0f)
			cameraDistance = 60.0f;
		}
		if (IsKeyDown(KEY_Q)) {
			cameraYaw += snapRotation;
		}
		if (IsKeyDown(KEY_E)) {
			cameraYaw -= snapRotation;
		}
		if (IsKeyPressed(KEY_T) && currentlySelected != nullptr){
			std::cout << "My current tile is (" << currentlySelected->tile->worldPosition.x << " , " << currentlySelected->tile->worldPosition.z << ")\n";
		}

		Vector3 newCameraOffset = {0};

		newCameraOffset.z -= cameraDistance;

		newCameraOffset = Vector3RotateByAxisAngle(
		newCameraOffset, (Vector3){1.0f, 0.0f, 0.0f}, cameraPitch * DEG2RAD);

		newCameraOffset = Vector3RotateByAxisAngle(
		newCameraOffset, (Vector3){0.0f, 1.0f, 0.0f}, cameraYaw * DEG2RAD);

		camera.position = Vector3Add(cameraTarget, newCameraOffset);
		camera.target = cameraTarget;
		BeginDrawing();
			ClearBackground(SKYBLUE);
			BeginMode3D(camera);
				grid.RenderGrid();
				for (const auto &entityPtr : AllEntities) {
					entityPtr->Draw(); // Each entity draws itself
				}

			EndMode3D();
		EndDrawing();
		}
	CloseWindow();
	return 0;
}

