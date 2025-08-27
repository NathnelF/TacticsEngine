
#include "grid.hpp"
#include "elevation.hpp"
#include "types.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <raymath.h>

std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
  os << "(" << v.x << ", " << v.y << ")";
  return os;
}
std::ostream &operator<<(std::ostream &os, const GridLocation&v) {
  os << "(" <<  v.z << ", " << v.x << ", " << v.y << ")";
  return os;
}
std::ostream &operator<<(std::ostream &os, const std::vector<GridLocation> &v) {
  for (auto &coord : v) {
    os << "(" << coord.z << ", " << coord.x << ", " << coord.y << ")\n";
  }
  return os;
}

namespace TacticalGrid {
TileType terrainGrid[3][GRID_HEIGHT][GRID_WIDTH];
int unitGrid[3][GRID_HEIGHT][GRID_WIDTH];
int movementGrid[3][GRID_HEIGHT][GRID_WIDTH];
MoveCell pathGrid[3][GRID_HEIGHT][GRID_WIDTH];
CoverData coverGrid[3][GRID_HEIGHT][GRID_WIDTH];
std::vector<MoveCell> waypoints;

void initGrids() {
  for (int z = 0; z < 3; z++){
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      terrainGrid[z][y][x] = TILE_EMPTY;
      unitGrid[z][y][x] = -1;
      movementGrid[z][y][x] = -1;
      pathGrid[z][y][x] = {-1.0f, {-1, -1}};
      coverGrid[z][y][x] = {
          COVER_NONE,
          COVER_NONE,
          COVER_NONE,
          COVER_NONE,
      };
    }
  }
  }

  // Add some more interesting terrain
  terrainGrid[0][1][2] = TILE_WALL;
  terrainGrid[0][1][3] = TILE_WALL;
  terrainGrid[0][1][4] = TILE_WALL;
  terrainGrid[0][1][5] = TILE_WALL;
  terrainGrid[0][1][6] = TILE_WALL;
  terrainGrid[0][10][10] = TILE_TREE;
  terrainGrid[0][9][10] = TILE_TREE;
  terrainGrid[0][8][10] = TILE_TREE;
  terrainGrid[0][6][15] = TILE_WALL;
  terrainGrid[0][7][16] = TILE_WALL;
  terrainGrid[0][11][11] = TILE_TREE;
  terrainGrid[0][10][11] = TILE_TREE;
  terrainGrid[0][11][10] = TILE_TREE;
  terrainGrid[0][20][5] = TILE_ROCK;
  terrainGrid[0][14][26] = TILE_ROCK;
  terrainGrid[0][16][4] = TILE_ROCK;
  terrainGrid[0][4][16] = TILE_ROCK;
  terrainGrid[0][4][2] = TILE_BOX;
  terrainGrid[0][4][3] = TILE_BOX;
  terrainGrid[0][3][2] = TILE_BOX;
  terrainGrid[0][17][21] = TILE_ROCK;
  terrainGrid[0][9][6] = TILE_BOX;
}

void clearTerrainGrid() {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        terrainGrid[z][y][x] = TILE_EMPTY;
      }
    }
  }
}

void clearMovementGrid() {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        movementGrid[z][y][x] = -1;
      }
    }
  }
}

void clearUnitGrid() {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        unitGrid[z][y][x] = -1;
      }
    }
  }
}

void addUnitToGrid(GridUnit unit) {
  unitGrid[unit.gridPosition.z][unit.gridPosition.y][unit.gridPosition.x] = unit.id;
}

void clearPathGrid() {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        pathGrid[z][y][x] = {-1.0f, {-1, -1}};;
      }
    }
  }
}

void clearCoverGrid() {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
       coverGrid[z][y][x] = {
          COVER_NONE,
          COVER_NONE,
          COVER_NONE,
          COVER_NONE,
      };

      }
    }
  }
}

