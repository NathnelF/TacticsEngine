
#pragma once
#include "grid.hpp"

struct EnemyUnit {
  // TODO: Pull data from external source of truth.
  int enemyType;
  int id;
  std::string name;
  GridUnit gridUnit;
  int aimStat;
  float speedStat;
  int maxHealth;
  int currentHealth;

  // TODO: Abilities.

  void updateGridUnit() {
    gridUnit.speed = speedStat;
    gridUnit.playerControlled = false;
    gridUnit.id = id + 10; //remember this (enemy units all have a grid of > 10. Player units will have an id < 10)
  }
};

namespace EnemyUnits {
extern std::vector<EnemyUnit> enemyUnits;
EnemyUnit* getEnemyUnit(int id);
EnemyUnit* getNextEnemyUnit(int currentId);
void initializeEnemyUnits(); // TODO: get enemy units from an external source
void drawEnemyUnits(Vector3 worldOrigin);

} // namespace EnemyUnits

// registry somewhere as well
