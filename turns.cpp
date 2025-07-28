
#include "turns.hpp"


namespace TurnSystem {


	TurnPhase currentPhase = PHASE_PLAYER; 
	int turnNumber = 1;

	void initializeTurn(){
		for (auto& unit : TacticalGrid::units){
			unit.hasActed = false;
			unit.hasMoved = false;
			unit.hasDashed = false;
			unit.turnComplete = false;
		}
	}

	bool isPlayerTurn(){
		return currentPhase == PHASE_PLAYER;
	}

	bool isEnemyTurn(){
		return currentPhase == PHASE_ENEMY;
	}

	bool canUnitPerformAction(Unit *unit, ActionType action){
		switch (action){
			case STEP_ACTION:
				if (unit->hasMoved || unit-> hasDashed || unit->hasActed || unit->turnComplete) return false;
				else return true;
			case DASH_ACTION:
				if (unit->hasDashed || unit->hasActed || unit->turnComplete) return false;
				else return true;
				
		}
	} 

	void executeAction(Unit* unit, ActionType action){
		switch (action){
			case STEP_ACTION:
				unit->hasMoved = true;
				break;
			case DASH_ACTION:
				unit->hasMoved = true;
				unit->hasDashed = true;
				unit-> hasActed = true;
				unit->turnComplete = true;
				break;
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