CoverType calculateCoverFromTerrain(TileType terrain) {
  switch (terrain) {
  case TILE_WALL:
    return COVER_FULL;
  case TILE_TREE:
    return COVER_FULL;
  case TILE_EMPTY:
    return COVER_NONE;
  case TILE_ROCK:
    return COVER_HALF;
  case TILE_BOX:
    return COVER_HALF;
  }
}
void calculateCoverGrid() {
  for (int z = 0; z < 3; z++){
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      // check each direction (only if in bounds)
      // north
      if (y > 0) {
        TileType northTerrain = terrainGrid[z][y - 1][x];
        CoverType coverVal = calculateCoverFromTerrain(northTerrain);
        coverGrid[z][y][x].north = coverVal;
      }
      // south
      if (y < GRID_HEIGHT - 1) {
        TileType southTerrain = terrainGrid[z][y + 1][x];
        CoverType coverVal = calculateCoverFromTerrain(southTerrain);
        coverGrid[z][y][x].south = coverVal;
      }
      // east
      if (x < GRID_WIDTH - 1) {
        TileType eastTerrain = terrainGrid[z][y][x + 1];
        CoverType coverVal = calculateCoverFromTerrain(eastTerrain);
        coverGrid[z][y][x].east = coverVal;
      }
      // west
      if (x > 0) {
        TileType westTerrain = terrainGrid[z][y][x - 1];
        CoverType coverVal = calculateCoverFromTerrain(westTerrain);
        coverGrid[z][y][x].west = coverVal;
      }
    }
  }
}
}

Vector3 gridToWorldPosition(GridLocation position, float yLevel){
  float yScale = 5.0f;
  Vector3 worldPos = {(float)position.x * TILE_SIZE, (position.z * yScale) + yLevel, (float)position.y * TILE_SIZE};
  return worldPos;
}

bool isPassable(int x, int y, int layer) {
  if (layer > 2 || layer < 0){
    return false;
  }
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
    return false;
  }
  if (terrainGrid[layer][y][x] == TILE_EMPTY) {
    return true;
  } else {
    return false;
  }
}

bool isGridUnitAt(int x, int y, int layer) { return unitGrid[layer][y][x] != -1; }

float getTerrainMultiplier(int x, int y, int layer) {
  if (layer < 0 || layer > 2){
    return -1.0f;
  }
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
    return -1.0f;
  TileType terrainType = terrainGrid[layer][y][x];
  if (terrainType == TILE_EMPTY) {
    return 1.0f;
  } else {
    return -1.0f;
  }
}

float getGridUnitMultiplier(int x, int y, int layer) {
  if (layer < 0 || layer > 2){
    return -1.0f;
  }
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
    return -1.0f;
  if (isGridUnitAt(x, y, layer)) {
    return -1.0f;
  } else {
    return 1.0f;
  }
}

