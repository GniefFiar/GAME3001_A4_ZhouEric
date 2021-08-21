#include "EnemyBullet.h"

void EnemyBullet::draw()
{
}

void EnemyBullet::update()
{
}

void EnemyBullet::clean()
{
}

glm::vec2 EnemyBullet::getTrajectory()
{
	return trajectory;
}

EnemyBullet::EnemyBullet()
{
	TextureManager::Instance().load("../Assets/sprites/Bullet.png", "enemyBullet");

	auto size = TextureManager::Instance().getTextureSize("enemyBullet");
	setWidth(size.x);
	setHeight(size.y);
}

EnemyBullet::EnemyBullet(glm::vec2 traj)
{
	trajectory = traj;
}
