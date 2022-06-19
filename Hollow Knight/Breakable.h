#pragma once
#include "pch.h"
#include "GameObjectStates.h"
#include "Spritesheet.h"
#include <vector>

class Breakable
{
public:
	Breakable(int maxHealth, const Rectf& hitbox, const std::string xmlPath, bool isSolid, float breakTime);

	void Draw() const;
	void Update(float elapsedSec);
	
	bool CheckForHit(const Rectf& hitbox);

	void HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec);

private:
	int m_Health;
	bool m_IsSolid;
	float m_CollisionRadius;
	float m_BreakTime;
	float m_AccumulatedTime;
	float m_CollisionOffset;
	Rectf m_Hitbox;
	std::vector<Point2f> m_Vertices;
	BreakableState m_State;
	Spritesheet m_Animations;

	std::string m_BreakSound;


	Point2f GetCenterPos() const;
	
	AnimationState CalculateAnimationState() const;
	void HandleState(float elapsedSec);
	void HandleHit();
};

