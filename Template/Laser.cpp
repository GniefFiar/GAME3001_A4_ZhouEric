#include "Laser.h"

void Laser::draw()
{
}

void Laser::update()
{
}

void Laser::clean()
{
}

glm::vec2 Laser::getTrajectory()
{
	return trajectory;
}

Laser::Laser()
{
	TextureManager::Instance().load("../Assets/sprites/Laser.png", "Laser");

	auto size = TextureManager::Instance().getTextureSize("Laser");
	setWidth(size.x);
	setHeight(size.y);
}

Laser::Laser(glm::vec2 traj)
{
	TextureManager::Instance().load("../Assets/sprites/Laser.png", "Laser");

	auto size = TextureManager::Instance().getTextureSize("Laser");
	setWidth(size.x);
	setHeight(size.y);
	trajectory = traj;
}
