#include "Level.h"
#include "utils.h"

using namespace utils;

Level::Level()
	:m_pBackgroundTexture{new Texture{"Resources/Sprites/background.png"}},
	m_pForegroundTexture{ new Texture{"Resources/Sprites/foreground.png"} }
{
	InitLevelVerts();
}

Level::~Level()
{
	CleanUp();
}

void Level::InitLevelVerts()
{
	m_Vertices.push_back(Point2f{ 0,0 });
	m_Vertices.push_back(Point2f{ 0,720 });
	m_Vertices.push_back(Point2f{ 44,720 });
	m_Vertices.push_back(Point2f{ 44,76 });
	m_Vertices.push_back(Point2f{ 1236,76 });
	m_Vertices.push_back(Point2f{ 1236,720 });
	m_Vertices.push_back(Point2f{ 1280,720 });
	m_Vertices.push_back(Point2f{ 1280,0 });
}

void Level::DrawBackground() const
{
	m_pBackgroundTexture->Draw(Point2f{ 0,0 });
}

void Level::DrawForeground() const
{
	m_pForegroundTexture->Draw(Point2f{0,0});
}

void Level::HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity)
{
	Point2f middleBottom{ actorHitbox.left + actorHitbox.width / 2, actorHitbox.bottom },
		middleTop{ actorHitbox.left + actorHitbox.width / 2 , actorHitbox.bottom + actorHitbox.height};
	HitInfo info{};
		
	if (Raycast(m_Vertices, middleTop, middleBottom, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y;
	}
}

bool Level::IsOnGround(const Rectf& actorHitbox) const
{
	Point2f middleBottom{ actorHitbox.left + actorHitbox.width / 2, actorHitbox.bottom },
		underActor{ middleBottom.x, middleBottom.y - 1 };
	HitInfo info{};
	if (Raycast(m_Vertices, middleBottom, underActor, info))
	{
		return true;
	}
	return false;
}

void Level::CleanUp()
{
	delete m_pBackgroundTexture;
	delete m_pForegroundTexture;
	m_pBackgroundTexture = nullptr;
	m_pForegroundTexture = nullptr;
}