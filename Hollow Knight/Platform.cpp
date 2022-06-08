#include "Platform.h"
#include "utils.h"

using namespace utils;

Platform::Platform()
	:Platform(Point2f{0,0})
{}

Platform::Platform(const Point2f& bottomLeft)
	:
	m_pTexture{new Texture{"Resources/Sprites/platform01.png"} },
	m_CollisionOffset{0.1f}
{
	m_Shape.left = bottomLeft.x;
	m_Shape.bottom = bottomLeft.y;
	m_Shape.width = m_pTexture->GetWidth();
	m_Shape.height = m_pTexture->GetHeight();
	m_PlatformVertices = GetVertices(m_Shape);
}

Platform::~Platform()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Platform::Draw() const
{
	m_pTexture->Draw(m_Shape);
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	std::vector<Point2f> actorVertices{ GetVertices(actorShape) };
	Point2f bottomCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom };
	Vector2f bottomOffset{ 0, actorShape.height / 2 }, topOffset{ 0, -actorShape.height / 2 },
		leftOffset{ actorShape.width / 2,0 }, rightOffset{ -actorShape.width / 2,0 };
	HitInfo info{};
	
	
	//bottom
	if (Raycast(m_PlatformVertices, bottomCenter, Point2f{ bottomCenter + bottomOffset }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_PlatformVertices, actorVertices[1], Point2f{ actorVertices[1] + topOffset }, info))
	{
		actorVelocity.y = 0;
		actorShape.bottom = info.intersectPoint.y - actorShape.height - m_CollisionOffset;
	}

	actorVertices = GetVertices(actorShape);

	//left
	if (Raycast(m_PlatformVertices, actorVertices[0], Point2f{ actorVertices[0] + leftOffset }, info) ||
		Raycast(m_PlatformVertices, actorVertices[1], Point2f{ actorVertices[1] + leftOffset }, info))
	{
		actorVelocity.x = 0;
		actorShape.left = info.intersectPoint.x + m_CollisionOffset;

	}

	//right
	else if (Raycast(m_PlatformVertices, actorVertices[2], Point2f{ actorVertices[2] + rightOffset }, info) ||
		Raycast(m_PlatformVertices, actorVertices[3], Point2f{ actorVertices[3] + rightOffset }, info))
	{
		actorVelocity.x = 0;
		actorShape.left = info.intersectPoint.x - actorShape.width - m_CollisionOffset;
	}
}

bool Platform::IsOnGround(const Rectf& actorShape) const
{
	Point2f bottomCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom };

	Vector2f offset{ 0,-1 };
	HitInfo info{};

	if (Raycast(m_PlatformVertices, bottomCenter, Point2f{ bottomCenter + offset }, info))
	{
		return true;
	}
	return false;
}