
#include "abilities.hpp"
#include "grid.hpp"
#include "movement.hpp"
#include <raymath.h>


namespace AbilityRegistry{
	std::unordered_map<AbilityID, AbilityDefinition> abilities;

	void executeStep(GridUnit* unit, Vector2 target){
		if (TacticalGrid::waypoints.empty()){
			std::vector<Vector2> path = TacticalGrid::reconstructPath(unit->gridPosition.x, unit->gridPosition.y, target.x, target.y);
			Movement::setPath(unit, path);
			TacticalGrid::setMovementDisplayDash(unit);
		}	  
		else {
			PathData pathData = TacticalGrid::calculateWaypointPath(unit, target);
			std::vector<Vector2> path = pathData.path;	
			std::cout << "waypoint path : " << path << std::endl;
			Movement::setPath(unit, path);
			TacticalGrid::setMovementDisplayDash(unit);

		
	 	} 

	}

	void executeDash(GridUnit* unit, Vector2 target){
		if (TacticalGrid::waypoints.empty()){
			std::vector<Vector2> path = TacticalGrid::reconstructPath(unit->gridPosition.x, unit->gridPosition.y, target.x, target.y);
			Movement::setPath(unit, path);
			TacticalGrid::clearMovementGrid();
		}
		else {
			PathData pathData = TacticalGrid::calculateWaypointPath(unit, target);
			std::vector<Vector2> path = pathData.path;	
			Movement::setPath(unit, path);
			TacticalGrid::clearMovementGrid();
		}
	}

	void initializeRegistry(){
	//add abiltiies to registry here.
		abilities[ABILITY_STEP] = {
			{ABILITY_STEP, "Step", "Move and retain actions", 1, 0, false, -1},
			executeStep
		};	
		 abilities[ABILITY_DASH] = {
			{ABILITY_DASH, "Dash", "Dash full movement", 2, 1, false, -1},
			executeDash
		};	 
	}

	AbilityDefinition* getAbility(AbilityID id) {
  	auto it = abilities.find(id);
		if (it != abilities.end()){
			return &it->second;
		}
		else {
			return nullptr;
		}
	}

	std::vector<AbilityID> getAllAbilityIDs(){
		std::vector<AbilityID> ids;
		for (const auto& ability : abilities){
			ids.push_back(ability.first);
		}
		return ids;
	}






}

