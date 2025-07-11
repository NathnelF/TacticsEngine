#include "Entity.hpp"
#include <raylib.h>
#include <raymath.h>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


// constructor 
	GameEntity::GameEntity(Vector3 initialPosition, Color initialColor){
		size = 2.0f;
		t = 0.0f;
		mesh = GenMeshCube(size, size, size);
		model = LoadModelFromMesh(mesh);
		collider = GetMeshBoundingBox(mesh);
		selected = false;
		isMoving = false;
		position = initialPosition;
		color = initialColor;
		collider.min = (Vector3){ position.x - size / 2.0f,
		     position.y - size / 2.0f,
		     position.z - size / 2.0f };
		collider.max = (Vector3){ position.x + size / 2.0f,
		     position.y + size / 2.0f,
		     position.z + size / 2.0f };


	}
	// add destructor?
	GameEntity::~GameEntity() {
		UnloadModel(model);
	}


	void GameEntity::InitiateMove(Vector3 newTargetPosition){
		if (!isMoving){
			targetPosition = newTargetPosition;
			std::cout << "aiming for: " << targetPosition << "from : " << position << std::endl;
			t = 0.0f;
			isMoving = true;
		}
	}

	//methods
	void GameEntity::UpdateMove(float moveSpeed){
		if (isMoving){
			t += GetFrameTime() * moveSpeed;
			t = fmin(t, 1.0f);
			position = Vector3Lerp(position, targetPosition, t);
			collider.min = (Vector3){position.x - 1.0f, position.y - 1.0f, position.z - 1.0f};
			collider.max = (Vector3){position.x + 1.0f, position.y + 1.0f, position.z + 1.0f};
			if (t >= 0.98f){
				position = targetPosition;
				isMoving = false;
				collider.min = (Vector3){position.x - 1.0f, position.y - 1.0f, position.z - 1.0f};
				collider.max = (Vector3){position.x + 1.0f, position.y + 1.0f, position.z + 1.0f};
				std::cout << "Reached destination" << t << std::endl;

			} else {
				Vector3 diff = Vector3Subtract(targetPosition, position);
				if (abs(diff.x) < 0.019 && abs(diff.y) < 0.019 && abs(diff.z) < 0.019){
					position = targetPosition;
					isMoving = false;
					t = 0.0f;
					collider.min = (Vector3){position.x - 1.0f, position.y - 1.0f, position.z - 1.0f};
					collider.max = (Vector3){position.x + 1.0f, position.y + 1.0f, position.z + 1.0f};
					std::cout << "Reached destination!: " << position << std::endl;
				}

			}
		}
	}

	void GameEntity::Draw(){
		DrawModel(model, position, 1.0f, color);
		DrawBoundingBox(collider, GREEN);
	}
