#pragma once
#ifndef __SHIP__
#define __SHIP__

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

class Ship : public Agent
{
public:
	Ship();
	~Ship();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();

	void move();
	void rotate();
	void rotateToTarget(DisplayObject* target);
	void closeCombatAttack();
	void rangedAttack();
	IceAttack* iceAttack;
	std::vector<EnemyBullet*> enemyBullets;
	// getters
	float getMaxSpeed() const;
	DisplayObject* getTarget();
	std::vector<DisplayObject*>& getPatrol();
	DecisionTree* getTree();
	DisplayObject* getCurrentWaypoint();
	DisplayObject* getNextWaypoint();
	PathNode* getClosestLOSNode();
	IceAttack* getIceAttack();
	int getHealth();

	// setters
	void setMaxSpeed(float newSpeed);
	void setTarget(DisplayObject* target);
	void setClosestLOSNode(PathNode* node);
	void resumePatrol();
	void setHealth(int amount);

	ActionState m_state;
	int attackTimer;
	int rangedTimer;
	int health;

	void m_checkBounds();
	void m_reset();

	float m_maxSpeed;
	float m_turnRate;

	// Decision Tree
	DecisionTree* m_pTree;

	std::vector<DisplayObject*> m_patrol;
	int m_waypoint;
	DisplayObject* m_pTarget;
	PathNode* m_pLOSNode;

	int m_ctr;

private:
	
};


#endif /* defined (__SHIP__) */

