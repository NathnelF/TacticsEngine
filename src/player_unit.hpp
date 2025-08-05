
#pragma once
#include "grid.hpp"

struct PlayerUnit {
  int id;
  GridUnit gridUnit;
  std::string name;
  int aimStat;
  float speedStat;
  int resolveStat;
  int maxHealthStat;
  int currentHealth;

  //TODO abilities
  //TODO perks
  
  void updateGridUnit(){
    gridUnit.speed = calculateSpeed();
    gridUnit.playerControlled = true;
    gridUnit.id = id;
  }

  private: float calculateSpeed(){
      //TODO equipment modifiers.
      return speedStat;
    }
};

namespace PlayerUnits{
    extern std::vector<PlayerUnit> playerUnits;
    PlayerUnit* getPlayerUnit(int id);
    PlayerUnit* getNextPlayerUnit(int currentId);
    void initializePlayerUnits(); //TODO: get units from external source.
    void drawGridUnits(Vector3 worldOrigin);      
    void setSelectedHighlight(int unitId);
  }
