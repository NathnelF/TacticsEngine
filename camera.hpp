#pragma once

#include <raylib.h>


extern float cameraPitch;
extern float cameraYaw;
extern 	float cameraDistance;
extern 	Vector3 cameraTarget;
extern 	float moveSpeed;
extern 	float panSpeed;
extern 	float zoomSpeed;
extern 	float snapRotation;

void initCamera(Camera3D& camera, Vector3 initialCameraPos);
