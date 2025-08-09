#include "grid.hpp"

struct LOSResult {
  std::vector<Vector2> line;
  bool hasSight;
};


namespace LineOfSight{
  //LOS is calculated by getting all possible peek postions out of cover (all adjacent, moveable tiles)
  // Then checking the line between that position and each peek position of the target for full cover blocking the line.
  // We use getPeekPosition to calculate each from and to position
  // We use findSingleLine to check the line between each pair of peek positions
  // And we use calculateLOS to determine if there is LOS between two units.
  std::vector<Vector2> getPeekPositions(Vector2 center);
  LOSResult findSingleLine(Vector2 from, Vector2 to);
  bool calculateLOS(Vector2 from, Vector2 to);
  void debugLOS(Vector2 from, Vector2 to);
  
}
