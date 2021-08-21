#include "MoveToPlayerAction.h"
#include <iostream>

MoveToPlayerAction::MoveToPlayerAction()
{
	name = "Move To Player Action";
}

MoveToPlayerAction::~MoveToPlayerAction()
= default;

void MoveToPlayerAction::Action()
{
	std::cout << "Performing Move To Player Action" << std::endl;
	if (m_pShip->m_state != ActionState::MOVE_TO_PLAYER) // "enter/init"
	{
		m_pTarget = m_pShip->getTree()->getMap()["target"]; // Again just target/player in map.
		m_pShip->setTarget(m_pTarget);
		m_pShip->m_state = ActionState::MOVE_TO_PLAYER;
	}

	m_pShip->rangedAttack();

	m_pShip->move();
}
