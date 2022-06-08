#include "Level.h"
#include "utils.h"
#include "SVGParser.h"
#include <iostream>

using namespace utils;

Level::Level()
	:m_pBackgroundTexture{new Texture{"Resources/Sprites/background.png"}},
	m_pForegroundTexture{new Texture{"Resources/Sprites/foreground.png"}},
	m_CollisionOffset{0.01f}
{
	InitLevelVerts();
	InitLevelBoundaries();
	InitPlatforms();
	InitEnemies();
}

Level::~Level()
{
	CleanUp();
}

void Level::InitLevelVerts()
{
	//m_Vertices.push_back(std::vector<Point2f> {});
	if (!SVGParser::GetVerticesFromSvgFile("Resources/XML/LevelVertices.svg", m_Vertices))
	{
		std::cout << "Level vertices loading failed\n";
	}
	int i{};
	for (Point2f it : m_Vertices[0])
	{
		std::cout << "Point " << i << " : " << it.x << ", " << it.y << '\n';
		i++;
	}
}

void Level::InitLevelBoundaries()
{
	m_Boundaries.left = 0;
	m_Boundaries.bottom = 0;
	m_Boundaries.width = m_pBackgroundTexture->GetWidth();
	m_Boundaries.height = m_pBackgroundTexture->GetHeight();
}

void Level::InitPlatforms()
{
	m_pPlatforms.push_back(new Platform{ Point2f{669, 300} });
	m_pPlatforms.push_back(new Platform{ Point2f{970, 430} });
	m_pPlatforms.push_back(new Platform{ Point2f{2003,789} });
	m_pPlatforms.push_back(new Platform{ Point2f{2276,891} });
}

void Level::InitEnemies()
{
	Rectf boundaries{1210,591,500,200};
	Rectf hitbox{ 1210,591,110,80 };
	m_pEnemies.push_back(new Crawlid{ hitbox, boundaries, "Resources/XML/CrawlidAnimations.xml", "Resources/Sprites/Crawlidsheet.png" });
}

void Level::DrawBackground() const
{
	m_pBackgroundTexture->Draw(Point2f{ 0,0 });
}

void Level::DrawForeground() const
{
	m_pForegroundTexture->Draw(Point2f{0,0});
	DrawEnemies();
	DrawPlatforms();
}

void Level::HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity)
{
	HandleCollisionLevel(actorHitbox, actorVelocity);
	HandleCollisionPlatforms(actorHitbox, actorVelocity);
	for (Enemy* it : m_pEnemies)
	{
		it->HandleCollision(actorHitbox);
	}
}

void Level::HandleCollisionLevel(Rectf& actorHitbox, Vector2f& actorVelocity)
{
	std::vector<Point2f> actorVertices{ GetVertices(actorHitbox) };

	Vector2f bottomOffset{ 0, actorHitbox.height / 2 }, topOffset{ 0, -actorHitbox.height / 2 },
		leftOffset{ actorHitbox.width / 2,0 }, rightOffset{ -actorHitbox.width / 2,0 };
	HitInfo info{};


	//bottom
	if (Raycast(m_Vertices[0], actorVertices[0], Point2f{actorVertices[0] + bottomOffset}, info) ||
		Raycast(m_Vertices[0], actorVertices[3], Point2f{ actorVertices[3] + bottomOffset }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y + m_CollisionOffset;
	}
	
	//top
	if (Raycast(m_Vertices[0], actorVertices[1], Point2f{ actorVertices[1] + topOffset }, info) ||
		Raycast(m_Vertices[0], actorVertices[2], Point2f{ actorVertices[2] + topOffset }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y - actorHitbox.height - m_CollisionOffset;
	}

	//left
	if (Raycast(m_Vertices[0], actorVertices[0], Point2f{ actorVertices[0] + leftOffset }, info) ||
		Raycast(m_Vertices[0], actorVertices[1], Point2f{ actorVertices[1] + leftOffset }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x + m_CollisionOffset;
	}

	//right
	if (Raycast(m_Vertices[0], actorVertices[2], Point2f{ actorVertices[2] + rightOffset }, info) ||
		Raycast(m_Vertices[0], actorVertices[3], Point2f{ actorVertices[3] + rightOffset }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x - actorHitbox.width - m_CollisionOffset;
	}
}

void Level::HandleCollisionPlatforms(Rectf& actorHitbox, Vector2f& actorVelocity)
{
	for (Platform* it : m_pPlatforms)
	{
		it->HandleCollision(actorHitbox, actorVelocity);
	}
}

bool Level::IsOnGround(const Rectf& actorHitbox) const
{
	if (IsOnGroundLevel(actorHitbox) || IsOnGroundPlatforms(actorHitbox))
	{
			return true;
	}
	return false;
}

bool Level::IsOnGroundLevel(const Rectf& actorHitbox) const
{
	Point2f middleBottom{ actorHitbox.left + actorHitbox.width / 2, actorHitbox.bottom },
		underActor{ middleBottom.x, middleBottom.y - 1 };
	HitInfo info{};
	if (Raycast(m_Vertices[0], middleBottom, underActor, info))
	{
		return true;
	}
	return false;
}

bool Level::IsOnGroundPlatforms(const Rectf& actorHitbox) const
{
	for (Platform* it : m_pPlatforms)
	{
		if (it->IsOnGround(actorHitbox))
		{
			return true;
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::CleanUp()
{
	delete m_pBackgroundTexture;
	delete m_pForegroundTexture;
	m_pBackgroundTexture = nullptr;
	m_pForegroundTexture = nullptr;

	for (Platform* it : m_pPlatforms)
	{
		delete it;
		it = nullptr;
	}
	m_pPlatforms.clear();

	for (Enemy* it : m_pEnemies)
	{
		delete it;
		it = nullptr;
	}
	m_pEnemies.clear();
}

void Level::DrawPlatforms() const
{
	for (Platform* it : m_pPlatforms)
	{
		it->Draw();
	}
}

void Level::UpdateEnemies(float elapsedSec)
{
	for (Enemy* it : m_pEnemies)
	{
		it->Update(elapsedSec);
	}
}

void Level::DrawEnemies() const
{
	for (Enemy* it : m_pEnemies)
	{
		it->Draw();
	}
}

void Level::Update(float elapsedSec)
{
	UpdateEnemies(elapsedSec);
}