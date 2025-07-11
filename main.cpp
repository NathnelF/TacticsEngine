#include "Entity.hpp"
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>


 
int main() {
  InitWindow(800, 600, "Window");
  Camera camera = {0};
  camera.position = (Vector3){10.f, 50.0f, 10.f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 35.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  float cameraPitch = 45.0f;
  float cameraYaw = 45.0f;
  float cameraDistance = 45.0f;
  Vector3 cameraTarget = {0.0f, 0.0f, 0.0f};

  Vector3 planePosition = {0.0f, 0.0f, 0.0f};
  Vector2 planeSize = {50.0f, 50.0f};
  Color planeColor = GREEN;
  Mesh planeMesh = GenMeshPlane(planeSize.x, planeSize.y, 0, 0);
  Model planeModel = LoadModelFromMesh(planeMesh);
  BoundingBox planeCollider = GetMeshBoundingBox(planeMesh);
  planeCollider.min = Vector3Add(planeCollider.min, planePosition);
  planeCollider.max = Vector3Add(planeCollider.max, planePosition);

  Vector3 planePtOne = {-25.0f, 0.0f, -25.0f};
  Vector3 planePtTwo = {25.0f, 0.0f, -25.0f};
  Vector3 planePtThree = {25.0f, 0.0f, 25.0f};
  Vector3 planePtFour = {-25.0f, 0.0f, 25.0f};

  float rotationSpeed = 0.2f;
  float moveSpeed = .05f;
  float t = 0.0f;
  float panSpeed = 3.0f;
  float zoomSpeed = 2.0f;
  float snapRotation = 4.0f;
  SetTargetFPS(60);

  std::vector<std::unique_ptr<GameEntity>> AllEntities;
  AllEntities.push_back(
      std::make_unique<GameEntity>(Vector3{0.0f, 1.0f, 0.0f}, MAGENTA));
  AllEntities.push_back(
      std::make_unique<GameEntity>(Vector3{5.0f, 1.0f, 0.0f}, BLUE));
  GameEntity *currentlySelected = nullptr;

  while (!WindowShouldClose()) {
    // std::cout << camera.position.x << "\n";
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
      RayCollision collision;
      if (currentlySelected != nullptr) {
        currentlySelected->color = MAGENTA;
      }
      for (const auto &entityPtr : AllEntities) {
        // go through each selectable to check if there is a collision.
        collision = GetRayCollisionBox(mouseRay, entityPtr->collider);
        if (collision.hit) {
          currentlySelected = entityPtr.get();
          entityPtr->color = BLACK;
          break;
        } else {
          currentlySelected = nullptr;
        }
      }
    }
    if (currentlySelected != nullptr) {
      if (IsKeyDown(KEY_M)) {
        Ray mouseRay = GetMouseRay(GetMousePosition(), camera);
        RayCollision collision;
        collision = GetRayCollisionQuad(mouseRay, planePtOne, planePtTwo,
                                        planePtThree, planePtFour);
        if (collision.hit) {
          // get plane coordinates and then move Player1.position to those
          // coordinates
          Vector3 newTargetPosition = {collision.point.x,
                                       currentlySelected->position.y,
                                       collision.point.z};
          currentlySelected->InitiateMove(newTargetPosition);
        }
      }
    }
    // Update position loop for each entity
    for (const auto &entityPtr : AllEntities) {
      entityPtr->UpdateMove(moveSpeed);
    }
    // ADD TAB KEY TO CYCLE THROUGH PLAYER CHARACTERS
    if (IsKeyDown(KEY_W)) {
      Vector3 flatForward =
          Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
                                     camera.target.z - camera.position.z});
      cameraTarget = Vector3Add(
          cameraTarget,
          Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
    }
    if (IsKeyDown(KEY_S)) {
      Vector3 flatForward =
          Vector3Normalize((Vector3){camera.target.x - camera.position.x, 0.0f,
                                     camera.target.z - camera.position.z});
      cameraTarget = Vector3Subtract(
          cameraTarget,
          Vector3Scale(flatForward, panSpeed * 10.0f * GetFrameTime()));
    }
    if (IsKeyDown(KEY_A)) {
      Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
          Vector3Subtract(camera.target, camera.position), camera.up));
      cameraTarget = Vector3Subtract(
          cameraTarget,
          Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
    }
    if (IsKeyDown(KEY_D)) {
      Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(
          Vector3Subtract(camera.target, camera.position), camera.up));
      cameraTarget = Vector3Add(
          cameraTarget,
          Vector3Scale(cameraRight, panSpeed * 10.0f * GetFrameTime()));
    }
    if (IsKeyDown(KEY_K)) {
      cameraDistance -= zoomSpeed;
      if (cameraDistance < 5.0f)
        cameraDistance = 5.0f;
    }
    if (IsKeyDown(KEY_J)) {
      cameraDistance += zoomSpeed;
      if (cameraDistance > 60.0f)
        cameraDistance = 60.0f;
    }
    if (IsKeyDown(KEY_Q)) {
      cameraYaw += snapRotation;
    }
    if (IsKeyDown(KEY_E)) {
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
    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode3D(camera);
    DrawPlane(planePosition, planeSize, planeColor);
    DrawBoundingBox(planeCollider, PINK);
    DrawGrid(50, 5.0f);
    for (const auto &entityPtr : AllEntities) {
      entityPtr->Draw(); // Each entity draws itself
    }

    EndMode3D();
    EndDrawing();
  }
  UnloadModel(planeModel);
  CloseWindow();
  return 0;
}
