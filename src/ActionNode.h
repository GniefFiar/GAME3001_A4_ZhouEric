#pragma once
#ifndef __ACTION_NODE__
#define __ACTION_NODE__
#include "TreeNode.h"
#include "ActionState.h"
#include "Util.h"
#include "ship.h" // New.

// Interface
class ActionNode : public TreeNode
{
protected: // New.
	Ship* m_pShip;
	DisplayObject* m_pTarget;
public:
	ActionNode(): m_pShip(nullptr), m_pTarget(nullptr) { isLeaf = true; }
	virtual ~ActionNode() = default;
	virtual void Action() = 0;

	void SetShip(Ship* ship) { m_pShip = ship; }
	void SetTarget(DisplayObject* t) { m_pTarget = t; }
};

#endif /* defined (__ACTION_NODE__) */