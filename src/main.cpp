
#include "abilities.hpp"
#include "camera.hpp"
#include "grid.hpp"
#include "imgui.h"
#include "input.hpp"
#include "movement.hpp"
#include "player_unit.hpp"
#include "enemy_unit.hpp"
#include "rlImGui.h"
#include "turns.hpp"
#include <iostream>
#include <raylib.h>

int main() {
  InitWindow(1200, 800, "Camera Test");
  Camera3D camera = {0};
  Vector3 initialCameraPos = {0.0f, 0.0f, 0.0f};
  initCamera(camera, initialCameraPos);
  SetTargetFPS(60);
  rlImGuiSetup(false);

  Vector3 worldOrigin = {0.0f, 0.0f, 0.0f};
  Vector2 currentTarget = {-1, -1};

  AbilityRegistry::initializeRegistry();
  TacticalGrid::initGrids();
  TacticalGrid::calculateCoverGrid();
  PlayerUnits::initializePlayerUnits();
  EnemyUnits::initializeEnemyUnits();
  TurnSystem::initializeTurn();

  PlayerUnit *selectedUnit =
      PlayerUnits::getPlayerUnit(TacticalGrid::unitGrid[5][5]);
  TacticalGrid::setMovementDisplayFull(&selectedUnit->gridUnit);

  bool showHover = false;
  Color hoverColor;
  bool showRange = false;
  bool toggleRange = false;
  bool showCover = false;

  std::vector<Vector2> pathPreview;
  bool showPreview = false;

  float lastFrame = GetTime();

  while (!WindowShouldClose()) {
    float currentFrame = GetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    Input::updateInput(camera, worldOrigin, GRID_WIDTH, GRID_HEIGHT);
    updateCamera(camera, Input::getCameraInput());

    const auto &mouseInput = Input::getMouseInput();
    int x = (int)mouseInput.gridPosition.x;
    int y = (int)mouseInput.gridPosition.y;

    if (mouseInput.hasValidGridPos && selectedUnit) {
      int moveCost = TacticalGrid::checkMoveDistance(x, y);
      if (moveCost != -1) {
        showHover = true;
        showCover = true;
        if (selectedUnit->gridUnit.isMoving) {
          showHover = false;
          showHover = false;
        }
        if (moveCost == 1) {
          hoverColor = SKYBLUE;
        }
        if (moveCost == 2) {
          hoverColor = GOLD;
        }
        if (TacticalGrid::waypoints.empty()) {
          pathPreview = TacticalGrid::reconstructPath(
              selectedUnit->gridUnit.gridPosition.x,
              selectedUnit->gridUnit.gridPosition.y, x, y);
          showPreview = !pathPreview.empty();
          if (selectedUnit->gridUnit.isMoving) {
            showPreview = false;
          }
        } else {
          PathData waypointPath = TacticalGrid::calculateWaypointPath(
              &selectedUnit->gridUnit, mouseInput.gridPosition);
          pathPreview = waypointPath.path;
        }
        if (IsKeyPressed(KEY_M) && !pathPreview.empty()) {
          // Movement::setPath(selectedUnit, pathPreview);
          if (moveCost == 1) {
            TurnSystem::executeAction(&selectedUnit->gridUnit, ABILITY_STEP,
                                      mouseInput.gridPosition);
            TacticalGrid::waypoints.clear();
          }
          if (moveCost == 2) {
            TurnSystem::executeAction(&selectedUnit->gridUnit, ABILITY_DASH,
                                      mouseInput.gridPosition);
            TacticalGrid::waypoints.clear();
          }
        }

        if (IsKeyPressed(KEY_O)) {
          if (TacticalGrid::waypoints.empty()) {
            // first waypoint
            if (selectedUnit->gridUnit.movePointsRemaining == 2) {
              // unit has both move actions
              float cost =
                  TacticalGrid::getMovementCost(&selectedUnit->gridUnit, x, y);
              TacticalGrid::waypoints.push_back(
                  {cost, mouseInput.gridPosition});

              // Remaining movement = total speed - cost to reach this waypoint
              // check for remaining scoot + dash range
              float remainingMovement = selectedUnit->gridUnit.speed - cost;
              float scootRange;
              float dashRange;
              if (remainingMovement < 0) {
                // used up all scoot range
                scootRange = 0;
                dashRange = selectedUnit->gridUnit.speed * .5 - cost;
              } else {
                scootRange = selectedUnit->gridUnit.speed - cost;
                dashRange = selectedUnit->gridUnit.speed * 1.5 - cost;
              }
              TacticalGrid::setMovementDisplayFull(x, y, scootRange, dashRange);
            } else {
              // unit only has dash left
              float cost =
                  TacticalGrid::getMovementCost(&selectedUnit->gridUnit, x, y);
              TacticalGrid::waypoints.push_back(
                  {cost, mouseInput.gridPosition});
              float remainingMovement =
                  selectedUnit->gridUnit.speed * 0.5 - cost;
              TacticalGrid::setMovementDisplayDash(x, y, remainingMovement);
            }
          } else {
            // there are existing waypoints with costs to account for
            if (selectedUnit->gridUnit.movePointsRemaining == 2) {
              // unit has both move actions left.
              int wayX = TacticalGrid::waypoints.back().parent.x;
              int wayY = TacticalGrid::waypoints.back().parent.y;
              float totalCost = TacticalGrid::waypoints.back().cost;
              float cost = TacticalGrid::getMovementCost(wayX, wayY, x, y);
              totalCost += cost;
              TacticalGrid::waypoints.push_back(
                  {totalCost, mouseInput.gridPosition});

              // Remaining movement = total speed - total cost to reach this
              // waypoint
              float remainingMovement =
                  selectedUnit->gridUnit.speed - totalCost;
              float scootRange;
              float dashRange;
              if (remainingMovement < 0) {
                scootRange = 0;
                dashRange = selectedUnit->gridUnit.speed * 1.5 - totalCost;
              } else {
                scootRange = selectedUnit->gridUnit.speed - totalCost;
                dashRange = selectedUnit->gridUnit.speed * 1.5 - totalCost;
              }
              TacticalGrid::setMovementDisplayFull(x, y, scootRange, dashRange);
            } else {
              // unit only has dash action left.
              int wayX = TacticalGrid::waypoints.back().parent.x;
              int wayY = TacticalGrid::waypoints.back().parent.y;
              float totalCost = TacticalGrid::waypoints.back().cost;
              float cost = TacticalGrid::getMovementCost(wayX, wayY, x, y);
              totalCost += cost;
              TacticalGrid::waypoints.push_back(
                  {totalCost, mouseInput.gridPosition});
              float remainingMovement =
                  selectedUnit->gridUnit.speed * 0.5 - totalCost;
              TacticalGrid::setMovementDisplayDash(x, y, remainingMovement);
            }
          }
        }

      } else {
        showHover = false;
        showCover = false;
      }
    }

    if (mouseInput.leftClicked && mouseInput.hasValidGridPos) {
      printf("Clicked grid tile: (%d, %d)\n", x, y);
      CoverData &cover = TacticalGrid::coverGrid[y][x];
      std::cout << cover << std::endl;
      if (selectedUnit) {
        std::cout << "It will cost "
                  << TacticalGrid::getMovementCost(&selectedUnit->gridUnit, x,
                                                   y)
                  << " to move to ( " << x << " , " << y << ")\n";
      }
      std::cout << TacticalGrid::isGridUnitAt(x, y) << " unit at ( " << x
                << " , " << y << ")\n";
      if (TacticalGrid::unitGrid[y][x] != -1 && TacticalGrid::unitGrid[y][x] < 10) {
        selectedUnit = PlayerUnits::getPlayerUnit(TacticalGrid::unitGrid[y][x]);
        if (selectedUnit->gridUnit.movePointsRemaining <= 0 ||
            selectedUnit->gridUnit.turnComplete ||
            selectedUnit->gridUnit.actionPointsRemaining <= 0) {
          std::cout << "clear movement display\n";
          TacticalGrid::clearMovementGrid();
        } else {
          if (selectedUnit->gridUnit.movePointsRemaining == 1) {
            TacticalGrid::setMovementDisplayDash(&selectedUnit->gridUnit);
          } else {
            TacticalGrid::setMovementDisplayFull(&selectedUnit->gridUnit);
          }
        }
      }
      if (TacticalGrid::unitGrid[y][x] != -1 && TacticalGrid::unitGrid[y][x] >= 10){
        //this should be an enemy
        int gridId = TacticalGrid::unitGrid[y][x];
        std::cout << "That's an enemy aAAAaaAAaAAAaaaaaAAAAAaH\n";
        EnemyUnit* enemy = EnemyUnits::getEnemyUnit(gridId - 10);
        std::cout << "Their name is " << enemy->name << "\n";
      }
    }
    if (IsKeyPressed(KEY_P) && selectedUnit) {
      TacticalGrid::waypoints.clear();
      if (selectedUnit->gridUnit.actionPointsRemaining == 1) {
        TacticalGrid::setMovementDisplayDash(&selectedUnit->gridUnit);
      } else {
        TacticalGrid::setMovementDisplayFull(&selectedUnit->gridUnit);
      }
    }

    if (IsKeyPressed(KEY_TAB) && selectedUnit) {
      int currentId = selectedUnit->id;
      selectedUnit = PlayerUnits::getNextPlayerUnit(currentId);
      if (selectedUnit->gridUnit.movePointsRemaining <= 0 ||
          selectedUnit->gridUnit.turnComplete ||
          selectedUnit->gridUnit.actionPointsRemaining <= 0) {
        std::cout << "clear movement display\n";
        TacticalGrid::clearMovementGrid();
      } else {
        if (selectedUnit->gridUnit.movePointsRemaining == 1) {
          std::cout << "Half movement left\n";
          TacticalGrid::setMovementDisplayDash(&selectedUnit->gridUnit);
        } else {
          std::cout << "Full movement left\n";
          TacticalGrid::setMovementDisplayFull(&selectedUnit->gridUnit);
        }
      }
    }

    if (IsKeyPressed(KEY_G)) {
      toggleRange = !toggleRange;
    }

    if (selectedUnit->gridUnit.isMoving) {
      showRange = false;
    } else {
      showRange = toggleRange;
    }

    if (IsKeyPressed(KEY_T)) {
      std::cout << selectedUnit->gridUnit.gridPosition.x << " , "
                << selectedUnit->gridUnit.gridPosition.y << std::endl;
    }

    if (IsKeyPressed(KEY_L)) {
      std::cout << "Current path preview:\n" << pathPreview << std::endl;
    }

    if (IsKeyPressed(KEY_ENTER)) {
      TurnSystem::endTurn();
    }

    Movement::updateMove(deltaTime);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    rlImGuiBegin();
    ImGui::SetNextWindowPos(
            ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() - 150.0f),
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 80));
    ImGui::Begin("Target Bar", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
    for (auto& enemy : EnemyUnits::enemyUnits){
      if (ImGui::Button(enemy.name.c_str())){
        currentTarget = enemy.gridUnit.gridPosition;
      }
      ImGui::SameLine();
    }
    ImGui::End();
    ImGui::SetNextWindowPos(
        ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() - 100.0f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 80));
    ImGui::Begin("Ability Bar", NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    if (ImGui::Button("Shoot")) {
      std::cout << "Shoot!\n";
      TurnSystem::executeAction(&selectedUnit->gridUnit, ABILITY_SHOOT_PRIMARY, currentTarget);
      }
    ImGui::SameLine();
    if (ImGui::Button("Overwatch")) {
      std::cout << "Overwatch!\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload")) {
      std::cout << "Reload!\n";
    }

    ImGui::PopStyleVar(2);
    
    ImGui::End();
    rlImGuiEnd();

    TurnSystem::displayTurnInfo();
    DrawText(TextFormat("Unit Id: %d, move points: %d, action points: %d, turn "
                        "Complete: %b",
                        selectedUnit->id,
                        selectedUnit->gridUnit.movePointsRemaining,
                        selectedUnit->gridUnit.actionPointsRemaining,
                        selectedUnit->gridUnit.turnComplete),
             300, 725, 25, BLACK);
    BeginMode3D(camera);
    TacticalGrid::drawTerrain(worldOrigin);
    PlayerUnits::setSelectedHighlight(selectedUnit->id);
    PlayerUnits::drawGridUnits(worldOrigin);
    EnemyUnits::drawEnemyUnits(worldOrigin);
    Movement::drawMovingUnits(worldOrigin);
    if (showRange)
      TacticalGrid::drawMovementOverlay(worldOrigin);
    if (showHover)
      TacticalGrid::drawHoverHighlight(x, y, worldOrigin, hoverColor);
    if (showPreview)
      TacticalGrid::drawPathPreview(pathPreview, hoverColor);
    for (auto &point : TacticalGrid::waypoints) {
      Vector3 pos = TacticalGrid::gridToWorldPosition(point.parent, 0.1f);
      DrawCube(pos, .25f, .01f, .25f, MAGENTA);
    }
    if (showCover)
      CoverSystem::renderCover(mouseInput.gridPosition, hoverColor);
    pathPreview.clear();
    EndMode3D();
    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();
  return 0;
}
