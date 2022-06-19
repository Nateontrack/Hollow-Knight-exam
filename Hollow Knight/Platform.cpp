#include "Platform.h"
#include "utils.h"
#include "TextureManager.h"


using namespace utils;

Platform::Platform()
	:Platform(Rectf{}, PlatformType::platform01)
{}

Platform::Platform(const Rectf& hitbox, PlatformType type)
	:
	m_CollisionOffset{0.1f},
	m_Shape{hitbox},
	m_Type{type},
	m_CollisionRadius{300}
{
	InitializeTexture();
	m_PlatformVertices = GetCollisionVertices(m_Shape);
}

void Platform::Draw() const
{
	Point2f centerPos{ GetCenterPos() };
	Point2f drawPos{ centerPos.x - m_pTexture->GetWidth() / 2, centerPos.y - m_pTexture->GetHeight() / 2 };
	m_pTexture->Draw(drawPos);
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float elapsedSec)
{
	if (GetDistance(actorShape.left, actorShape.bottom, m_Shape.left, m_Shape.bottom) > m_CollisionRadius)
	{
		return;
	}
	//vertices are ordered clockwise starting bottom left
	std::vector<Point2f> actorVertices{ GetCollisionVertices(actorShape) };
	Vector2f futureTranslateX{ actorVelocity.x * elapsedSec, 0 }, futureTranslateY{0, actorVelocity.y * elapsedSec};
	HitInfo info{};

	//left
	if (Raycast(m_PlatformVertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateX }, info) ||
		Raycast(m_PlatformVertices, actorVertices[1], Point2f{ actorVertices[1] + futureTranslateX }, info) ||
		Raycast(m_PlatformVertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorShape.left = info.intersectPoint.x + m_CollisionOffset;
	}

	//right
	else if (Raycast(m_PlatformVertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateX }, info) ||
		Raycast(m_PlatformVertices, actorVertices[4], Point2f{ actorVertices[4] + futureTranslateX }, info) ||
			Raycast(m_PlatformVertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorShape.left = info.intersectPoint.x - actorShape.width - m_CollisionOffset;
	}

	//actorVertices = GetCollisionVertices(actorShape);

	//bottom
	else if (Raycast(m_PlatformVertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_PlatformVertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_PlatformVertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_PlatformVertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y - actorShape.height - m_CollisionOffset;
	}

	actorVertices = GetCollisionVertices(actorShape);

	//bottom
	if (Raycast(m_PlatformVertices, actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_PlatformVertices, actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_PlatformVertices, actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_PlatformVertices, actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y - actorShape.height - m_CollisionOffset;
	}
}

bool Platform::IsOnGround(const Rectf& actorShape) const
{
	std::vector<Point2f> actorVertices{ GetCollisionVertices(actorShape) };

	Vector2f offset{ 0,-1 };
	HitInfo info{};

	if (Raycast(m_PlatformVertices, actorVertices[0], Point2f{ actorVertices[0] + offset }, info) ||
		Raycast(m_PlatformVertices, actorVertices[5], Point2f{ actorVertices[5] + offset }, info))
	{
		return true;
	}
	return false;
}

void Platform::InitializeTexture()
{
	std::string texturePath{ "Resources/Sprites/plat_float_0" };
	texturePath = texturePath + std::to_string(static_cast<int>(m_Type)) + ".png";
	m_pTexture = TextureManager::GetInstance()->GetTexture(texturePath);
}

Point2f Platform::GetCenterPos() const
{
	Point2f center{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };
	return center;
}