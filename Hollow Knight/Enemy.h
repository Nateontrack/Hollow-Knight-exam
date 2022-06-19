#pragma once
#include "Spritesheet.h"
#include "Vector2f.h"
#include "GameObjectStates.h"

class Enemy
{
public:
	Enemy(const Rectf& hitbox, const Rectf& boundaries, int health);
	virtual ~Enemy() = default;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const Point2f& playerPos) = 0;
	/*virtual bool HandleCollision(const Rectf& actorHitbox) = 0;*/
	
	bool CheckForHit(const Rectf& hitbox) const;
	bool GetIsDead() const;
	virtual void HitEnemy() = 0;

protected:
	Vector2f m_Velocity;
	Rectf m_Hitbox;
	MoveDirection m_MoveDirState;
	Rectf m_Boundaries;
	Spritesheet m_HitAnimation;
	int m_Health;
	float m_HitTime;
	float m_AccumulatedHitTime;
	bool m_IsHit;
	bool m_IsDead;

	virtual AnimationState CalculateAnimationState() const = 0;
	virtual void HandleState(float elapsedSec) = 0;
	
	
	bool Clamp();
	Point2f GetCenterPos() const;
private:
};

