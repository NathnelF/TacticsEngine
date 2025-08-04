
#pragma once

#include "grid.hpp"
#include <unordered_map>
#include <vector>

typedef int AbilityID;

const AbilityID ABILITY_STEP = 1;
const AbilityID ABILITY_DASH = 2;
//put ids here


struct AbilityData{
	AbilityID id;
	std::string name;
	std::string descr;

	int movePointCost;
	int actionPointCost;

	//int ammoCost
	
	bool endsTurn;
	//bool requiresTarget
	//bool requiresAmmo
	//bool requiresConsumable (grenade)
	//bool requiresLineOfSight
	
	int maxUsesPerMission;

	//bool requiesWeapon (weapon type?)
	//

};

typedef void (*AbilityExecuteFunction)(GridUnit* unit, Vector2 target);

struct AbilityDefinition{
	AbilityData data;
	AbilityExecuteFunction executeFunction;
};

struct UnitAbilityState{
	//tracks ability usage
	std::unordered_map<AbilityID, int> usesThisMission;
};

namespace AbilityRegistry{
	void initializeRegistry();
	AbilityDefinition* getAbility(AbilityID id);
	std::vector<AbilityID> getAllAbilityIDs();
}
