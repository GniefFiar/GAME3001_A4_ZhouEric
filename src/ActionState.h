#pragma once
#ifndef __ACTION_STATE__
#define __ACTION_STATE__

enum ActionState
{
	PATROL,
	MOVE_TO_LOS,
	MOVE_TO_PLAYER,
	ATTACK,
	NO_STATE
};

#endif /* defined(__COLLISION_SHAPE__) */