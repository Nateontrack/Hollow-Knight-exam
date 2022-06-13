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

void Spike::DrawHitbox() const
{
	Color4f magenta{ 1,0,1,1 };
	SetColor(magenta);
	DrawRect(m_Hitbox);
}

