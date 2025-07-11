#include "Entity.hpp"
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Tile {
Vector3 worldPosition;
Vector2 gridPosition;
float height;
bool hasUnit;
bool traversable;
Color color;
BoundingBox bounds;
};

Tile* previouslyHoveredTile = NULL;
const Color tileDefaultColor = LIGHTGRAY;

#define GRID_WIDTH 4
#define GRID_HEIGHT 4
#define TILE_SIZE 4

Tile grid[GRID_WIDTH][GRID_HEIGHT];

void RenderGrid(const Tile grid[GRID_WIDTH][GRID_HEIGHT]){
for (int x = 0; x < GRID_WIDTH; x++){
	for (int y = 0; y < GRID_HEIGHT; y++){
		Tile currentTile = grid[x][y];
		DrawCubeWires(currentTile.worldPosition, TILE_SIZE, 0.1f, TILE_SIZE, currentTile.color);
	}
}
}


int main() {
InitWindow(800, 600, "Window");
Camera camera = {0};
camera.position = (Vector3){10.f, 50.0f, 10.f};
camera.target = (Vector3){0.0f, 0.0f, 0.0f};
camera.up = (Vector3){0.0f, 1.0f, 0.0f};
camera.fovy = 35.0f;
camera.projection = CAMERA_PERSPECTIVE;

float cameraPitch = 45.0f;
float cameraYaw = 45.0f;
float cameraDistance = 45.0f;
Vector3 cameraTarget = {0.0f, 0.0f, 0.0f};

Vector3 planePosition = {0.0f, 0.0f, 0.0f};
Vector2 planeSize = {50.0f, 50.0f};
Color planeColor = GREEN;
Mesh planeMesh = GenMeshPlane(planeSize.x, planeSize.y, 0, 0);
Model planeModel = LoadModelFromMesh(planeMesh);
BoundingBox planeCollider = GetMeshBoundingBox(planeMesh);
planeCollider.min = Vector3Add(planeCollider.min, planePosition);
planeCollider.max = Vector3Add(planeCollider.max, planePosition);

Vector3 planePtOne = {-25.0f, 0.0f, -25.0f};
Vector3 planePtTwo = {25.0f, 0.0f, -25.0f};
Vector3 planePtThree = {25.0f, 0.0f, 25.0f};
Vector3 planePtFour = {-25.0f, 0.0f, 25.0f};

float rotationSpeed = 0.2f;
float moveSpeed = .05f;
float t = 0.0f;
float panSpeed = 3.0f;
float zoomSpeed = 2.0f;
float snapRotation = 4.0f;
SetTargetFPS(60);

std::vector<std::unique_ptr<GameEntity>> AllEntities;
AllEntities.push_back(
std::make_unique<GameEntity>(Vector3{0.0f, 1.0f, 0.0f}, MAGENTA));
AllEntities.push_back(
std::make_unique<GameEntity>(Vector3{5.0f, 1.0f, 0.0f}, BLUE));
GameEntity *currentlySelected = nullptr;

for (int x = 0; x < GRID_WIDTH; x++){
	for (int y = 0; y < GRID_HEIGHT; y++){
		grid[x][y].height = 0.0f; //keep it flat for now
		grid[x][y].gridPosition = (Vector2){ (float)x, (float)y }; 
		grid[x][y].worldPosition = (Vector3){
			(float)x * TILE_SIZE - TILE_SIZE/2,
			0.0f,
			(float)y * TILE_SIZE - TILE_SIZE/2
		};
		grid[x][y].traversable = true;
		grid[x][y].hasUnit = false;
		grid[x][y].color = LIGHTGRAY;
		grid[x][y].bounds = {
			(Vector3){(float)x * TILE_SIZE - TILE_SIZE, 0.0f, (float)y * TILE_SIZE - TILE_SIZE},
			(Vector3){(float)x * TILE_SIZE, 0.0f, (float)y * TILE_SIZE},
		};
	}
}

while (!WindowShouldClose()) {
	// std::cout << camera.position.x << "\n";
	
	Ray mousePos = GetMouseRay(GetMousePosition(), camera);
	Tile* hoveredTile = NULL;
	
	if (previouslyHoveredTile != NULL){
			previouslyHoveredTile->color = tileDefaultColor;
			previouslyHoveredTile = NULL;
		}
	for (int x = 0; x < GRID_WIDTH; x++){
			for (int y = 0; y < GRID_HEIGHT; y++){
				Tile* currentTile = &grid[x][y];
				RayCollision gridCollision = GetRayCollisionBox(mousePos, currentTile->bounds);

				if (gridCollision.hit){
					hoveredTile = currentTile;
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
			currentlySelected->color = MAGENTA;
		}
		for (const auto &entityPtr : AllEntities) {
			// go through each selectable to check if there is a collision.
			collision = GetRayCollisionBox(mouseRay, entityPtr->collider);
			if (collision.hit) {
				currentlySelected = entityPtr.get();
				entityPtr->color = BLACK;
				break;
			} else {
				currentlySelected = nullptr;
			}
		}
	}
	if (currentlySelected != nullptr) {
		if (IsKeyDown(KEY_M) && hoveredTile != NULL) {
			Vector3 newTarget = (Vector3){
					hoveredTile->worldPosition.x,
					1.0f,
					hoveredTile->worldPosition.z
				}; 
			currentlySelected->InitiateMove(newTarget);
		}
	}
	// Update position loop for each entity
	for (const auto &entityPtr : AllEntities) {
		entityPtr->UpdateMove(moveSpeed);
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
	DrawPlane(planePosition, planeSize, planeColor);
	DrawBoundingBox(planeCollider, PINK);
	// DrawGrid(50, 5.0f);
	RenderGrid(grid);
	for (const auto &entityPtr : AllEntities) {
	entityPtr->Draw(); // Each entity draws itself
	}

	EndMode3D();
	EndDrawing();
	}
	UnloadModel(planeModel);
	CloseWindow();
	return 0;
}
