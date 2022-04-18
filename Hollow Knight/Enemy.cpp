#include "Enemy.h"

Enemy::Enemy(const Rectf& hitbox, const Rectf& boundaries)
	:m_Hitbox{hitbox},
	m_Boundaries{boundaries},
	m_Velocity{},
	m_MoveDirState{MoveDirection::right}
{}

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