#include "Target.h"

#include "SoundManager.h"
#include "TextureManager.h"


Target::Target()
{
	TextureManager::Instance().loadSpriteSheet("../Assets/sprites/UFO.txt", "../Assets/sprites/UFO.png","UFO");
	setSpriteSheet(TextureManager::Instance().getSpriteSheet("UFO"));

	// set frame width
	setWidth(24);

	// set frame height
	setHeight(21);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);

	m_buildAnimations();

	m_health = 5;
	m_iframes = 0;
	m_attackCooldown = 0;

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	//TextureManager::Instance().playAnimation("UFO", getAnimation("idle"), x, y, 0.5f, 0, 255, true);
	switch (m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance().playAnimation("UFO", getAnimation("idle"),
			x, y, 0.5f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance().playAnimation("UFO", getAnimation("idle"),
			x, y, 0.5f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance().playAnimation("UFO", getAnimation("run"),
			x, y, 0.9f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance().playAnimation("UFO", getAnimation("run"),
			x, y, 0.9f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
}

void Target::update()
{
	/*m_move();
	m_checkBounds();*/
}

void Target::clean()
{
}

int Target::getHealth()
{
	return m_health;
}

void Target::setHealth(int amount)
{
	m_health -= amount;
}

int Target::getIFrames()
{
	return m_iframes;
}

void Target::setIFrames(int amount)
{
	m_iframes -= amount;
	if (m_iframes < 0)
	{
		m_iframes = 0;
	}
}

int Target::getAttackCooldown()
{
	return m_attackCooldown;
}

void Target::setAttackCooldown(int amount)
{
	m_attackCooldown -= amount;
}

PlayerAnimationState Target::getCurrentAnimationState()
{
	return m_currentAnimationState;
}

void Target::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}

void Target::m_checkBounds()
{
}

void Target::m_reset()
{
}

void Target::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-3"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-4"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-5"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-6"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-idle-7"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-4"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-5"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-6"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("UFO-run-7"));

	setAnimation(runAnimation);
}

SpriteSheet* Target::getSpriteSheet()
{
	return m_pSpriteSheet;
}

void Target::setAnimation(const Animation& animation)
{
	if (!m_animationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

void Target::setSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

Animation& Target::getAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

bool Target::m_animationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}

void Target::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}


