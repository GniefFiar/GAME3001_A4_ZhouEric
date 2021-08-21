#include "IceAttack.h"

IceAttack::IceAttack()
{
	TextureManager::Instance().load("../Assets/sprites/Close Combat Attack.png", "ice");

	auto size = TextureManager::Instance().getTextureSize("ice");
	setWidth(size.x);
	setHeight(size.y);
	timer = 30;
}

void IceAttack::draw()
{

}

void IceAttack::update()
{
	timer--;
}

void IceAttack::clean()
{

}

int IceAttack::getTimer()
{
	return timer;
}
