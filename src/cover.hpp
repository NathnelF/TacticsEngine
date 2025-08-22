
#pragma once
#include <raylib.h>
#include <iostream>
#include "types.hpp"

enum CoverType{
  COVER_NONE = 0,
  COVER_HALF = 1,
  COVER_FULL = 2
};

enum Direction{
  NORTH = 0,
  SOUTH = 1,
  EAST = 2,
  WEST = 3
};

struct CoverData{
  CoverType north;
  CoverType south;
  CoverType east;
  CoverType west;
};

std::ostream& operator<<(std::ostream& os, const CoverType& c);
std::ostream& operator<<(std::ostream& os, const CoverData& cd);


namespace CoverSystem{
  CoverType getCoverValue(GridLocation target, Direction direction);
  bool hasCover(GridLocation target, Direction direction);
  int getCoverBonus(CoverType cover);
  int getTargetCoverBonus(GridLocation from, GridLocation target);
  void renderCover(GridLocation target, Color color);
  void renderCoverDirection(Vector3 tileCenter, Direction direction, CoverType coverType, Color color);

  
}