void calculateCostsFrom(int startX, int startY, int startZ, float maxRange) {
  auto startTime = std::chrono::high_resolution_clock::now();
  clearPathGrid();

  std::priority_queue<std::tuple<float, int, int, int>, std::vector<std::tuple<float, int, int, int>>, std::greater<>> pq;
  
  pathGrid[startZ][startY][startX].cost = 0.0f;
  pathGrid[startZ][startY][startX].parent = {startX, startY, startZ};
  pq.push({0.0f, startX, startY, startZ});

  int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  float costs[] = {1.414f, 1.0f, 1.414f, 1.0f, 1.0f, 1.414f, 1.0f, 1.414f};

  while (!pq.empty()) {
    auto [currentCost, currentX, currentY, currentZ] = pq.top();
     pq.pop();

    if (currentCost > pathGrid[currentZ][currentY][currentX].cost)
      continue;

    for (int i = 0; i < 8; i++) {
      int neighborX = currentX + dx[i];
      int neighborY = currentY + dy[i];

      if (neighborX < 0 || neighborX >= GRID_WIDTH || neighborY < 0 ||
          neighborY >= GRID_HEIGHT)
        continue;

      float terrainMultiplier = getTerrainMultiplier(neighborX, neighborY, currentZ);
      if (terrainMultiplier < 0)
        continue;

      float unitMultiplier = getGridUnitMultiplier(neighborX, neighborY, currentZ);
      if (unitMultiplier < 0)
        continue;

      float moveCost = costs[i] * terrainMultiplier;
      float newCost = currentCost + moveCost;

      if (newCost > maxRange)
        continue;

      if (pathGrid[currentZ][neighborY][neighborX].cost < 0 ||
          newCost < pathGrid[currentZ][neighborY][neighborX].cost) {

        pathGrid[currentZ][neighborY][neighborX].cost = newCost;
        pathGrid[currentZ][neighborY][neighborX].parent = {currentX, currentY, currentZ};
        pq.push({newCost, neighborX, neighborY, currentZ});
      }
    }

    //check for layer connections
    std::vector<LayerConnection> connections = Elevation::getAllConnectionsAt(currentX, currentY, currentZ);
    for (const auto& conn : connections){
      float newCost = currentCost + conn.movementCost;
      if (newCost > maxRange) continue;

      int newZ = conn.toLayer;
      if (pathGrid[newZ][currentX][currentY].cost < 0 || newCost < pathGrid[newZ][currentX][currentY].cost){
        pathGrid[newZ][currentX][currentY].cost = newCost;
        pathGrid[newZ][currentX][currentY].parent = {currentX, currentY, currentZ};
        pq.push({newCost, currentX, currentY, newZ});
      }
    }
  }
  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
  // std::cout << "Path calculation took " << duration.count() << " microseconds!\n";
}

std::vector<GridLocation> reconstructPath(int fromX, int fromY,  int fromZ, int toX, int toY, int toZ) {
  std::vector<GridLocation> path;

  // std::cout << "DEBUGGING reconstructPath method...\n";
  // std::cout << "target destination is: ( " << toZ << ", " << toX << ", " << toY << " )\n";
  if (pathGrid[toZ][toY][toX].cost < 0) {
    return path;
    // std::cout << "returning because the destination is unreachable (cost of < 0)\n";
  }

  GridLocation current= {toX, toY, toZ};
  // std::cout << "current gridLocation: " << current << std::endl;
  // Build path backwards from destination to start
  while (true) {
    path.push_back(current);
    GridLocation parent = pathGrid[current.z][current.y][current.x].parent;

    // std::cout << "parent gridLocation : " << parent << std::endl;

    // Check if we reached the start (parent points to itself)
    if (parent.x == fromX && parent.y == fromY && parent.z == fromZ) {
      // std::cout << "reached start!\n";
      break;
    }

    current = parent;
  }
  path.push_back({fromX, fromY, fromZ});

  // std::cout << "reversing path!\n";
  // Reverse the path so it goes from start to destination
  std::reverse(path.begin(), path.end());

  // std::cout << "path reversed!\n";
  // std::cout << "final path : " << path << std::endl;

  return path;
}

float getMovementCost(int fromX, int fromY, int fromZ, int toX, int toY, int toZ) {
  calculateCostsFrom(fromX, fromY, fromZ);
  return pathGrid[toZ][toY][toX].cost;
}
float getMovementCost(const GridUnit *unit, int toX, int toY, int toZ) {
  calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit->gridPosition.z);
  return pathGrid[toZ][toY][toX].cost;
}
bool isReachable(int fromX, int fromY, int fromZ, int toX, int toY, int toZ, float maxMovement) {
  float cost = getMovementCost(fromX, fromY, fromZ, toX, toY, toZ);
  return cost >= 0 && cost <= maxMovement;
}

