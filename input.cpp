
#include "input.hpp"

namespace Input {
	static CameraInput cameraInput;

	void updateInput() {
		cameraInput = {};
		//pan movement
		 cameraInput.panForward = IsKeyDown(KEY_W);
		 cameraInput.panBack = IsKeyDown(KEY_S);
		 cameraInput.panLeft = IsKeyDown(KEY_A);
		 cameraInput.panRight = IsKeyDown(KEY_D);

		//zoom movement
		 cameraInput.up = IsKeyDown(KEY_K);
		 cameraInput.down = IsKeyDown(KEY_J);
		
		//rotate movement
		 cameraInput.rotateLeft = IsKeyPressed(KEY_Q);
		 cameraInput.rotateRight = IsKeyPressed(KEY_E);
		
	}
	
	const CameraInput& getCameraInput() {
		return cameraInput;
	}
}
