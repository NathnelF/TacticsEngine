
#pragma once
#include <raylib.h>


struct GridLocation {
 int x, y, z;

 bool operator==(const GridLocation& other) const {
 	return x == other.x && y == other.y && z == other.z;
 }	
};
