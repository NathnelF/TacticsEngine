#include <raylib.h>
#include <raymath.h>
#include <iostream>

 struct entity {
	Vector3 position;
	Vector3 targetPosition;
	Color color;
	float size;
	Mesh mesh;
	Model model;
	BoundingBox collider;
	bool selected;
	bool isMoving;
};

int main() {
	InitWindow(800, 600, "Window");
	Camera camera = { 0 };
	camera.position = (Vector3){ 10.f, 50.0f, 10.f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 35.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	float cameraPitch = 45.0f;
	float cameraYaw = 45.0f;
	float cameraDistance = 25.0f;
	Vector3 cameraTarget = { 0.0f, 0.0f, 0.0f };

	// bool cubeSelected = false;
	// float cubeSize = 2.0f;
	// Mesh cubeMesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
	// Model cubeModel = LoadModelFromMesh(cubeMesh);
	// Vector3 Player1.position = { 0.0f, 1.0f, 0.0f, }; 
	// Vector3 targetPosition;
	// Color cubeColor = MAGENTA;
	// BoundingBox cubeCollider = GetMeshBoundingBox(cubeMesh);
	// cubeCollider.min = Vector3Add(cubeCollider.min, Player1.position);
	// cubeCollider.max = Vector3Add(cubeCollider.max, Player1.position);
	
	



	




	Vector3 planePosition = {0.0f, 0.0f, 0.0f};
	Vector2 planeSize = {50.0f, 50.0f};
	Color planeColor = GREEN;

	Mesh planeMesh = GenMeshPlane(planeSize.x, planeSize.y, 0, 0);
	Model planeModel = LoadModelFromMesh(planeMesh);
	BoundingBox planeCollider = GetMeshBoundingBox(planeMesh);
	planeCollider.min = Vector3Add(planeCollider.min, planePosition);
	planeCollider.max = Vector3Add(planeCollider.max, planePosition); 

	Vector3 planePtOne = { -25.0f, 0.0f, -25.0f };
	Vector3 planePtTwo = { 25.0f, 0.0f, -25.0f };
	Vector3 planePtThree = { 25.0f, 0.0f, 25.0f };
	Vector3 planePtFour = { -25.0f, 0.0f, 25.0f };

	float rotationSpeed = 0.2f;
	float moveSpeed = .05f;
	float t = 0.0f;
	float panSpeed = 3.0f;
	float zoomSpeed = 2.0f;
	float snapRotation = 4.0f;
	SetTargetFPS(60);

	entity Player1;
	Player1.position = (Vector3){ 0.0f, 1.0f, 0.0f };
	Player1.size = 2.0f;
	Player1.selected = false;
	Player1.color = MAGENTA;
	Player1.mesh = GenMeshCube(Player1.size, Player1.size, Player1.size);
	Player1.model = LoadModelFromMesh(Player1.mesh);
	Player1.collider = GetMeshBoundingBox(Player1.mesh);
	Player1.collider.min = (Vector3){ Player1.position.x - Player1.size / 2.0f,
                                      Player1.position.y - Player1.size / 2.0f,
                                      Player1.position.z - Player1.size / 2.0f };
	Player1.collider.max = (Vector3){ Player1.position.x + Player1.size / 2.0f,
                                      Player1.position.y + Player1.size / 2.0f,
                                      Player1.position.z + Player1.size / 2.0f };
	Player1.isMoving = false;

	
	entity Player2;
	Player2.position = (Vector3){ 20.0f, 1.0f, 10.0f };
	Player2.size = 2.0f;
	Player2.selected = false;
	Player2.color = BLUE;
	Player2.mesh = GenMeshCube(Player2.size, Player2.size, Player2.size);
	Player2.model = LoadModelFromMesh(Player2.mesh);
	Player2.collider = GetMeshBoundingBox(Player2.mesh);
	Player2.collider.min = (Vector3){ Player2.position.x - Player2.size / 2.0f,
                                      Player2.position.y - Player2.size / 2.0f,
                                      Player2.position.z - Player2.size / 2.0f };
	Player2.collider.max = (Vector3){ Player2.position.x + Player2.size / 2.0f,
                                      Player2.position.y + Player2.size / 2.0f,
                                      Player2.position.z + Player2.size / 2.0f };
	Player2.isMoving = false;

	entity* selectables[2] = {&Player1, &Player2};
	int numSelectables = sizeof(selectables) / sizeof(selectables[0]);
	std::cout << "LOOOK HEREEREER " << numSelectables << "\n";
	entity* currentlySelected = nullptr;



	while (!WindowShouldClose()){
		// std::cout << camera.position.x << "\n";
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
			Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
			RayCollision collision;
			// for (int i = 0; i < numSelectables; i++){
			// 	// selectables[i]->selected = false;
			// 	selectables[i]->color = MAGENTA;
			//
			// 
			if (currentlySelected != nullptr){
				currentlySelected->color = MAGENTA;
			}
			for (int i = 0; i < numSelectables; i++){
				//go through each selectable to check if there is a collision.
				collision = GetRayCollisionBox(mouseRay, selectables[i]->collider);
				if (collision.hit){
					// selectables[i]->selected = true;
					// selectables[i]->color = BLACK;
					currentlySelected = selectables[i];
					selectables[i]->color = BLACK;
					break;
				}
				else {
					currentlySelected = nullptr;
				}
			}
		
		}
		if (currentlySelected != nullptr){
			if (IsKeyDown(KEY_M)){
			Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
			RayCollision collision;
			collision = GetRayCollisionQuad(mouseRay, planePtOne, planePtTwo, planePtThree, planePtFour);
			if (collision.hit){
				//get plane coordinates and then move Player1.position to those coordinates
				currentlySelected->targetPosition = {collision.point.x, currentlySelected->position.y, collision.point.z};
				t = 0.0f;
				currentlySelected->isMoving = true;
				}
			}
		if (currentlySelected->isMoving){
			t += GetFrameTime() * moveSpeed;
			t = fmin(t, 1.0f);
			currentlySelected->position = Vector3Lerp(currentlySelected->position, currentlySelected->targetPosition, t);
			currentlySelected->collider.min = (Vector3){currentlySelected->position.x - 1.0f, currentlySelected->position.y - 1.0f, currentlySelected->position.z - 1.0f};
			currentlySelected->collider.max = (Vector3){currentlySelected->position.x + 1.0f, currentlySelected->position.y + 1.0f, currentlySelected->position.z + 1.0f};

			if (t >= 1.0f){
				currentlySelected->isMoving = false;
				currentlySelected->position = currentlySelected->targetPosition;
				}
			}
		}
		//ADD TAB KEY TO CYCLE THROUGH PLAYER CHARACTERS
		if (IsKeyDown(KEY_W)){
			Vector3 flatForward = Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f, camera.target.z - camera.position.z});
			cameraTarget = Vector3Add(cameraTarget, Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_S)){
			Vector3 flatForward = Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f, camera.target.z - camera.position.z});
			cameraTarget = Vector3Subtract(cameraTarget, Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_A)){
			Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(camera.target, camera.position), camera.up));
			cameraTarget = Vector3Subtract(cameraTarget, Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_D)){
			Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(Vector3Subtract(camera.target, camera.position), camera.up));
			cameraTarget = Vector3Add(cameraTarget, Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
		}
		if (IsKeyDown(KEY_K)){
			cameraDistance -= zoomSpeed;
			if (cameraDistance < 5.0f) cameraDistance = 5.0f;
		}
		if (IsKeyDown(KEY_J)){
			cameraDistance += zoomSpeed;
			if (cameraDistance > 60.0f) cameraDistance = 60.0f;
		}
		if (IsKeyDown(KEY_Q)){
			cameraYaw += snapRotation;
		}
		if (IsKeyDown(KEY_E)){
			cameraYaw -= snapRotation;
		}
		Vector3 newCameraOffset = { 0 };

		newCameraOffset.z -= cameraDistance;

		newCameraOffset = Vector3RotateByAxisAngle(newCameraOffset, (Vector3){1.0f, 0.0f, 0.0f}, cameraPitch * DEG2RAD);

		newCameraOffset = Vector3RotateByAxisAngle(newCameraOffset, (Vector3){0.0f, 1.0f, 0.0f}, cameraYaw * DEG2RAD);

		camera.position = Vector3Add(cameraTarget, newCameraOffset);
		camera.target = cameraTarget;
		BeginDrawing();
			ClearBackground(SKYBLUE);
			BeginMode3D(camera);
				DrawPlane(planePosition, planeSize, planeColor);
				DrawBoundingBox(planeCollider, PINK);
				DrawGrid(50, 5.0f);
				DrawModel(Player1.model, Player1.position, 1.0f, Player1.color);
				DrawBoundingBox(Player1.collider, GREEN);
				DrawModel(Player2.model, Player2.position, 1.0f, Player2.color);
				DrawBoundingBox(Player2.collider, GREEN);
		
			EndMode3D();
		EndDrawing();
	}
	UnloadModel(Player1.model);
	UnloadModel(Player2.model);
	UnloadModel(planeModel);
	CloseWindow();
	return 0;
}
