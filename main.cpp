
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



int main() {
	InitWindow(800, 600, "Window");
	Grid grid(GRID_WIDTH, GRID_HEIGHT);

	Vector2 unit1_start = (Vector2){10, 10};
	Vector2 unit2_start = (Vector2){10, 11};
	Vector2 unit3_start = (Vector2){10, 12};
	Vector2 unit4_start = (Vector2){10, 13};
	Vector2 unit5_start = (Vector2){10, 14};

	std::vector<std::unique_ptr<GameEntity>> AllEntities;
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(unit1_start.x, unit1_start.y).worldPosition.x, 0.2f, grid.getTile(unit1_start.x, unit1_start.y).worldPosition.z }, MAGENTA, grid.getTilePointer(unit1_start.x,unit1_start.y), 7));
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(unit2_start.x,unit2_start.y).worldPosition.x, 0.2f, grid.getTile(unit2_start.x,unit2_start.y).worldPosition.z }, BLUE, grid.getTilePointer(unit2_start.x, unit2_start.y), 7));
	AllEntities.push_back(std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(unit3_start.x,unit3_start.y).worldPosition.x, 0.2f, grid.getTile(unit3_start.x,unit3_start.y).worldPosition.z }, PURPLE, grid.getTilePointer(unit3_start.x, unit3_start.y), 6));
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(unit4_start.x,unit4_start.y).worldPosition.x, 0.2f, grid.getTile(unit4_start.x,unit4_start.y).worldPosition.z }, DARKGREEN, grid.getTilePointer(unit4_start.x, unit4_start.y), 6));
	AllEntities.push_back(
	std::make_unique<GameEntity>(grid, Vector3{ grid.getTile(unit5_start.x,unit5_start.y).worldPosition.x, 0.2f, grid.getTile(unit5_start.x,unit5_start.y).worldPosition.z }, ORANGE, grid.getTilePointer(unit5_start.x, unit5_start.y), 6));

	GameEntity *currentlySelected = nullptr;
	for (const auto& ent : AllEntities){
		ent->currentTile->addEntity(ent.get());
	}
	std::unique_ptr<GameEntity>& firstEntityPtr = AllEntities.at(0);
	Vector3 initialCameraPos = (Vector3){(float)firstEntityPtr->position.x, 1.0f, (float)firstEntityPtr->position.z};
	Tile* previouslyHoveredTile = NULL;

	std::list<Tile> pathPreview;
	bool showPath;

	std::vector<Tile> waypoints;
	std::vector<Tile> movementRange;
	bool showMovementRange = false;

	int numPlayers = AllEntities.size();
	int entityIndex = 0;

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
		Tile* currentWaypoint = NULL;
	
		
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
			if (currentlySelected != nullptr && !currentlySelected->isMoving){
				if (!waypoints.empty()){
					pathPreview = grid.getWaypointPath(*currentlySelected->currentTile, waypoints, *hoveredTile); //from entity to last waypoint
					// std::list<Tile> secondSegmentPreview = grid.getPath(waypoints.back(), *hoveredTile); // from last waypoint to hovered Tile.
					// pathPreview.splice(pathPreview.end(), secondSegmentPreview);


					showPath = !pathPreview.empty();
					//the problem is that this just shows the path from the entity to the last waypoint
					//I need it to show the the path from the last waypoint to the hovered tile as well.


				} else {
					pathPreview = grid.getPath(*currentlySelected->currentTile, *hoveredTile);
					showPath = !pathPreview.empty();

				}
			} else {
				showPath = false; 
			}
		} else {
			showPath = false;
		}

		previouslyHoveredTile = hoveredTile;
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON && hoveredTile != NULL)) {
			if (hoveredTile->getEntity() != nullptr){
				if (currentlySelected != nullptr){
					currentlySelected->currentColor = currentlySelected->defaultColor;
				}
				currentlySelected = hoveredTile->getEntity();
				currentlySelected -> currentColor = BLACK;
			} else {
				if (currentlySelected != nullptr){
					currentlySelected->currentColor = currentlySelected->defaultColor;
					currentlySelected = nullptr;
				}	
				currentlySelected = nullptr;
			}
			if (!waypoints.empty()){
				waypoints.clear(); //clear waypoints on entity change.
			}
			if (!movementRange.empty()){
				movementRange.clear();
			}
		}
		if (currentlySelected != nullptr) {
			movementRange = currentlySelected->movementPreview();
			if (!movementRange.empty() && currentlySelected->isMoving == false){
				showMovementRange = true;
			} else {
				showMovementRange = false;
			}
			// for (const auto& tile : movementRange){
			// 	std::cout << "(" << tile.gridPosition.x << " , " << tile.gridPosition.y << " )"  << std::endl;
			// }
			if (IsKeyPressed(KEY_M) && hoveredTile != NULL && !IsKeyDown(KEY_LEFT_CONTROL)) { 
				if (waypoints.empty()){
					std::list<Tile> path = grid.getPath(*currentlySelected->currentTile, *hoveredTile);
					if (!path.empty()){
						currentlySelected->SetPath(path);
						currentlySelected->currentTile->entity = NULL;
						hoveredTile->entity=currentlySelected;
						hoveredTile->hasUnit = true;
					}
				} else {
					std::list<Tile> path = grid.getWaypointPath(*currentlySelected->currentTile, waypoints, *hoveredTile);
					if (!path.empty()){
						currentlySelected->SetPath(path);
						currentlySelected->currentTile->entity = NULL;
						hoveredTile->entity=currentlySelected;
						hoveredTile->hasUnit = true;
						waypoints.clear();
					}
				}
						
			}
			if (IsKeyPressed(KEY_M) && hoveredTile != NULL && IsKeyDown(KEY_LEFT_CONTROL)){
				//this is for waypoint movement.
				//We should store the path from each waypoint to the next, and then initate and complete movement for each?
				//A path of paths perhaps?
				//Oh yeah we should get each path and add them together and when you then press the move key it will initiate a move for that conjoined path.
			}
			if (IsKeyPressed(KEY_O) && hoveredTile != NULL){
				if (!hoveredTile->hasUnit){
					currentWaypoint = hoveredTile;
					// std::cout << "Set current waypoint!\n";
					waypoints.push_back(*currentWaypoint);

				}
						}
		}
		// Update position loop for each entity
		for (const auto &entityPtr : AllEntities) {
			entityPtr->UpdateMove(moveSpeed, deltaTime);
		}
		// ADD TAB KEY TO CYCLE THROUGH PLAYER CHARACTERS
		if (IsKeyPressed(KEY_TAB)){
			// std::cout << "Current entity index: " << entityIndex << " out of " << numPlayers << " entities.\n";
			if (currentlySelected != nullptr){
				currentlySelected->currentColor = currentlySelected->defaultColor;
			}
			if (entityIndex == numPlayers-1){
				entityIndex = 0;
				// std::cout << "Entity index after change " << entityIndex << std::endl;
				currentlySelected = AllEntities.at(entityIndex).get();
				currentlySelected->currentColor = BLACK;
			} else {
				entityIndex++;
				// std::cout << "Entity index after change " << entityIndex << std::endl;
				currentlySelected = AllEntities.at(entityIndex).get();
				currentlySelected->currentColor = BLACK;
			}
			if (!waypoints.empty()){
				waypoints.clear();
			}
			if (!movementRange.empty()){
				movementRange.clear();
			}
		
		}


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
			std::cout << "My current tile is (" << currentlySelected->currentTile->gridPosition.x << " , " << currentlySelected->currentTile->gridPosition.y << ")\n";
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
				if (showPath){
					grid.RenderPath(pathPreview, PINK);
				}
				if (showMovementRange){
				std::vector<TileEdge> outline = grid.getMovementRangeOutline(movementRange);
					grid.RenderOutline(outline);
				}
				if (!waypoints.empty()){
					for (const auto& waypoint : waypoints){
						grid.RenderWaypoints(waypoint, DARKGREEN);
					}
				}

			EndMode3D();
		EndDrawing();
		}
	CloseWindow();
	return 0;
}

