
#include "input.hpp"
#include <raylib.h>
#include <raymath.h>

namespace Input {
	static CameraInput cameraInput;
	static MouseInput mouseInput;

	void updateInput(Camera3D& camera, Vector3 worldOrigin, int gridWidth, int gridHeight) {
		cameraInput = {};
		mouseInput = {};
		//pan movement
		 cameraInput.panForward = IsKeyDown(KEY_W);
		 cameraInput.panBack = IsKeyDown(KEY_S);
		 cameraInput.panLeft = IsKeyDown(KEY_A);
		 cameraInput.panRight = IsKeyDown(KEY_D);

		//zoom movement
		 cameraInput.up = IsKeyDown(KEY_K);
		 cameraInput.down = IsKeyDown(KEY_J);
		
		//rotate movement
		 cameraInput.rotateLeft = IsKeyDown(KEY_Q);
		 cameraInput.rotateRight = IsKeyDown(KEY_E);


		mouseInput.screenPosition = GetMousePosition();
		mouseInput.leftClicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
		mouseInput.rightClicked = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
		Ray mouseRay = GetMouseRay(mouseInput.screenPosition, camera);
		if (mouseRay.direction.y != 0) {
		    float t = -(mouseRay.position.y - worldOrigin.y) / mouseRay.direction.y;
		    if (t > 0) {
				Vector3 hitPoint = Vector3Add(mouseRay.position, Vector3Scale(mouseRay.direction, t));
				Vector3 relative = Vector3Subtract(hitPoint, worldOrigin);
                
				float gridX = relative.x / 4.0f; // Assuming 2.0f tile size
				float gridY = relative.z / 4.0f;
                
				int x = (int)(gridX + 0.5f);
				int y = (int)(gridY + 0.5f);
                
				if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
				    mouseInput.gridPosition = {(float)x, (float)y};
				    mouseInput.hasValidGridPos = true;
				} else {
				    mouseInput.gridPosition = {-1.0f, -1.0f};
				    mouseInput.hasValidGridPos = false;
				}
			    }
		}
	}
	
	const CameraInput& getCameraInput() {
		return cameraInput;
	}

	const MouseInput& getMouseInput() {
		return mouseInput;
	}
}
