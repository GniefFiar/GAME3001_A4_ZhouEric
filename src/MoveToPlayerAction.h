#pragma once
#ifndef __MOVE_TO_PLAYER_ACTION__
#define __MOVE_TO_PLAYER_ACTION__
#include "ActionNode.h"
#include "../Template/CloseCombatEnemy.h"

class MoveToPlayerAction : public ActionNode
{
public:
	MoveToPlayerAction();
	virtual ~MoveToPlayerAction();

	virtual void Action() override;
private:
};

#endif 
