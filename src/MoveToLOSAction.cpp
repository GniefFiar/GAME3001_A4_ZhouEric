#include "MoveToLOSAction.h"
#include "PlayScene.h"
#include "ship.h"
#include "DisplayObject.h"
#include <iostream>

MoveToLOSAction::MoveToLOSAction()
{
	name = "Move To LOS Action";
}

MoveToLOSAction::~MoveToLOSAction()
= default;

void MoveToLOSAction::Action()
{
	std::cout << "Performing Move To LOS Action" << std::endl;
	if (m_pShip->m_state != ActionState::MOVE_TO_LOS)
	{
		m_pShip->m_state = ActionState::MOVE_TO_LOS;		
	}
	m_pTarget = m_pShip->getClosestLOSNode();
	m_pShip->setTarget(m_pTarget);
	if (Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 5.0f)
	{ // If agent is at node, rotate towards target so it actually gets LOS. Add this. 
		m_pShip->rotateToTarget(m_pShip->getTree()->getMap()["target"]);
	}
	else
		m_pShip->move();
}