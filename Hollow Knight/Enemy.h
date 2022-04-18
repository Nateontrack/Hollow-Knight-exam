#pragma once
#include "Spritesheet.h"
#include "Vector2f.h"
#include "GameObjectStates.h"

class Enemy
{
public:
	Enemy(const Rectf& hitbox, const Rectf& boundaries);
	virtual ~Enemy() = default;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec) = 0;
	virtual void HandleCollision(const Rectf& actorHitbox) = 0;
	
	

protected:
	Vector2f m_Velocity;
	Rectf m_Hitbox;
	MoveDirection m_MoveDirState;
	Rectf m_Boundaries;

	virtual AnimationState CalculateAnimationState() const = 0;
	virtual void HandleState(float elapsedSec) = 0;
	
	bool Clamp();
	Point2f GetCenterPos() const;
private:
};

