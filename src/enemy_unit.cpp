
#include "enemy_unit.hpp"

namespace EnemyUnits {
std::vector<EnemyUnit> enemyUnits;

EnemyUnit *getEnemyUnit(int id) {
  for (auto &enemyUnit : enemyUnits) {
    if (enemyUnit.id == id) {
      return &enemyUnit;
    }
  }
  return nullptr;
}

EnemyUnit *getNextEnemyUnit(int currentId){
  int temp = currentId + 1;
  if (temp > enemyUnits.size() - 1){
    temp = 0;
  }
  return getEnemyUnit(temp);
}

void initializeEnemyUnits(){
  enemyUnits.clear();
  GridUnit enemy1g = {0, {17,4}, false, 6.0f, DARKGRAY, false, 2, 1, false};  
  EnemyUnit enemy1 = {1, 0, "Philipe", enemy1g, 25, 6.0f, 3, 3};
  enemy1.updateGridUnit();
  enemyUnits.push_back(enemy1);
  TacticalGrid::addUnitToGrid(enemy1.gridUnit);

  GridUnit enemy2g = {0, {15,5}, false, 6.0f, DARKGRAY, false, 2, 1, false};  
  EnemyUnit enemy2 = {1, 1, "Gretchin", enemy2g, 25, 6.0f, 3, 3};
  enemy2.updateGridUnit();
  enemyUnits.push_back(enemy2);
  TacticalGrid::addUnitToGrid(enemy2.gridUnit);

  GridUnit enemy3g= {0, {16,6}, false, 6.0f, DARKGRAY, false, 2, 1, false};  
  EnemyUnit enemy3 = {1, 2, "Achlan", enemy3g, 25, 6.0f, 3, 3};
  enemy3.updateGridUnit();
  enemyUnits.push_back(enemy3);
  TacticalGrid::addUnitToGrid(enemy3.gridUnit);
}

void drawEnemyUnits(Vector3 worldOrigin){
      for (auto &unit : enemyUnits) {
      if (!unit.gridUnit.isMoving) {
        Vector3 unitPos = {worldOrigin.x + unit.gridUnit.gridPosition.x * TILE_SIZE,
                           worldOrigin.y + 1.0f,
                           worldOrigin.z + unit.gridUnit.gridPosition.y * TILE_SIZE};
        DrawCube(unitPos, 1.5f, 1.0f, 1.5f, unit.gridUnit.color);
      }
    }
}

} // namespace EnemyUnits