PathData getPathInfo(int fromX, int fromY, int fromZ, int toX, int toY, int toZ, float maxMovement) {
  calculateCostsFrom(fromX, fromY, fromZ, maxMovement);

  PathData result;
  result.totalCost = pathGrid[toZ][toY][toX].cost;
  result.isReachable = isReachable(fromX, fromY, fromZ, toX, toY, toZ, maxMovement);

  if (result.isReachable) {
    result.path = reconstructPath(fromX, fromY, fromZ, toX, toY, toZ);
  }

  return result;
}

int checkMoveDistance(int x, int y, int layer) { return movementGrid[layer][y][x]; }

void setMovementDisplayFull(GridUnit *unit) {
  clearMovementGrid();
  calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit-> gridPosition.z, 
                     unit->speed * 1.5);

  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        float cost = pathGrid[z][y][x].cost;
        if (cost > 0 && cost <= unit->speed) {
          movementGrid[z][y][x] = 1;
        }
        if (cost > unit->speed && cost <= unit->speed * 1.5) {
          movementGrid[z][y][x] = 2;
        }
      }
    }
  }
}

void setMovementDisplayFull(int fromX, int fromY, int fromZ, float remainingScootRange,
                            float remainingDashRange) {
  clearMovementGrid();

  calculateCostsFrom(fromX, fromY, fromZ, remainingDashRange);
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        float cost = pathGrid[z][y][x].cost;
        if (cost > 0 && cost <= remainingScootRange) {
          movementGrid[z][y][x] = 1;
        }
        if (cost > remainingScootRange && cost <= remainingDashRange) {
          movementGrid[z][y][x] = 2;
        }
      }
    }
  }
}

void setMovementDisplayDash(GridUnit *unit) {
  // used to set display after step movement.
  clearMovementGrid();
  calculateCostsFrom(unit->gridPosition.x, unit->gridPosition.y, unit->gridPosition.z,
                     unit->speed * 0.5);
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        float cost = pathGrid[z][y][x].cost;
        if (cost > 0 && cost <= unit->speed * 0.5) {
          movementGrid[z][y][x] = 2;
        }
      }
    }
  }
}
void setMovementDisplayDash(int fromX, int fromY, int fromZ,
                            float remainingDashRange) {
  // used to set display after step movement.
  clearMovementGrid();
  calculateCostsFrom(fromX, fromY, fromZ, remainingDashRange);
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        float cost = pathGrid[z][y][x].cost;
        if (cost > 0 && cost <= remainingDashRange) {
          movementGrid[z][y][x] = 2;
        }
      }
    }
  }
}

void drawHoverHighlight(int x, int y, Vector3 worldOrigin, Color hoverColor) {
  Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y,
                 worldOrigin.z + y * TILE_SIZE};
  DrawCubeWires(pos, TILE_SIZE, 0.15f, TILE_SIZE, hoverColor);
}

void drawTerrain(Vector3 worldOrigin) {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        Vector3 terrainPos = {worldOrigin.x + x * TILE_SIZE,
                              (worldOrigin.y * 5 * z) + 1.0f,
                              worldOrigin.z + y * TILE_SIZE};
        Vector3 wirePos = {worldOrigin.x + x * TILE_SIZE, (worldOrigin.y * 5 * z),
                           worldOrigin.z + y * TILE_SIZE};

        if (terrainGrid[z][y][x] == TILE_EMPTY) {
          DrawCubeWires(wirePos, TILE_SIZE, 0.1f, TILE_SIZE, BLACK);
          continue;
        }

        Color color;
        switch (terrainGrid[z][y][x]) {
        case TILE_WALL:
          color = DARKBROWN;
          break;
        case TILE_BOX:
          color = ORANGE;
          break;
        case TILE_TREE:
          color = DARKGREEN;
          break;
        case TILE_ROCK:
          color = DARKGRAY;
          break;
        }
        float height;
        switch (terrainGrid[z][y][x]) {
        case TILE_WALL:
          height = 4.0f;
          break;
        case TILE_BOX:
          height = 2.0f;
          break;
        case TILE_TREE:
          height = 4.0f;
          break;
        case TILE_ROCK:
          height = 2.0f;
          break;
        }
        DrawCube(terrainPos, 2.0f, height, 2.0f, color);

        // DrawCubeWires(wirePos, TILE_SIZE, 0.1f, TILE_SIZE, BLACK);
      }
    }
  }
}

