#include "CloseCombatEnemy.h"

CloseCombatEnemy::CloseCombatEnemy()
{
	TextureManager::Instance().load("../Assets/sprites/CloseCombatEnemy.png", "CCE");

	auto size = TextureManager::Instance().getTextureSize("CCE");
	setWidth(size.x);
	setHeight(size.y);

	m_maxSpeed = 3;
	attackTimer = 0;
	rangedTimer = 0;
	health = 5;

	getTransform()->position = glm::vec2(400.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(AGENT);

	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	// Create the tree.
	m_pTree = new DecisionTree(this); // 'this' is a pointer to the Ship object
	m_pTree->setAgent(this);
	m_pTree->Display(); // Optional.
	m_patrol.reserve(4);

	iceAttack = nullptr;
}

void CloseCombatEnemy::rangedAttack()
{
	
	return;
}

void CloseCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	if (health > 0)
	{
		// draw the ship
		TextureManager::Instance().draw("CCE", x, y, getCurrentHeading(), 255, true);

		if (iceAttack != nullptr)
		{
			TextureManager::Instance().draw("ice", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
		}

		for (unsigned i = 0; i < enemyBullets.size(); i++)
		{
			if (enemyBullets[i] != nullptr)
			{
				TextureManager::Instance().draw("enemyBullet", enemyBullets[i]->getTransform()->position.x, enemyBullets[i]->getTransform()->position.y, 0, 255, true);
			}
		}

		// draw LOS
		Util::DrawLine(getTransform()->position, getTransform()->position + getCurrentDirection() * getLOSDistance(), getLOSColour());
	}

	if (health == 0)
	{
		SoundManager::Instance().allocateChannels(8);
		SoundManager::Instance().load("../Assets/audio/EnemyDeath.wav", "EnemyDeath", SOUND_SFX);
		SoundManager::Instance().playSound("EnemyDeath", 0);
		SoundManager::Instance().setSoundVolume(50);
		getTransform()->position = glm::vec2(-700.0f, -700.0f);
		glm::vec2 deadPosition = getTransform()->position;
		TextureManager::Instance().load("..Assets/sprites/Death Animation.png", "DA");
		TextureManager::Instance().draw("DA", deadPosition.x, deadPosition.y, 0, 255, true);
		health = -1;
	}
}
