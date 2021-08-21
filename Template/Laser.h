#pragma once
#ifndef __LASER__
#define __LASER__
#include "DisplayObject.h"
#include "TextureManager.h"
#include "GLM/vec4.hpp"

class Laser : public DisplayObject
{
private:
	glm::vec2 trajectory;


public:
	void draw() override;
	void update() override;
	void clean() override;
	glm::vec2 getTrajectory();
	Laser();
	Laser(glm::vec2 traj);

};

#endif