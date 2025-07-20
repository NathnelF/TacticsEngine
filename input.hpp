
#pragma once

#include <raylib.h>

namespace Input {
	struct CameraInput {
		//pan movement
		bool panForward = false;
		bool panBack = false;
		bool panLeft = false;
		bool panRight = false;

		//zoom movement
		bool up = false;
		bool down = false;
		
		//rotate movement
		bool rotateLeft = false;
		bool rotateRight = false;
	};
	struct MouseInput {

        	Vector2 screenPosition = {0.0f, 0.0f};    // Raw mouse screen coords
		Vector2 gridPosition = {-1.0f, -1.0f};    // Grid coordinates (-1,-1 = invalid)
		bool leftClicked = false;
		bool rightClicked = false;
		bool hasValidGridPos = false;             // True if mouse is over valid grid tile
	};

	void updateInput(Camera3D& camera, Vector3 worldOrigin, int gridWidth, int gridHeight);

	const CameraInput& getCameraInput();
	const MouseInput& getMouseInput();
}
