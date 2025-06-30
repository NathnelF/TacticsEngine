#include <raylib.h>
#include <raymath.h>
#include <iostream>

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

	bool cubeSelected = false;
	float cubeSize = 2.0f;
	Mesh cubeMesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
	Model cubeModel = LoadModelFromMesh(cubeMesh);
	Vector3 cubePosition = { 0.0f, 1.0f, 0.0f, }; 
	Vector3 targetPosition;
	Color cubeColor = MAGENTA;
	BoundingBox cubeCollider = GetMeshBoundingBox(cubeMesh);
	cubeCollider.min = Vector3Add(cubeCollider.min, cubePosition);
	cubeCollider.max = Vector3Add(cubeCollider.max, cubePosition);


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
	bool is_moving = false;
	float panSpeed = 3.0f;
	float zoomSpeed = 2.0f;
	float snapRotation = 4.0f;
	SetTargetFPS(60);


	while (!WindowShouldClose()){
		// std::cout << camera.position.x << "\n";
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
			Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
			RayCollision collision;
			collision = GetRayCollisionBox(mouseRay, cubeCollider);
			if (collision.hit){
				cubeSelected = true;
				cubeColor = BLACK;
			}
			else {
				if (cubeSelected != false){
					cubeSelected = false;
					cubeColor = MAGENTA;
				} 
			}
		}
		if (IsKeyDown(KEY_M) && cubeSelected == true){
			Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
			RayCollision collision;
			collision = GetRayCollisionQuad(mouseRay, planePtOne, planePtTwo, planePtThree, planePtFour);
			if (collision.hit){
				//get plane coordinates and then move cubePosition to those coordinates
				targetPosition = {collision.point.x, cubePosition.y, collision.point.z};
				t = 0.0f;
				is_moving = true;
								

				

			}
			else { 
 				//outside of plane so nothing should happen
			}
			//functionality to get intersect between ray and ground plane;
			//
			//then we need to take the position at that intersection 
			//and move the cub to there. Maybe interpolate it a bit for smoothness.


		}
		if (is_moving){
			t += GetFrameTime() * moveSpeed;
			t = fmin(t, 1.0f);
			cubePosition = Vector3Lerp(cubePosition, targetPosition, t);
			cubeCollider.min = (Vector3){cubePosition.x - 1.0f, cubePosition.y - 1.0f, cubePosition.z - 1.0f};
			cubeCollider.max = (Vector3){cubePosition.x + 1.0f, cubePosition.y + 1.0f, cubePosition.z + 1.0f};

			if (t >= 1.0f){
				is_moving = false;
				cubePosition = targetPosition;
			}

		}
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
				DrawModel(cubeModel, cubePosition, 1.0f, cubeColor);
				DrawBoundingBox(cubeCollider, GREEN);
		
			EndMode3D();
		EndDrawing();
	}
	UnloadModel(cubeModel);
	UnloadModel(planeModel);
	CloseWindow();
	return 0;
}
