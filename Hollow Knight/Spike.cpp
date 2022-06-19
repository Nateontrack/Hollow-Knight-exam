#include "Spike.h"
#include "utils.h"

using namespace utils;

Spike::Spike(const Rectf& hitbox)
	:m_Hitbox{hitbox}
{}

bool Spike::CheckForHit(const Rectf& combatHitbox)
{
	return IsOverlapping(m_Hitbox, combatHitbox);
}

