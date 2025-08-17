
#pragma once
#include "grid.hpp"

// this file will contain all the code that deals with elevation.
//
struct ElevatedArea {
  int width;
  int height;
  Vector2 origin; //bottom left coordinate is origin. Area extends from origin width x height
  std::vector<Vector2> accessPoints; 
};

struct ElevatedGrid {
  
};

namespace Elevation {
  
}
