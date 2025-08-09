
#include "los.hpp"
#include <raymath.h>

namespace LineOfSight {
std::vector<Vector2> getPeekPositions(Vector2 center) {
  std::vector<Vector2> positions = {center};

  Vector2 adjacentPositions[] = {{0, 1}, {0, -1}, {1, 0},  {-1, 0},
                                 {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (auto pos : adjacentPositions) {
    Vector2 peekPos = Vector2Add(center, pos);
    if (!TacticalGrid::isGridUnitAt(peekPos.x, peekPos.y) &&
        TacticalGrid::isPassable(peekPos.x, peekPos.y)) {
      positions.push_back(peekPos);
    }
  }
  return positions;
}

LOSResult findSingleLine(Vector2 from, Vector2 to) {
  std::vector<Vector2> points;
  bool hasSight = true;

  int x0 = (int)from.x;
  int x1 = (int)to.x;
  int y0 = (int)from.y;
  int y1 = (int)to.y;

  int xDistance = abs(x1 - x0);
  int yDistance = abs(y1 - y0);

  int currentX = x0;
  int currentY = y0;

  int xIncrement = (x1 > x0) ? 1 : -1;
  int yIncrement = (y1 > y0) ? 1 : -1;

  int error = xDistance - yDistance;

  xDistance *= 2;
  yDistance *= 2;

  while (true) {
    points.push_back({(float)currentX, (float)currentY});

    if (currentX < 0 || currentX >= GRID_WIDTH || currentY < 0 ||
        currentY >= GRID_HEIGHT) {
      hasSight = false;
      break;
    }

    if (TacticalGrid::terrainGrid[currentY][currentX] == TILE_WALL ||
        TacticalGrid::terrainGrid[currentY][currentX] == TILE_TREE) {
      hasSight = false;
    }

    if (currentX == x1 && currentY == y1) {
      break;
    }

    int error2 = error * 2;

    if (error2 > -yDistance) {
      currentX += xIncrement;
      error -= yDistance;
    }

    if (error2 < xDistance) {
      currentY += yIncrement;
      error += xDistance;
    }
  }
  return {points, hasSight};
}

bool calculateLOS(Vector2 from, Vector2 to) {
  std::vector<Vector2> peekPositionsFrom = getPeekPositions(from);
  std::vector<Vector2> peekPositionsTo = getPeekPositions(to);

  for (auto peekFrom : peekPositionsFrom) {
    for (auto peekTo : peekPositionsTo) {
      LOSResult los = findSingleLine(peekFrom, peekTo);
      if (los.hasSight == true) {
        return true;
      }
    }
  }
  return false;
}

void debugLOS(Vector2 from, Vector2 to) {
  std::vector<Vector2> peekPositionsFrom = getPeekPositions(from);
  std::vector<Vector2> peekPositionsTo = getPeekPositions(to);

  for (auto peekFrom : peekPositionsFrom) {
    for (auto peekTo : peekPositionsTo) {
      LOSResult los = findSingleLine(peekFrom, peekTo);
      if (los.hasSight == true) {
        TacticalGrid::drawPathPreview(los.line, GREEN);
      } else {
        TacticalGrid::drawPathPreview(los.line, RED);
      }
    }
  }
}
} // namespace LineOfSight
