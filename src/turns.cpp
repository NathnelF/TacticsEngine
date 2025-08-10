
#include "turns.hpp"
#include "abilities.hpp"
#include "player_unit.hpp"
#include "los.hpp"


namespace TurnSystem {


	TurnPhase currentPhase = PHASE_PLAYER; 
	int turnNumber = 1;

	void initializeTurn(){
		//TODO: initialize enemy turns as well
		for (auto& unit : PlayerUnits::playerUnits){
			unit.gridUnit.movePointsRemaining = 2;
			unit.gridUnit.actionPointsRemaining = 1;
			unit.gridUnit.turnComplete = false;
		}
	}

	bool isPlayerTurn(){
		return currentPhase == PHASE_PLAYER;
	}

	bool isEnemyTurn(){
		return currentPhase == PHASE_ENEMY;
	}

	bool canUnitPerformAction(GridUnit*unit, AbilityID abilityId){
		//get the ability
		AbilityDefinition* ability = AbilityRegistry::getAbility(abilityId);
		const AbilityData& data = ability->data;
		
		if (data.movePointCost > unit->movePointsRemaining) return false;
		if (data.actionPointCost > unit->actionPointsRemaining) return false;


		//check usage limits as well
		// usage per turn
		// usage per mission
		return true;
	} 

	void executeAction(GridUnit* unit, AbilityID abilityId, Vector2 target){
		//get the ability
		//
		if (!canUnitPerformAction(unit, abilityId)){
			std::cout << "can't perform action now\n";
			return;
		}
	
		AbilityDefinition* ability = AbilityRegistry::getAbility(abilityId);
		const AbilityData& data =ability->data;
		//check for line of sight
		if (data.requiresLineOfSight){
			//check LOS to target
			bool hasLOS = LineOfSight::calculateLOS(unit->gridPosition, target);
			std::cout << "has los " << hasLOS << std::endl;
			if (!hasLOS){
				std::cout << "unit does not have LOS to target!\n";
				return;
			}
		}
		//apply ability costs to unit
		unit->movePointsRemaining -= data.movePointCost;
		unit->actionPointsRemaining -= data.actionPointCost;

		//apply turn / mission usage

		ability->executeFunction(unit, target);
		//execute action
		if (data.endsTurn || unit->movePointsRemaining <= 0 || unit-> actionPointsRemaining <= 0){
			unit->turnComplete = true;
		}	
	
		
	}	


	void endTurn(){
		//for now we just will reinitialize the turn!
		initializeTurn();
		turnNumber++;
	}

	void displayTurnInfo(){
		DrawText(TextFormat("Turn number: %d ",  turnNumber), 300, 750, 30, BLACK);
	}

}
