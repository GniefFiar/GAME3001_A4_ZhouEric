#pragma once
#ifndef __CLOSECOMBATENEMY__
#define __CLOSECOMBATENEMY__

#include "Agent.h"
#include "TextureManager.h"
#include "DecisionTree.h"
#include "ActionState.h"
#include "PathNode.h"
#include <glm/vec4.hpp>
#include "../Template/IceAttack.h"
#include "EnemyBullet.h"
#include "../Template/EnemyBullet.h"
#include "SoundManager.h"
#include "Ship.h"
#include "Util.h"

class CloseCombatEnemy : public Ship
{
private:



public:
	CloseCombatEnemy();
	void draw();
	void rangedAttack();


};
#endif