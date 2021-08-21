#pragma once
#ifndef __RANGEDENEMY__
#define __RANGEDENEMY__

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

class RangedEnemy : public Ship
{
private:



public:
	RangedEnemy();
	void draw();
	void rangedAttack();


};
#endif