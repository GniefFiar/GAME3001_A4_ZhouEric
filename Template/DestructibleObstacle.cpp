#include "DestructibleObstacle.h"

DestructibleObstacle::DestructibleObstacle()
{
	TextureManager::Instance().load("../Assets/sprites/DestructibleObstacle.png", "DO");

	auto size = TextureManager::Instance().getTextureSize("DO");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;
	health = 8;
}

void DestructibleObstacle::draw()
{
	TextureManager::Instance().draw("DO",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);

	if (health == 0)
	{
		SoundManager::Instance().allocateChannels(8);
		SoundManager::Instance().load("../Assets/audio/thunder.wav", "thunder", SOUND_SFX);
		SoundManager::Instance().playSound("thunder", 0);
		SoundManager::Instance().setSoundVolume(50);
		getTransform()->position = glm::vec2(-700.0f, -700.0f);
		glm::vec2 deadPosition = getTransform()->position;
		TextureManager::Instance().load("..Assets/sprites/Death Animation.png", "DA");
		TextureManager::Instance().draw("DA", deadPosition.x, deadPosition.y, 0, 255, true);
		health = -1;
	}
}

void DestructibleObstacle::update()
{
}

void DestructibleObstacle::clean()
{
}

int DestructibleObstacle::getHealth()
{
	return health;
}

void DestructibleObstacle::setHealth(int amount)
{
	health -= amount;
}
