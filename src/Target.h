#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "DisplayObject.h"
#include "Sprite.h"
#include "PlayerAnimationState.h"

class Target final : public DisplayObject {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	int getHealth();
	void setHealth(int amount);

	int getIFrames();
	void setIFrames(int amount);

	int getAttackCooldown();
	void setAttackCooldown(int amount);

	PlayerAnimationState getCurrentAnimationState();

	SpriteSheet* getSpriteSheet();
	void setAnimationState(PlayerAnimationState new_state);
	void setAnimation(const Animation& animation);
	void setSpriteSheet(SpriteSheet* sprite_sheet);
	Animation& getAnimation(const std::string& name);

private:
	void m_move();
	void m_checkBounds();
	void m_reset();
	bool m_animationExists(const std::string& id);

	void m_buildAnimations();

	SpriteSheet* m_pSpriteSheet;

	PlayerAnimationState m_currentAnimationState;

	std::unordered_map<std::string, Animation> m_pAnimations;

	int m_health;
	int m_iframes;
	int m_attackCooldown;
};


#endif /* defined (__TARGET__) */