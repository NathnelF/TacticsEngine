
#pragma once
#include <vector>
#include <raylib.h>
#include "grid.hpp"

//this file is responsible for random map generation
// 
// Current thoughts:
// We want the map to be truly random in terms of high / low cover potential.
// Split the map into a definite number of chunks based on it's total cell count
// Randomize layout:
  // Building chunk
  // Road chunk
  // Perimeter wall chunk
  // Hill
  // Valley
  // Other structured terrain
// The rest of the map will be split into cover chunks
// Each chunk will have a total cover value (of high, medium, or low)
  // High cover value : 2 x 1 high - low cover ratio
  // Medium cover value : 1 x 1 high - low cover ratio
  // Low cover value : 1 x 2 high - low cover ratio

enum MissionType {
  ELIMINATION = 0,
  AMBUSH = 1,
  EXIFLTRATION = 2,
  INFILTRATION = 3,
  SABOTAGE = 4,
};

enum MapSetting {
  WILDERNESS = 0,
  TOWN = 1,
  CITY = 2,
  BASE = 3,
  
};

enum ChunkType {
  EMPTY = 0,
  MISC = 1,
  BUILDING = 2,
  ROAD = 3,
  PERIMITER = 4,
  HILL = 5,
  RIVER = 6
  //etc
};

// enum CoverChunk{
//   LOW = 0,
//   MEDIUM = 1,
//   HIGH = 2
// };

namespace MapGeneration {
  MapSetting randomizeSetting();
  std::vector<ChunkType> getValidChunkTypes(MissionType mission);
  extern std::vector<ChunkType> mapChunks;
  extern ChunkType chunkGrid[GRID_HEIGHT][GRID_WIDTH];
  ChunkType randomizeChunkType(int random);
  int randomizeStaticChunkCount(int random);
  int randomizeDynaimcChunkCount(int random);
  bool hasPerimeter();
  bool hasRiver();
  bool hasRoad();
  bool isValidPosition(int x, int y);
  bool checkForOverlap(int x, int y);
  // CoverChunk randomizeCoverChunkValue(int random);
  Vector2 getCellDimensions(ChunkType chunk);
}
