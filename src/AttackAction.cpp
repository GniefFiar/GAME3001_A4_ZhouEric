#include "AttackAction.h"
#include "TextureManager.h"
#include <iostream>
#include "../Template/IceAttack.h"

AttackAction::AttackAction()
{
	name = "Attack Action";
}

AttackAction::~AttackAction()
= default;

void AttackAction::Action()
{
	std::cout << "Performing Attack Action" << std::endl;
	if (m_pShip->m_state != ActionState::ATTACK)
	{
		m_pTarget = m_pShip->getTree()->getMap()["target"]; // Again just target/player in map.
		m_pShip->setTarget(m_pTarget);
		m_pShip->m_state = ActionState::ATTACK;
	}

	m_pShip->closeCombatAttack();
}
