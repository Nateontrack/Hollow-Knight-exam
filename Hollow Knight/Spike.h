#pragma once
#include "structs.h"

class Spike
{
public:
	Spike(const Rectf& hitbox);
	bool CheckForHit(const Rectf& combatHitbox);
private:
	Rectf m_Hitbox;
};

