#pragma once
#include "structs.h"

class Spike
{
public:
	Spike(const Rectf& hitbox);
	bool CheckForHit(const Rectf& combatHitbox);

	void DrawHitbox() const;
private:
	Rectf m_Hitbox;
};