PathData calculateWaypointPath(const GridUnit *unit, GridLocation finalDestination) {
  PathData result;
  result.totalCost = 0.0f;
  result.isReachable = true;

  GridLocation currentPos = unit->gridPosition;
  float remainingMovement = unit->speed * 1.5;

  // Path to first waypoint
  if (!waypoints.empty()) {
    PathData segmentResult = getPathInfo(
        currentPos.x, currentPos.y, currentPos.z, waypoints[0].parent.x, waypoints[0].parent.y, waypoints[0].parent.z, remainingMovement);
    if (!segmentResult.isReachable) {
      result.isReachable = false;
      return result;
    }

    result.path = segmentResult.path;
    result.totalCost += segmentResult.totalCost;
    remainingMovement -= segmentResult.totalCost;
    currentPos = waypoints[0].parent;
  }

  // Path through waypoints
  for (size_t i = 1; i < waypoints.size(); i++) {
    PathData segmentResult = getPathInfo(
        currentPos.x, currentPos.y, currentPos.z, waypoints[i].parent.x, waypoints[i].parent.y, waypoints[i].parent.z, remainingMovement);
    if (!segmentResult.isReachable) {
      result.isReachable = false;
      return result;
    }

    // Remove first element to avoid duplicates
    if (!segmentResult.path.empty()) {
      segmentResult.path.erase(segmentResult.path.begin());
      result.path.insert(result.path.end(), segmentResult.path.begin(),
                         segmentResult.path.end());
    }

    result.totalCost += segmentResult.totalCost;
    remainingMovement -= segmentResult.totalCost;
    currentPos = waypoints[i].parent;
  }

  // Path to final destination
  PathData finalSegment =
      getPathInfo(currentPos.x, currentPos.y, currentPos.z, finalDestination.x, finalDestination.y, finalDestination.z, remainingMovement);
  if (!finalSegment.isReachable) {
    result.isReachable = false;
    return result;
  }

  if (!finalSegment.path.empty()) {
    finalSegment.path.erase(finalSegment.path.begin());
    result.path.insert(result.path.end(), finalSegment.path.begin(),
                       finalSegment.path.end());
  }

  result.totalCost += finalSegment.totalCost;

  return result;
}

void drawMovementOverlay(Vector3 worldOrigin) {
  for (int z = 0; z < 3; z++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        int cost = movementGrid[z][y][x];
        if (cost == 1) {
          Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y * 5 * z,
                         worldOrigin.z + y * TILE_SIZE};
          DrawCube(pos, 0.5f, 0.05f, 0.5f, SKYBLUE);
        }
        if (cost == 2) {
          Vector3 pos = {worldOrigin.x + x * TILE_SIZE, worldOrigin.y * 5 * z,
                         worldOrigin.z + y * TILE_SIZE};
          DrawCube(pos, 0.5f, 0.05f, 0.5f, GOLD);
        }
      }
    }
  }
}

void drawPathPreview(std::vector<GridLocation> path, Color color) {
  if (path.size() < 3) {
    return;
  }

  auto from = path.begin();
  auto to = from;
  to++;

  while (to != path.end()) {
    Vector3 start = gridToWorldPosition(*from, 0.0f);
    Vector3 end = gridToWorldPosition(*to, 0.0f);

    auto temp = to;
    temp++;
    if (temp == path.end()) {
      Vector3 direction = Vector3Normalize(Vector3Subtract(end, start));
      end = Vector3Subtract(end, Vector3Scale(direction, TILE_SIZE / 2.0f));
    }
    DrawLine3D(start, end, color);
    from++;
    to++;
  }
}

} // namespace TacticalGrid
