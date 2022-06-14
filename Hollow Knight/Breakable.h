#pragma once
#include "pch.h"
#include "GameObjectStates.h"
class Breakable
{
public:
	Breakable(int maxHealth, const Rectf& hitbox);

	

private:
	int m_Health;
	Rectf m_Hitbox;
	BreakableState m_State;


	Point2f GetCenterPos() const;
	bool CheckForHit(const Rectf& hitbox);
};

