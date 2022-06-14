#include "Breakable.h"
#include "utils.h"

using namespace utils;

Breakable::Breakable(int maxHealth, const Rectf& hitbox)
	:m_Health{maxHealth},
	m_Hitbox{hitbox},
	m_State{BreakableState::whole}
{

}

bool Breakable::CheckForHit(const Rectf& hitbox)
{
	return IsOverlapping(hitbox, m_Hitbox);
}