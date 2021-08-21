#pragma once
#ifndef __ENEMYBULLET__
#define __ENEMYBULLET__
#include "DisplayObject.h"
#include "TextureManager.h"
#include "GLM/vec4.hpp"

class EnemyBullet : public DisplayObject
{
private:
	glm::vec2 trajectory;


public:
	void draw() override;
	void update() override;
	void clean() override;
	int getTimer();
	glm::vec2 getTrajectory();
	EnemyBullet();
	EnemyBullet(glm::vec2 traj);

};

#endif