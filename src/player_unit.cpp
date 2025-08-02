#include "player_unit.hpp"

namespace PlayerUnits{
  std::vector<PlayerUnit> playerUnits;

  PlayerUnit* getPlayerUnit(int id){
    for (auto & playerUnit : playerUnits){
      if (playerUnit.id == id){
        return &playerUnit;
      }
    }
    return nullptr;
  }    

  PlayerUnit* getNextPlayerUnit(int currentId){
    int temp = currentId + 1;
    if (temp > (int)playerUnits.size() - 1){
      temp = 0;
    }
    return getPlayerUnit(temp);
  }

  void initializePlayerUnits(){
    //gotta make gridUnits first
    playerUnits.clear();
    GridUnit gunit1 = {0, {5,5}, false, 6.0f, RED, false, 2, 1, false};
    PlayerUnit unit1 = {0, gunit1, "Reggie", 30, 6.0f, 19, 4};
    unit1.updateGridUnit();
    playerUnits.push_back(unit1);
    TacticalGrid::addUnitToGrid(unit1.gridUnit);
    GridUnit gunit2= {0, {4,6}, false, 6.0f, GREEN, false, 2, 1, false};
    PlayerUnit unit2 = {1, gunit2, "Charles", 31, 6.0f, 16, 5};
    unit2.updateGridUnit();
    playerUnits.push_back(unit2);
    TacticalGrid::addUnitToGrid(unit2.gridUnit);
    GridUnit gunit3 = {0, {5,7}, false, 6.0f, BLUE, false, 2, 1, false};
    PlayerUnit unit3 = {2, gunit3, "Sharon", 28, 7.0f, 15, 3};
    unit3.updateGridUnit();
    playerUnits.push_back(unit3);
    TacticalGrid::addUnitToGrid(unit3.gridUnit);

      
    
  }
  void drawGridUnits(Vector3 worldOrigin) {
    for (auto &unit : playerUnits) {
      if (!unit.gridUnit.isMoving) {
        Vector3 unitPos = {worldOrigin.x + unit.gridUnit.gridPosition.x * TILE_SIZE,
                           worldOrigin.y + 1.0f,
                           worldOrigin.z + unit.gridUnit.gridPosition.y * TILE_SIZE};
        DrawCube(unitPos, 1.5f, 1.0f, 1.5f, unit.gridUnit.color);
      }
    }
  }
  void setSelectedHighlight(int unitId) {
    for (auto &unit : playerUnits) {
      if (unitId == unit.id) {
        Vector3 pos = {unit.gridUnit.gridPosition.x * TILE_SIZE, 0.0f,
                       unit.gridUnit.gridPosition.y * TILE_SIZE};
        DrawCubeWires(pos, TILE_SIZE, 0.1f, TILE_SIZE, SKYBLUE);
        break;
      }
    }
  }
  } // namespace PlayerUnits
