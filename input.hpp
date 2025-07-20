
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

	void updateInput();

	const CameraInput& getCameraInput();
}
