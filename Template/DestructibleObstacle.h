#pragma once
#ifndef __DESTRUCTIBLEOBJECT__
#define __DESTRUCTIBLEOBJECT__
#include "DisplayObject.h"
#include "SoundManager.h"
#include "TextureManager.h"

class DestructibleObstacle : public DisplayObject
{
public:
	// constructors
	DestructibleObstacle();

	int health;

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	int getHealth();
	void setHealth(int amount);
private:

};

#endif /* defined (__OBSTACLE__) */
