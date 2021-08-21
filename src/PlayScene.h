#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Obstacle.h"
#include "ship.h"
#include "PathNode.h"
#include "Target.h"
#include "Player.h"
#include "PlayerAnimationState.h"
#include "Laser.h"
#include "../Template/CloseCombatEnemy.h"
#include "../Template/RangedEnemy.h"
#include "../Template/Laser.h"
#include "../Template/DestructibleObstacle.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	//void moveStarShip() const;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects
	//Player* m_pDragoon;
	Target* m_pTarget;
	Ship* m_pShip;
	CloseCombatEnemy* m_pCloseCombatEnemy;
	RangedEnemy* m_pRangedEnemy;
	Obstacle* m_pObstacle1;
	Obstacle* m_pObstacle2;
	Obstacle* m_pObstacle3;
	DestructibleObstacle* m_pDestructibleObstacle1;
	DestructibleObstacle* m_pDestructibleObstacle2;
	DestructibleObstacle* m_pDestructibleObstacle3;
	std::vector<Laser*> m_lasers;

	// Other
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	bool m_CheckAgentLOS(Agent* agent, DisplayObject* target);
	void m_CheckPathNodeLOS(DisplayObject* target);
	PathNode* m_findClosestPathNode(Agent* agent, DisplayObject* target);
	void m_toggleGrid(bool state);
	int killCount = 0;
};

#endif /* defined (__PLAY_SCENE__) */