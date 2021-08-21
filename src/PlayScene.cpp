#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Transition.h"
#include "Attack.h"
#include "MoveToLOS.h"
#include "MoveToPlayer.h"
#include "Patrol.h"
#include "PlayerAnimationState.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr)
		{
			TextureManager::Instance().draw("Laser", m_lasers[i]->getTransform()->position.x, m_lasers[i]->getTransform()->position.y, 0, 255, true);
		}
	}
}

void PlayScene::update()
{
	// Set three conditions of decision tree here.
	m_pShip->getTree()->getLOSNode()->setLOS(m_pShip->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.

	m_pShip->getTree()->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 400.0f);

	m_pShip->getTree()->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 60.0f);

	// Set three conditions of decision tree here.
	m_pCloseCombatEnemy->getTree()->getLOSNode()->setLOS(m_pCloseCombatEnemy->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.

	m_pCloseCombatEnemy->getTree()->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pCloseCombatEnemy->getTransform()->position, m_pTarget->getTransform()->position) <= 400.0f);

	m_pCloseCombatEnemy->getTree()->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pCloseCombatEnemy->getTransform()->position, m_pTarget->getTransform()->position) <= 60.0f);

	m_pRangedEnemy->getTree()->getLOSNode()->setLOS(m_pShip->hasLOS()); // Or use m_pShip if you want to be lazy/wrong.

	m_pRangedEnemy->getTree()->getRadiusNode()->setIsWithinRadius(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 400.0f);

	m_pRangedEnemy->getTree()->getCloseCombatNode()->setIsWithinCombatRange(
		Util::distance(m_pShip->getTransform()->position, m_pTarget->getTransform()->position) <= 60.0f);

	updateDisplayList();
	
	m_CheckAgentLOS(m_pShip, m_pTarget);
	m_CheckPathNodeLOS(m_pTarget);

	m_CheckAgentLOS(m_pCloseCombatEnemy, m_pTarget);
	m_findClosestPathNode(m_pCloseCombatEnemy, m_pTarget);

	m_CheckAgentLOS(m_pRangedEnemy, m_pTarget);
	m_findClosestPathNode(m_pRangedEnemy, m_pTarget);

	m_findClosestPathNode(m_pShip, m_pTarget);

	// Color the patrol nodes...
	m_pGrid[21]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGrid[85]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGrid[185]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pGrid[261]->setLOSColour(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	if (m_pTarget->getAttackCooldown() > 0)
	{
		m_pTarget->setAttackCooldown(1);
	}

	if (m_pTarget->getIFrames() > 0)
	{
		m_pTarget->setIFrames(1);
	}

	if (m_pShip->iceAttack != nullptr && m_pTarget->getIFrames() == 0 && CollisionManager::AABBCheck(m_pTarget, m_pShip->iceAttack))
	{
		m_pTarget->setHealth(1);
		m_pTarget->setIFrames(-120);
	}

	if (m_pCloseCombatEnemy->iceAttack != nullptr && m_pTarget->getIFrames() == 0 && CollisionManager::AABBCheck(m_pTarget, m_pCloseCombatEnemy->iceAttack))
	{
		m_pTarget->setHealth(1);
		m_pTarget->setIFrames(-120);
	}

	for (unsigned i = 0; i < m_pShip->enemyBullets.size(); i++)
	{
		if (m_pShip->enemyBullets[i] != nullptr && m_pTarget->getIFrames() == 0 && CollisionManager::AABBCheck(m_pTarget, m_pShip->enemyBullets[i]))
		{
			m_pTarget->setHealth(1);
			m_pTarget->setIFrames(-120);
			delete m_pShip->enemyBullets[i];
			m_pShip->enemyBullets[i] = nullptr;
			m_pShip->enemyBullets.erase(m_pShip->enemyBullets.begin() + i);
			m_pShip->enemyBullets.shrink_to_fit();
			std::cout << "Hit!" << std::endl;
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/PlayerDamage.wav", "PlayerDamage", SOUND_SFX);
			SoundManager::Instance().playSound("PlayerDamage", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_pShip->enemyBullets.size(); i++)
	{
		if (m_pShip->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle1, m_pShip->enemyBullets[i]))
		{
			m_pDestructibleObstacle1->setHealth(1);
			delete m_pShip->enemyBullets[i];
			m_pShip->enemyBullets[i] = nullptr;
			m_pShip->enemyBullets.erase(m_pShip->enemyBullets.begin() + i);
			m_pShip->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pRangedEnemy->enemyBullets.size(); i++)
	{
		if (m_pRangedEnemy->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle1, m_pRangedEnemy->enemyBullets[i]))
		{
			m_pDestructibleObstacle1->setHealth(1);
			delete m_pRangedEnemy->enemyBullets[i];
			m_pRangedEnemy->enemyBullets[i] = nullptr;
			m_pRangedEnemy->enemyBullets.erase(m_pRangedEnemy->enemyBullets.begin() + i);
			m_pRangedEnemy->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pShip->enemyBullets.size(); i++)
	{
		if (m_pShip->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle2, m_pShip->enemyBullets[i]))
		{
			m_pDestructibleObstacle2->setHealth(1);
			delete m_pShip->enemyBullets[i];
			m_pShip->enemyBullets[i] = nullptr;
			m_pShip->enemyBullets.erase(m_pShip->enemyBullets.begin() + i);
			m_pShip->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pRangedEnemy->enemyBullets.size(); i++)
	{
		if (m_pRangedEnemy->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle2, m_pRangedEnemy->enemyBullets[i]))
		{
			m_pDestructibleObstacle2->setHealth(1);
			delete m_pRangedEnemy->enemyBullets[i];
			m_pRangedEnemy->enemyBullets[i] = nullptr;
			m_pRangedEnemy->enemyBullets.erase(m_pRangedEnemy->enemyBullets.begin() + i);
			m_pRangedEnemy->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pShip->enemyBullets.size(); i++)
	{
		if (m_pShip->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle3, m_pShip->enemyBullets[i]))
		{
			m_pDestructibleObstacle3->setHealth(1);
			delete m_pShip->enemyBullets[i];
			m_pShip->enemyBullets[i] = nullptr;
			m_pShip->enemyBullets.erase(m_pShip->enemyBullets.begin() + i);
			m_pShip->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pRangedEnemy->enemyBullets.size(); i++)
	{
		if (m_pRangedEnemy->enemyBullets[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle3, m_pRangedEnemy->enemyBullets[i]))
		{
			m_pDestructibleObstacle3->setHealth(1);
			delete m_pRangedEnemy->enemyBullets[i];
			m_pRangedEnemy->enemyBullets[i] = nullptr;
			m_pRangedEnemy->enemyBullets.erase(m_pRangedEnemy->enemyBullets.begin() + i);
			m_pRangedEnemy->enemyBullets.shrink_to_fit();
		}
	}

	for (unsigned i = 0; i < m_pRangedEnemy->enemyBullets.size(); i++)
	{
		if (m_pRangedEnemy->enemyBullets[i] != nullptr && m_pTarget->getIFrames() == 0 && CollisionManager::AABBCheck(m_pTarget, m_pRangedEnemy->enemyBullets[i]))
		{
			m_pTarget->setHealth(1);
			m_pTarget->setIFrames(-120);
			delete m_pRangedEnemy->enemyBullets[i];
			m_pRangedEnemy->enemyBullets[i] = nullptr;
			m_pRangedEnemy->enemyBullets.erase(m_pRangedEnemy->enemyBullets.begin() + i);
			m_pRangedEnemy->enemyBullets.shrink_to_fit();
			std::cout << "Hit!" << std::endl;
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/PlayerDamage.wav", "PlayerDamage", SOUND_SFX);
			SoundManager::Instance().playSound("PlayerDamage", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	if (m_pTarget->getHealth() <= 0)
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		m_lasers[i]->getTransform()->position += glm::vec2(5.0f, 5.0f) * m_lasers[i]->getTrajectory();
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr)
		{
			if (m_lasers[i]->getTransform()->position.x < 0 || m_lasers[i]->getTransform()->position.x > 850 ||
				m_lasers[i]->getTransform()->position.y < 0 || m_lasers[i]->getTransform()->position.y > 650)
			{
				delete m_lasers[i];
				m_lasers[i] = nullptr;
				m_lasers.erase(m_lasers.begin() + i);
				m_lasers.shrink_to_fit();
			}
			break;
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pShip, m_lasers[i]))
		{
			m_pShip->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/EnemyHit.wav", "EnemyHit", SOUND_SFX);
			SoundManager::Instance().playSound("EnemyHit", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pCloseCombatEnemy, m_lasers[i]))
		{
			m_pCloseCombatEnemy->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/EnemyHit.wav", "EnemyHit", SOUND_SFX);
			SoundManager::Instance().playSound("EnemyHit", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pRangedEnemy, m_lasers[i]))
		{
			m_pRangedEnemy->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/EnemyHit.wav", "EnemyHit", SOUND_SFX);
			SoundManager::Instance().playSound("EnemyHit", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle1, m_lasers[i]))
		{
			m_pDestructibleObstacle1->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/ObstacleDamage.wav", "ObstacleDamage", SOUND_SFX);
			SoundManager::Instance().playSound("ObstacleDamage", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle2, m_lasers[i]))
		{
			m_pDestructibleObstacle2->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/ObstacleDamage.wav", "ObstacleDamage", SOUND_SFX);
			SoundManager::Instance().playSound("ObstacleDamage", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	for (unsigned i = 0; i < m_lasers.size(); i++)
	{
		if (m_lasers[i] != nullptr && CollisionManager::AABBCheck(m_pDestructibleObstacle3, m_lasers[i]))
		{
			m_pDestructibleObstacle3->setHealth(1);
			delete m_lasers[i];
			m_lasers[i] = nullptr;
			m_lasers.erase(m_lasers.begin() + i);
			m_lasers.shrink_to_fit();
			SoundManager::Instance().allocateChannels(8);
			SoundManager::Instance().load("../Assets/audio/ObstacleDamage.wav", "ObstacleDamage", SOUND_SFX);
			SoundManager::Instance().playSound("ObstacleDamage", 0);
			SoundManager::Instance().setSoundVolume(50);
		}
	}

	if (m_pShip->getHealth() == -1)
	{
		killCount += 1;
		m_pShip->setHealth(1);
	}

	if (m_pCloseCombatEnemy->getHealth() == -1)
	{
		killCount += 1;
		m_pCloseCombatEnemy->setHealth(1);
	}

	if (m_pRangedEnemy->getHealth() == -1)
	{
		killCount += 1;
		m_pRangedEnemy->setHealth(1);
	}

	if (killCount == 3)
	{
		TheGame::Instance().changeSceneState(WIN_SCENE);
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
	{
		m_pTarget->getTransform()->position -= glm::vec2(0.0f, 2.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	{
		m_pTarget->setAnimationState(PLAYER_RUN_LEFT);
		m_pTarget->getTransform()->position -= glm::vec2(2.0f, 0.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
	{
		m_pTarget->getTransform()->position += glm::vec2(0.0f, 2.0f);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
	{
		m_pTarget->setAnimationState(PLAYER_RUN_RIGHT);
		m_pTarget->getTransform()->position += glm::vec2(2.0f, 0.0f);
	}

	if (m_pTarget->getAttackCooldown() <= 0 && EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		TextureManager::Instance().load("../Assets/sprites/Laser.png", "Laser");
		glm::vec2 temptraj;
		if (m_pTarget->getCurrentAnimationState() == PLAYER_IDLE_LEFT || m_pTarget->getCurrentAnimationState() == PLAYER_RUN_LEFT)
		{
			temptraj = glm::vec2(-1.0f, 0.0f);
		}
		else
		{
			temptraj = glm::vec2(1.0f, 0.0f);
		}
		m_lasers.push_back(new Laser(temptraj));
		m_lasers.shrink_to_fit();
		m_lasers[m_lasers.size() - 1]->getTransform()->position = m_pTarget->getTransform()->position;

		m_pTarget->setAttackCooldown(-30);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	SoundManager::Instance().allocateChannels(16);
	SoundManager::Instance().load("../Assets/audio/Zook.ogg", "Zook", SOUND_MUSIC);
	SoundManager::Instance().playMusic("Zook", -1);
	SoundManager::Instance().setMusicVolume(50);

	m_buildGrid();

	// add the ship to the scene as a start point
	m_pShip = new Ship();
	m_pShip->getTransform()->position = glm::vec2(150.f, 300.f);
	addChild(m_pShip, 3);

	// Set the ship's patrol route.
	m_pShip->getPatrol().push_back(m_pGrid[21]);
	m_pShip->getPatrol().push_back(m_pGrid[85]);
	m_pShip->getPatrol().push_back(m_pGrid[185]);
	m_pShip->getPatrol().push_back(m_pGrid[261]);
	m_pShip->setTarget(m_pShip->getPatrol().front());

	m_pCloseCombatEnemy = new CloseCombatEnemy();
	m_pCloseCombatEnemy->getTransform()->position = glm::vec2(150.f, 100.f);
	addChild(m_pCloseCombatEnemy);
	m_pCloseCombatEnemy->getPatrol().push_back(m_pGrid[21]);
	m_pCloseCombatEnemy->getPatrol().push_back(m_pGrid[85]);
	m_pCloseCombatEnemy->getPatrol().push_back(m_pGrid[185]);
	m_pCloseCombatEnemy->getPatrol().push_back(m_pGrid[261]);
	m_pCloseCombatEnemy->setTarget(m_pCloseCombatEnemy->getPatrol().front());

	m_pRangedEnemy = new RangedEnemy();
	m_pRangedEnemy->getTransform()->position = glm::vec2(150.f, 100.f);
	addChild(m_pRangedEnemy);
	m_pRangedEnemy->getPatrol().push_back(m_pGrid[21]);
	m_pRangedEnemy->getPatrol().push_back(m_pGrid[85]);
	m_pRangedEnemy->getPatrol().push_back(m_pGrid[185]);
	m_pRangedEnemy->getPatrol().push_back(m_pGrid[261]);
	m_pRangedEnemy->setTarget(m_pRangedEnemy->getPatrol().front());

	// add the Obstacle to the scene as a start point
	m_pObstacle1 = new Obstacle();
	m_pObstacle1->getTransform()->position = glm::vec2(380.f, 280.f);
	addChild(m_pObstacle1);

	// add the Obstacle to the scene as a start point
	m_pObstacle2 = new Obstacle();
	m_pObstacle2->getTransform()->position = glm::vec2(380.f, 80.f);
	addChild(m_pObstacle2);

	// add the Obstacle to the scene as a start point
	m_pObstacle3 = new Obstacle();
	m_pObstacle3->getTransform()->position = glm::vec2(380.f, 480.f);
	addChild(m_pObstacle3);

	m_pDestructibleObstacle1 = new DestructibleObstacle();
	m_pDestructibleObstacle1->getTransform()->position = glm::vec2(670.f, 280.f);
	addChild(m_pDestructibleObstacle1);

	m_pDestructibleObstacle2 = new DestructibleObstacle();
	m_pDestructibleObstacle2->getTransform()->position = glm::vec2(375.f, 200.f);
	addChild(m_pDestructibleObstacle2);

	m_pDestructibleObstacle3 = new DestructibleObstacle();
	m_pDestructibleObstacle3->getTransform()->position = glm::vec2(400.f, 400.f);
	addChild(m_pDestructibleObstacle3);

	// added the target to the scene a goal
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(700.f, 300.f);
	addChild(m_pTarget);

	// Set some data onto the ship's decision tree's map. I only have the player/target for now.
	m_pShip->getTree()->getMap().emplace("target", m_pTarget);
	m_pShip->getTree()->MakeDecision(); // Or we won't go into patrol.

	// Set some data onto the ship's decision tree's map. I only have the player/target for now.
	m_pCloseCombatEnemy->getTree()->getMap().emplace("target", m_pTarget);
	m_pCloseCombatEnemy->getTree()->MakeDecision(); // Or we won't go into patrol.

	// Set some data onto the ship's decision tree's map. I only have the player/target for now.
	m_pRangedEnemy->getTree()->getMap().emplace("target", m_pTarget);
	m_pRangedEnemy->getTree()->MakeDecision(); // Or we won't go into patrol.

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// TODO:
	// Toggle Visibility for the StarShip and the Target
		
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - M2021 - Lab 9", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static bool gridVisible = true;
	if (ImGui::Checkbox("Toggle Grid", &gridVisible))
	{
		m_toggleGrid(gridVisible);
	}

	ImGui::Separator();

	// allow ship rotation
	static int angle;
	angle = m_pShip->getCurrentHeading(); // New.
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pShip->setCurrentHeading(angle);
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pShip->getTransform()->position.x, m_pShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pShip->getTransform()->position.x = shipPosition[0];
		m_pShip->getTransform()->position.y = shipPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << m_pShip->getTree()->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];

		/*std::cout << "------------------------" << std::endl;
		std::cout << m_pShip->getTree()->MakeDecision() << std::endl;
		std::cout << "------------------------\n" << std::endl;*/
	}

	ImGui::Separator();
	
	ImGui::End();
}

void PlayScene::m_buildGrid()
{
	auto tileSize = Config::TILE_SIZE;

	// add path_nodes to the Grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2(
				(col * tileSize) + tileSize * 0.5f, (row * tileSize) + tileSize * 0.5f);
			addChild(path_node);
			m_pGrid.push_back(path_node);
		}
	}
}

void PlayScene::m_toggleGrid(bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

bool PlayScene::m_CheckAgentLOS(Agent* agent, DisplayObject* target)
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);
	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target );
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto object : getDisplayList())
		{
			if (object->getType() == GameObjectType::OBSTACLE) // Only putting obstacles in contactList now.
			{
				// check if object is farther than than the target
				auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, object);
				if (AgentToObjectDistance <= AgentToTargetDistance)
					contactList.push_back(object);
			}
		}
		auto agentTarget = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		auto hasLOS = CollisionManager::LOSCheck(agent, agentTarget, contactList, target);
		agent->setHasLOS(hasLOS);
	}
	return hasLOS;
}

void PlayScene::m_CheckPathNodeLOS(DisplayObject* target)
{
	for (auto path_node : m_pGrid)
	{
		auto targetDirection = target->getTransform()->position - path_node->getTransform()->position;
		auto normalizeDirection = Util::normalize(targetDirection);
		path_node->setCurrentDirection(normalizeDirection);
		m_CheckAgentLOS(path_node, target);
	}
}

PathNode* PlayScene::m_findClosestPathNode(Agent* agent, DisplayObject* target)
{
	auto min = FLT_MAX;
	PathNode* closestPathNode = nullptr;
	for (auto path_node : m_pGrid)
	{
		const auto distance = Util::distance(agent->getTransform()->position, path_node->getTransform()->position);
		if (distance < min && path_node->hasLOS() 
			&& (Util::distance(target->getTransform()->position, path_node->getTransform()->position) <= agent->getLOSDistance()))
		{
			min = distance;
			closestPathNode = path_node;
		}
	}
	closestPathNode->setLOSColour(glm::vec4(0.0f,0.0f,1.0f,1.0f));
	m_pShip->setClosestLOSNode(closestPathNode);
	m_pCloseCombatEnemy->setClosestLOSNode(closestPathNode);
	m_pRangedEnemy->setClosestLOSNode(closestPathNode);
	return closestPathNode;
}
