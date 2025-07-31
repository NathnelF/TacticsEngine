
#include "grid.hpp"
#include <raymath.h>

std::ostream &operator<<(std::ostream &os, const CoverType &c) {
  switch (c) {
  case COVER_NONE:
    os << "No Cover\n";
    return os;
  case COVER_HALF:
    os << "Half Cover\n";
    return os;
  case COVER_FULL:
    os << "Full cover\n";
    return os;
  }
  os << "Unkown cover\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, const CoverData &cd){
  os << "North: " << cd.north << "\n";
  os << "South: " << cd.south << "\n";
  os << "East: " << cd.east << "\n";
  os << "West: " << cd.west << "\n";
  return os;
}

namespace CoverSystem {

CoverType getCoverValue(Vector2 target, Direction direction) {
  CoverData tileCover = TacticalGrid::coverGrid[(int)target.y][(int)target.x];
  switch (direction) {
  case NORTH:
    return tileCover.north;
  case SOUTH:
    return tileCover.south;
  case EAST:
    return tileCover.east;
  case WEST:
    return tileCover.west;
  }
}

bool hasCover(Vector2 target, Direction direction) {
  CoverType coverVal = getCoverValue(target, direction);
  switch (coverVal) {
  case COVER_FULL:
    return true;
  case COVER_HALF:
    return true;
  case COVER_NONE:
    return false;
  }
}

// Get direction offset vector
Vector2 getDirectionOffset(Direction dir) {
  switch (dir) {
  case NORTH:
    return {0.0f, -1.0f};
  case SOUTH:
    return {0.0f, 1.0f};
  case EAST:
    return {1.0f, 0.0f};
  case WEST:
    return {-1.0f, 0.0f};
  default:
    return {0.0f, 0.0f};
  }
}

// Get rotation angle for direction (in radians)
float getDirectionRotation(Direction dir) {
  switch (dir) {
  case NORTH:
    return 0.0f; // 0°
  case EAST:
    return PI / 2.0f; // 90°
  case SOUTH:
    return PI; // 180°
  case WEST:
    return 3.0f * PI / 2.0f; // 270°
  default:
    return 0.0f;
  }
}

// Render half cover (half circle)
void renderHalfCover(Vector3 position, float rotation, Color color) {
    float radius = 1.00;
    DrawCircle3D(position, radius, {1, 0, 0}, 90, color);
}

// Render full cover (full circle)
void renderFullCover(Vector3 position, float rotation, Color color) {
    float radius = 1.00;
    DrawCircle3D(position, radius, {1, 0, 0}, 90, color);
    float second_radius = radius * .75;
    DrawCircle3D(position, second_radius, {1, 0, 0}, 90, color);
    float third_radius = radius * .5;
    DrawCircle3D(position, third_radius, {1, 0, 0}, 90, color);
}

// Render cover indicator for a specific direction
void renderCoverDirection(Vector3 tileCenter, Direction direction,
                          CoverType coverType, Color color) {
  if (coverType == COVER_NONE)
    return;

  // Calculate position offset based on direction
  Vector2 offset = getDirectionOffset(direction);
  float offsetDistance = TILE_SIZE * 0.35f; // Position near edge of tile

  Vector3 coverPos = {tileCenter.x + offset.x * offsetDistance, tileCenter.y,
                      tileCenter.z + offset.y * offsetDistance};

  // Calculate rotation for the cover symbol
  float rotation = getDirectionRotation(direction);

  // Render based on cover type
  if (coverType == COVER_HALF) {
    renderHalfCover(coverPos, rotation, color);
  } else if (coverType == COVER_FULL) {
    renderFullCover(coverPos, rotation, color);
  }
}

// Render cover for a single tile
void renderCover(Vector2 target, Color color) {
  // Bounds check
  if (target.x < 0 || target.x >= GRID_WIDTH || target.y < 0 ||
      target.y >= GRID_HEIGHT) {
    return;
  }
  // Calculate world position for the tile center
  Vector3 tileCenter = TacticalGrid::gridToWorldPosition(target, 0.2f);

  // Render cover indicators for each direction
  renderCoverDirection(tileCenter, NORTH, getCoverValue(target, NORTH),
                       color);
  renderCoverDirection(tileCenter, EAST, getCoverValue(target, EAST), color);
  renderCoverDirection(tileCenter, SOUTH, getCoverValue(target, SOUTH),
                       color);
  renderCoverDirection(tileCenter, WEST, getCoverValue(target, WEST), color);
}

} // namespace CoverSystem
