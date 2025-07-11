#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

class GameEntity{
public:
	// Fields
	Vector3 position;
	Vector3 targetPosition;
	float t;
	Color color;
	float size;
	Mesh mesh;
	Model model;
	BoundingBox collider;
	bool selected;
	bool isMoving;

	GameEntity(Vector3 initialPosition, Color initialColor);

	~GameEntity();

	void InitiateMove(Vector3 newTargetPositon);
	void UpdateMove(float moveSpeed);
	void Draw();
};

#endif
