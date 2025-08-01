
#include "camera.hpp"

float cameraPitch = 45.0f;
float cameraYaw = 45.0f;
float cameraDistance = 60.0f;
Vector3 cameraTarget;
float moveSpeed = 10.0f;
float panSpeed = 3.0f;
float zoomSpeed = 2.0f;
float snapRotation = 2.5f;


void initCamera(Camera3D& camera, Vector3 initialCameraPos){
	camera.position = (Vector3){10.0f, 50.0f, 10.f};
	camera.target = initialCameraPos; 
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 35.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	cameraTarget = initialCameraPos;
	}

void updateCamera(Camera3D& camera, const Input::CameraInput& input){
	float deltaTime = GetFrameTime();

	// Handle movement input
	if (input.panForward) {
		Vector3 flatForward = Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
		camera.target.z - camera.position.z});
		cameraTarget = Vector3Add(
		cameraTarget,
		Vector3Scale(flatForward, panSpeed * 10.0f * deltaTime));
	}
	if (input.panBack) {
		Vector3 flatForward =
		Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
		camera.target.z - camera.position.z});
		cameraTarget = Vector3Subtract(
		cameraTarget,
		Vector3Scale(flatForward, panSpeed * 10.0f * deltaTime));
	}
	if (input.panLeft) {
		Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
		Vector3Subtract(camera.target, camera.position), camera.up));
		cameraTarget = Vector3Subtract(
		cameraTarget,
		Vector3Scale(cameraRight, panSpeed * 10.0f * deltaTime));
	}
	if (input.panRight) {
		Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
		Vector3Subtract(camera.target, camera.position), camera.up));
		cameraTarget = Vector3Add(
		cameraTarget,
		Vector3Scale(cameraRight, panSpeed * 10.0f * deltaTime));
	}

	// Handle zoom input
	if (input.down) {
		cameraDistance -= zoomSpeed;
		if (cameraDistance < 5.0f)
			cameraDistance = 5.0f;
		}
	if (input.up) {
		cameraDistance += zoomSpeed;
		if (cameraDistance > 80.0f)
			cameraDistance = 80.0f;
		}

	// Handle rotation input
	if (input.rotateLeft) {
		cameraYaw += snapRotation;
	}
	if (input.rotateRight) {
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
}
