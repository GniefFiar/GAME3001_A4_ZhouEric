#include "ship.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Ship::Ship() : m_maxSpeed(2.0f), m_waypoint(0), m_pTarget(nullptr), m_state(ActionState::NO_STATE), m_ctr(0)
{
	TextureManager::Instance().load("../Assets/sprites/Galactic Nova Nucleus.png", "ship");

	auto size = TextureManager::Instance().getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
	attackTimer = 0;
	rangedTimer = 0;
	health = 5;

	getTransform()->position = glm::vec2(400.0f, 300.0f);
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
	// m_pTree->setAgent(this);
	m_pTree->Display(); // Optional.
	m_patrol.reserve(4);

	iceAttack = nullptr;
}

Ship::~Ship()
= default;

void Ship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	if (health > 0)
	{
		// draw the ship
		TextureManager::Instance().draw("ship", x, y, getCurrentHeading(), 255, true);

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


void Ship::update()
{
	getTree()->Update(); // Tell the ship what to do.
	if (m_ctr++ % 60 == 0)
		getTree()->MakeDecision(); // Check for a new potential action.
}

void Ship::clean(){}

void Ship::turnRight()
{
	setCurrentHeading(getCurrentHeading() + m_turnRate);
	if (getCurrentHeading() >= 360)
	{
		setCurrentHeading(getCurrentHeading() - 360.0f);
	}
}

void Ship::turnLeft()
{
	setCurrentHeading(getCurrentHeading() - m_turnRate);
	if (getCurrentHeading() < 0)
	{
		setCurrentHeading(getCurrentHeading() + 360.0f);
	}
}

void Ship::moveForward()
{
	getTransform()->position += getCurrentDirection() * m_maxSpeed;
	//getRigidBody()->velocity = getCurrentDirection() * m_maxSpeed;
}

void Ship::moveBack()
{
	//getRigidBody()->velocity = getCurrentDirection() * -m_maxSpeed;
}

void Ship::move()
{
	rotate();
	moveForward();
	/*getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;*/
}

void Ship::rotate()
{
	// Step 1: Find angle between two points.
	// Util::angle(getTransform()->position, m_pTarget->getTransform()->position)

	// Step 2: Plug angle into a lerp.
	// Util::lerpAngle(getCurrentHeading(), <destination angle>, 0.1f)

	// Step 3: Set lerped angle into currentHeading
	// setCurrentHeading()

	setCurrentHeading(Util::lerpAngle(getCurrentHeading(), 
		Util::angle(getTransform()->position, m_pTarget->getTransform()->position), 0.1f));
}

void Ship::rotateToTarget(DisplayObject* target)
{
	setCurrentHeading(Util::lerpAngle(getCurrentHeading(), Util::angle(getTransform()->position, target->getTransform()->position), 0.25f));
}

void Ship::closeCombatAttack()
{
	TextureManager::Instance().load("../Assets/sprites/Close Combat Attack.png", "ice");

	auto size = TextureManager::Instance().getTextureSize("ice");

	attackTimer++;

	if (attackTimer == 120)
	{
		iceAttack = new IceAttack();
		std::cout << "New ice attack!" << std::endl;
		iceAttack->getTransform()->position = getTransform()->position;
		attackTimer = 0;
	}

	if (iceAttack != nullptr)
	{
		iceAttack->update();
	}

	if (iceAttack != nullptr && iceAttack->getTimer() == 0)
	{
		delete iceAttack;
		iceAttack = nullptr;
	}
}

void Ship::rangedAttack()
{
	TextureManager::Instance().load("../Assets/sprites/Bullet.png", "enemyBullet");
	auto size = TextureManager::Instance().getTextureSize("enemyBullet");

	rangedTimer++;

	if (rangedTimer == 20)
	{
		std::cout << "fired!" << std::endl;
		enemyBullets.push_back(new EnemyBullet(getCurrentDirection()));
		enemyBullets.shrink_to_fit();
		enemyBullets[enemyBullets.size() - 1]->getTransform()->position = getTransform()->position;

		rangedTimer = 0;
	}

	for (unsigned i = 0; i < enemyBullets.size(); i++)
	{
		enemyBullets[i]->getTransform()->position += glm::vec2(5.0f, 5.0f) * enemyBullets[i]->getTrajectory();
	}

	for (unsigned i = 0; i < enemyBullets.size(); i++)
	{
		if (enemyBullets[i] != nullptr)
		{
			if (enemyBullets[i]->getTransform()->position.x < 0 || enemyBullets[i]->getTransform()->position.x > 850 ||
				enemyBullets[i]->getTransform()->position.y < 0 || enemyBullets[i]->getTransform()->position.y > 650)
			{
				delete enemyBullets[i];
				enemyBullets[i] = nullptr;
				enemyBullets.erase(enemyBullets.begin() + i);
				enemyBullets.shrink_to_fit();
			}
			break;
		}
	}
}


float Ship::getMaxSpeed() const
{
	return m_maxSpeed;
}

DisplayObject* Ship::getTarget()
{
	return m_pTarget;
}

std::vector<DisplayObject*>& Ship::getPatrol()
{
	return m_patrol;
}

DecisionTree* Ship::getTree()
{
	return m_pTree;
}

void Ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Ship::setTarget(DisplayObject* target)
{
	m_pTarget = target;
}

void Ship::setClosestLOSNode(PathNode* node)
{
	m_pLOSNode = node;
}

DisplayObject* Ship::getCurrentWaypoint()
{
	return m_patrol[m_waypoint];
}

void Ship::resumePatrol()
{
	setTarget(m_patrol[m_waypoint]); // m_waypoint is current PathNode to go to
	m_state = ActionState::PATROL;
}

void Ship::setHealth(int amount)
{
	health -= amount;
}

DisplayObject* Ship::getNextWaypoint()
{
	m_waypoint++;
	if (m_waypoint == 4) m_waypoint = 0;
	setTarget(m_patrol[m_waypoint]);
	return m_pTarget;
}

PathNode* Ship::getClosestLOSNode()
{
	return m_pLOSNode;
}

IceAttack* Ship::getIceAttack()
{
	return iceAttack;
}

int Ship::getHealth()
{
	return health;
}

void Ship::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Ship::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}