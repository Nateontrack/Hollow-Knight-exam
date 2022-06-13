#include "Enemy.h"
#include "utils.h"

using namespace utils;

Enemy::Enemy(const Rectf& hitbox, const Rectf& boundaries, int health)
	:m_Hitbox{hitbox},
	m_Health{health},
	m_Boundaries{boundaries},
	m_Velocity{},
	m_MoveDirState{MoveDirection::right},
	m_pHitAnimation{ new Spritesheet{"Resources/XML/HitAnimation.xml", "Resources/Sprites/HitAnimation.png"} },
	m_HitTime{0.15f},
	m_IsHit{false},
	m_AccumulatedHitTime{0},
	m_IsDead{false}
{}

Enemy::~Enemy()
{
	delete m_pHitAnimation;
	m_pHitAnimation = nullptr;
}

Point2f Enemy::GetCenterPos() const
{
	Point2f centerPos{ m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2 };
	return centerPos;
}

bool Enemy::Clamp()
{
	bool hasClamped{false};
	Point2f bottomLeft{ m_Hitbox.left , m_Hitbox.bottom},
		topRight{ m_Hitbox.left + m_Hitbox.width, m_Hitbox.bottom + m_Hitbox.height };


	if (bottomLeft.x < m_Boundaries.left)
	{
		bottomLeft.x = m_Boundaries.left;
		hasClamped = true;
	}

	else if (bottomLeft.y < m_Boundaries.bottom)
	{
		bottomLeft.y = m_Boundaries.bottom;
		hasClamped = true;
	}

	if (topRight.x > m_Boundaries.left + m_Boundaries.width)
	{
		bottomLeft.x -= topRight.x - (m_Boundaries.left + m_Boundaries.width);
		hasClamped = true;
	}

	else if (topRight.y > m_Boundaries.bottom + m_Boundaries.height)
	{
		bottomLeft.y -= topRight.y - (m_Boundaries.bottom + m_Boundaries.height);
		hasClamped = true;
	}

	return hasClamped;
}

bool Enemy::CheckForHit(const Rectf& hitbox)
{
	return IsOverlapping(hitbox, m_Hitbox);
}

bool Enemy::GetIsDead()
{
	return m_IsDead;
}