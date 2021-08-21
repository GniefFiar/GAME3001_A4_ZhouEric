#pragma once
#ifndef __ATTACK_ACTION__
#define __ATTACK_ACTION__
#include "ActionNode.h"
#include "IceAttack.h"
#include "../Template/IceAttack.h"
#include <glm/vec4.hpp>
#include "ship.h"

class AttackAction : public ActionNode
{
public:
	AttackAction();
	virtual ~AttackAction();

	virtual void Action() override;
private:
	int attackTimer;
	IceAttack* iceAttack;
};

#endif /* defined (__PATROL_ACTION__) */
