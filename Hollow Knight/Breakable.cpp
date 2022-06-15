#include "Breakable.h"
#include "utils.h"
#include <iostream>

using namespace utils;

Breakable::Breakable(int maxHealth, const Rectf& hitbox, const std::string xmlPath, bool isSolid, float breakTime)
	:m_Health{maxHealth},
	m_Hitbox{hitbox},
	m_State{BreakableState::whole},
	m_Animations{ Spritesheet{xmlPath, "Resources/Sprites/breakables.png"} },
	m_IsSolid{isSolid},
	m_BreakTime{breakTime},
	m_AccumulatedTime{0}
{
}

bool Breakable::CheckForHit(const Rectf& hitbox)
{
	if (IsOverlapping(hitbox, m_Hitbox))
	{
		HandleHit();
		return true;
	}
	else
	{
		return false;
	}
}

Point2f Breakable::GetCenterPos() const
{
	Point2f centerPos{};
	centerPos.x = m_Hitbox.left + m_Hitbox.width / 2;
	centerPos.y = m_Hitbox.bottom + m_Hitbox.height / 2;
	return centerPos;
}

void Breakable::Update(float elapsedSec)
{
	m_Animations.Update(CalculateAnimationState(), elapsedSec);
	HandleState(elapsedSec);
}

void Breakable::Draw() const
{
	Point2f centerPos = GetCenterPos();
	m_Animations.Draw(CalculateAnimationState(), centerPos);
}

AnimationState Breakable::CalculateAnimationState() const
{
	switch (m_State)
	{
	case BreakableState::whole:
		return AnimationState::still;
		break;
	case BreakableState::breaking:
		return AnimationState::hit;
		break;
	case BreakableState::broken:
		return AnimationState::deathGround;
		break;
	default:
		return AnimationState::still;
		break;
	}
}

void Breakable::HandleState(float elapsedSec)
{
	switch (m_State)
	{
	case BreakableState::whole:
		break;
	case BreakableState::breaking:
		m_AccumulatedTime += elapsedSec;
		if (m_AccumulatedTime > m_BreakTime)
		{
			m_AccumulatedTime = 0;
			m_State = BreakableState::broken;
		}
		break;
	case BreakableState::broken:
		break;
	}
}

void Breakable::HandleHit()
{
	--m_Health;
	if (m_Health == 0)
	{
		m_State = BreakableState::breaking;
	}
}