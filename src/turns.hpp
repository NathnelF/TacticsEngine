
#pragma once

#include "grid.hpp"
#include <raylib.h>
#include <raymath.h>
#include "abilities.hpp"

namespace TurnSystem {
	enum TurnPhase {
		PHASE_PLAYER,
		PHASE_ENEMY,
	};

	extern TurnPhase currentPhase;

	
	void initializeTurn(); //reset turn state.  
	bool isPlayerTurn();
	bool isEnemyTurn();

	bool canUnitPerformAction(Unit* unit, AbilityID abilityId);
	void executeAction(Unit* unit, AbilityID abilityId, Vector2 target);

	void endTurn();

	void displayTurnInfo();


}
