#pragma once
#ifndef __ICEATTACK__
#define __ICEATTACK__
#include "DisplayObject.h"
#include "TextureManager.h"

class IceAttack : public DisplayObject
{
private:
	int timer;

public:
	void draw() override;
	void update() override;
	void clean() override;
	int getTimer();
	IceAttack();

};

#endif
