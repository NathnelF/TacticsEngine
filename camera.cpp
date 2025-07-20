
#include "camera.hpp"

float cameraPitch = 45.0f;
float cameraYaw = 45.0f;
float cameraDistance = 45.0f;
Vector3 cameraTarget;
float moveSpeed = 10.0f;
float panSpeed = 3.0f;
float zoomSpeed = 2.0f;
float snapRotation = 4.0f;


void initCamera(Camera3D& camera, Vector3 initialCameraPos){
	camera.position = (Vector3){10.0f, 50.0f, 10.f};
	camera.target = initialCameraPos; 
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 35.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	cameraTarget = initialCameraPos;
}
