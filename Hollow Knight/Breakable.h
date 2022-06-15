#pragma once
#include "pch.h"
#include "GameObjectStates.h"
#include "Spritesheet.h"

class Breakable
{
public:
	Breakable(int maxHealth, const Rectf& hitbox, const std::string xmlPath, bool isSolid, float breakTime);

	void Draw() const;
	void Update(float elapsedSec);
	
	bool CheckForHit(const Rectf& hitbox);

private:
	int m_Health;
	bool m_IsSolid;
	float m_BreakTime;
	float m_AccumulatedTime;
	Rectf m_Hitbox;
	BreakableState m_State;
	Spritesheet m_Animations;


	Point2f GetCenterPos() const;
	
	AnimationState CalculateAnimationState() const;
	void HandleState(float elapsedSec);
	void HandleHit();
};

