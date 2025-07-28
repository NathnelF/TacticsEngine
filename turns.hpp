
#pragma once

#include "grid.hpp"
#include <raylib.h>
#include <raymath.h>

namespace TurnSystem {
	enum TurnPhase {
		PHASE_PLAYER,
		PHASE_ENEMY,
	};

	extern TurnPhase currentPhase;

	enum ActionType{
		STEP_ACTION,
		DASH_ACTION,
		SHOOT_ACTION,
		PRIMARY_ACTION,
		FREE_ACTION,
	};
	void initializeTurn(); //reset turn state.  
	bool isPlayerTurn();
	bool isEnemyTurn();

	bool canUnitPerformAction(Unit* unit, ActionType action);
	void executeAction(Unit* unit, ActionType action);

	void endTurn();

	void displayTurnInfo();


}
