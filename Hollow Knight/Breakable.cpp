#include "Breakable.h"
#include "utils.h"
#include "SoundUtils.h"
#include <iostream>

using namespace soundUtils;
using namespace utils;

Breakable::Breakable(int maxHealth, const Rectf& hitbox, const std::string xmlPath, bool isSolid, float breakTime)
	:m_Health{maxHealth},
	m_Hitbox{hitbox},
	m_State{BreakableState::whole},
	m_Animations{ Spritesheet{xmlPath, "Resources/Sprites/breakables.png"} },
	m_IsSolid{isSolid},
	m_BreakTime{breakTime},
	m_AccumulatedTime{0},
	m_CollisionOffset{0.01f},
	m_CollisionRadius{300},
	m_BreakSound{"Resources/Sounds/BreakableHit.wav"}
{
	if (m_IsSolid)
	{
		m_Vertices = GetCollisionVertices(m_Hitbox);
	}
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
		m_IsSolid = false;
		PlaySoundEffect(m_BreakSound, 1);
	}
}

void Breakable::HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec)
{
	//only if solid and close enough
	if (!m_IsSolid)
	{
		return;
	}
	if (GetDistance(actorHitbox.left, actorHitbox.bottom, m_Hitbox.left, m_Hitbox.bottom) > m_CollisionRadius)
	{
		return;
	}

	//vertices are ordered clockwise starting bottom left
	//does vertical twice to make sure nothing gets missed by first casts (hypothenuse of vector)
	std::vector<Point2f> actorVertices{ GetCollisionVertices(actorHitbox) };
	Vector2f futureTranslateX{ actorVelocity.x * elapsedSec, 0 }, futureTranslateY{ 0, actorVelocity.y * elapsedSec };
	HitInfo info{};

	//left
	if (Raycast(m_Vertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateX }, info) ||
		Raycast(m_Vertices, actorVertices[1], Point2f{ actorVertices[1] + futureTranslateX }, info) ||
		Raycast(m_Vertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x + m_CollisionOffset;
	}

	//right
	else if (Raycast(m_Vertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateX }, info) ||
		Raycast(m_Vertices, actorVertices[4], Point2f{ actorVertices[4] + futureTranslateX }, info) ||
		Raycast(m_Vertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x - actorHitbox.width - m_CollisionOffset;
	}

	

	//bottom
	else if (Raycast(m_Vertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_Vertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_Vertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_Vertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y - actorHitbox.height - m_CollisionOffset;
	}

	actorVertices = GetCollisionVertices(actorHitbox);

	//bottom
	if (Raycast(m_Vertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_Vertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_Vertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_Vertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y - actorHitbox.height - m_CollisionOffset;
	}
}