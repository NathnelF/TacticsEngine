
#include <algorithm>
#include "abilities.hpp"
#include "enemy_unit.hpp"
#include "grid.hpp"
#include "movement.hpp"
#include "player_unit.hpp"
#include "cover.hpp"
#include <raymath.h>
#include <cstdlib>
#include <ctime>

int calculateHitChance(GridUnit unit, GridLocation target){
  int coverBonus = CoverSystem::getTargetCoverBonus(unit.gridPosition, target);
  PlayerUnit* player = PlayerUnits::getPlayerUnit(unit.id);
  if (player == nullptr){
    return -1;
  }
  int hitChance = 50 + player->aimStat - coverBonus; //TODO: use enemy defense?
  if (hitChance < 0){
    hitChance = 0;
  }
  return hitChance;
}

namespace AbilityRegistry {
std::unordered_map<AbilityID, AbilityDefinition> abilities;

void executeStep(GridUnit *unit, GridLocation target) {
  //Check if waypoints exist.
  if (TacticalGrid::waypoints.empty()) {
    //if no waypoints
    //Just reconstruct path to destination and intialize movement on that path
    std::vector<GridLocation> path = TacticalGrid::reconstructPath(
        unit->gridPosition.x, unit->gridPosition.y, unit->gridPosition.z, target.x, target.y, target.z);
    Movement::setPath(unit, path);
    //Set movement display to show one remaining move
    TacticalGrid::setMovementDisplayDash(unit);
  } else {
    //if there are waypoints
    // we get the path along all the waypoints ad initialize movement on that path
    PathData pathData = TacticalGrid::calculateWaypointPath(unit, target);
    std::vector<GridLocation> path = pathData.path;
    Movement::setPath(unit, path);
    //set movement display to show one remaining move
    TacticalGrid::setMovementDisplayDash(unit);
  }
}

void executeDash(GridUnit *unit, GridLocation target) {
  //check if waypoints exist
  if (TacticalGrid::waypoints.empty()) {
    //if no waypoints
    //Just reconstruct path to destination and intialize movement on that path
    std::vector<GridLocation> path = TacticalGrid::reconstructPath(
        unit->gridPosition.x, unit->gridPosition.y, unit->gridPosition.z, target.x, target.y, target.z);
    Movement::setPath(unit, path);
    //Set movement display to show no valid moves left
    TacticalGrid::clearMovementGrid();
  } else {
    //if waypoints
    // get the path along the waypoints and initialize movement on that path
    PathData pathData = TacticalGrid::calculateWaypointPath(unit, target);
    std::vector<GridLocation> path = pathData.path;
    Movement::setPath(unit, path);
    //set movement display to show no valid moves left
    TacticalGrid::clearMovementGrid();
  }
}

void executeShootPrimary(GridUnit *unit, GridLocation target) {
  // do we check line of sight here or somewhere else?
  //  Assume line of sight is valid for now.
  //  First need to check if this is a player or enemy unit doing the
  //  shooting.
  if (target.x == -1 && target.y == -1){
    std::cout << "Target not found\n";
    return;
  }
  if (unit->id < 10) {
    // player Unit
    // Second we need to get enemy at the target square
    int unitId = TacticalGrid::unitGrid[target.z][target.y][target.x];
    if (unitId == -1 || unitId < 10) {
      // not a valid shot
      //  unitId of -1 means there is no unit on that square
      //  unitId of < 10 means there is a friendly unit on that square.
      std::cout << "not a valid shot\n";
      return;
    } 
    srand(time(0));
    //Third we get the Units' stats
    PlayerUnit* playerUnit = PlayerUnits::getPlayerUnit(unit->id);
    EnemyUnit* enemyUnit = EnemyUnits::getEnemyUnit(unitId - 10);
    std::cout << playerUnit->name << " is shooting " << enemyUnit->name << " \n";
    //calculate cover between player and enemy.
    int coverBonus = CoverSystem::getTargetCoverBonus(unit->gridPosition, target);
    std::cout << "cover bonus of " << coverBonus << " \n";
    //calculate hit change
    int hitChance = 50 + playerUnit->aimStat - coverBonus; //defense? equipment bonus?
    std::cout << "Shot has " << hitChance << " % accuracy.\n";
    // roll to hit
    int hitRoll = rand() % 101;
    bool hit = false;
    std::cout << "Rolled a " << hitRoll << " to hit\n";
    if (hitRoll < hitChance){
	     hit = true;	
    }
    if (hit){
    	int damage = 1; //set this constant for now
    	enemyUnit->currentHealth -= damage;
    	std::cout << "Shot hit! " << enemyUnit->name << " takes 1 point of damage! They have " << enemyUnit->currentHealth << " health remaining.\n";
    	if (enemyUnit->currentHealth <= 0){
    		std::cout << enemyUnit->name << " died! You monster.\n";
    		//TODO: remove unit from vector.
    		enemyUnit->gridUnit.color = PINK;
    		TacticalGrid::unitGrid[target.z][target.y][target.x] = -1;
    	}
    }
    else {
    	std::cout << "Shot missed!\n";
    }
    TacticalGrid::clearMovementGrid();
    return;
  }
  else {
  	//enemy Unit
  	// We need to get the player at the target square
  	int unitId = TacticalGrid::unitGrid[target.z][target.y][target.x] ;
  	if (unitId == -1 || unitId >= 10){
  		//square is empty or is fellow enemy
  		return;
  	}
  	srand(time(0));
  	PlayerUnit* playerUnit = PlayerUnits::getPlayerUnit(unitId);
  	EnemyUnit* enemyUnit = EnemyUnits::getEnemyUnit(unit->id - 10);
    std::cout << enemyUnit->name << " is shooting " << playerUnit->name << " \n";
    //calculate cover between player and enemy.
    int coverBonus = CoverSystem::getTargetCoverBonus(unit->gridPosition, target);
    //calculate hit change
    int hitChance = 50 + enemyUnit->aimStat - coverBonus; //defense? equipment bonus?
    std::cout << "Shot has " << hitChance << " % accuracy.\n";
    // roll to hit
    int hitRoll = rand() % 101;
    bool hit = false;
    std::cout << "Rolled a " << hitRoll << " to hit\n";
    if (hitRoll < hitChance){
	     hit = true;	
    }
    if (hit){
    	int damage = 1; //set this constant for now
    	playerUnit->currentHealth -= damage;
    	std::cout << "Shot hit! " << playerUnit->name << " takes 1 point of damage! They have " << playerUnit->currentHealth << " health remaining.\n";
    }
    else {
    	std::cout << "Shot missed!\n";
    }
    return;
  }
}

void initializeRegistry() {
  // add abiltiies to registry here.
  abilities[ABILITY_STEP] = {
      {ABILITY_STEP, "Step", "Move and retain actions", 1, 0, false, false, -1},
      executeStep};
  abilities[ABILITY_DASH] = {
      {ABILITY_DASH, "Dash", "Dash full movement", 1, 1, false, false, -1},
      executeDash};
  abilities[ABILITY_SHOOT_PRIMARY] = {
  		{ABILITY_SHOOT_PRIMARY, "Shoot (Primary)", "Take a shot with your primary weapon", 0, 1, false, true, -1},
  		executeShootPrimary};
  
}

AbilityDefinition *getAbility(AbilityID id) {
  auto it = abilities.find(id);
  if (it != abilities.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}

std::vector<AbilityID> getAllAbilityIDs() {
  std::vector<AbilityID> ids;
  for (const auto &ability : abilities) {
    ids.push_back(ability.first);
  }
  return ids;
}

} // namespace AbilityRegistry
