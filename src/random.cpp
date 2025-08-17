
#include "random.hpp"
#include <random>

namespace Random {
  std::random_device rd;
  std::mt19937 gen(rd());

  int randomNumber(int min, int max){
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
  }

  int rollToHit(){
    std::uniform_int_distribution<int> dist(1, 100);
    return dist(gen);
  }
  
}
