
#include "turns.hpp"
#include "abilities.hpp"


namespace TurnSystem {


	TurnPhase currentPhase = PHASE_PLAYER; 
	int turnNumber = 1;

	void initializeTurn(){
		for (auto& unit : TacticalGrid::units){
			unit.movePointsRemaining = 2;
			unit.actionPointsRemaining = 1;
			unit.turnComplete = false;
		}
	}

	bool isPlayerTurn(){
		return currentPhase == PHASE_PLAYER;
	}

	bool isEnemyTurn(){
		return currentPhase == PHASE_ENEMY;
	}

	bool canUnitPerformAction(Unit *unit, AbilityID abilityId){
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

	void executeAction(Unit* unit, AbilityID abilityId, Vector2 target){
		//get the ability
	
		AbilityDefinition* ability = AbilityRegistry::getAbility(abilityId);
		const AbilityData& data =ability->data;
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
