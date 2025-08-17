
#pragma once
#include <random>

//contains all the code for random number generation
namespace Random {
  extern std::random_device rd;
  extern std::mt19937 gen;
  int randomNumber(int min, int max);
  int rollToHit();
}
