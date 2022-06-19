#include "Level.h"
#include "utils.h"
#include "SVGParser.h"
#include "SoundUtils.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace utils;
using namespace soundUtils;

Level::Level(const std::string& filePath, const Point2f& startPos)
	:m_CollisionOffset{0.01f},
	m_RespawnPos{startPos},
	m_AmbientSound{"Resources/Sounds/Dirtmouth 1.wav"}
{
	PlaySoundStream(m_AmbientSound);
	InitLevelVerts();
	InitLevelBoundaries();
	LoadGameObjectDataFromFile(filePath);
}

Level::~Level()
{

	CleanUp();
}

void Level::InitLevelVerts()
{
	if (!SVGParser::GetVerticesFromSvgFile("Resources/XML/LevelVertices.svg", m_Vertices))
	{
		std::cout << "Level vertices loading failed\n";
	}
}

void Level::InitLevelBoundaries()
{
	m_Boundaries.left = 0;
	m_Boundaries.bottom = 0;
	m_Boundaries.width = 10850;
	m_Boundaries.height = 4700;
}

void Level::DrawEntities() const
{
	DrawEnemies();
	DrawBreakables();
	DrawPowerUps();
}

void Level::DrawForeground() const
{
	DrawPlatforms();
}

void Level::HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec)
{
	HandleCollisionLevel(actorHitbox, actorVelocity, elapsedSec);
	HandleCollisionPlatforms(actorHitbox, actorVelocity, elapsedSec);
	HandleCollisionBreakables(actorHitbox, actorVelocity, elapsedSec);
}

bool Level::CheckForHitEnemies(const Rectf& actorHitbox) const
{
	for (Enemy* it : m_pEnemies)
	{
		if (!it->GetIsDead())
		{
			if (it->CheckForHit(actorHitbox)) return true;
		}
	}
	return false;
}

bool Level::CheckForHitPowerUp(const Rectf& actorHitbox)
{
	for (PowerUp* it : m_pPowerUps)
	{
		if (it->CheckForHit(actorHitbox))
		{
			delete it;
			it = nullptr;
			m_pPowerUps.clear();
			return true;
		}
	}
	return false;
}

void Level::HandleCollisionLevel(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec)
{
	//vertices are ordered clockwise starting bottom left
	std::vector<Point2f> actorVertices{ GetCollisionVertices(actorHitbox) };
	Vector2f futureTranslateX{ actorVelocity.x * elapsedSec, 0 }, futureTranslateY{ 0, actorVelocity.y * elapsedSec };
	HitInfo info{};

	//left
	if (Raycast(m_Vertices[0], actorVertices[0], Point2f{ actorVertices[0] + futureTranslateX }, info) ||
		Raycast(m_Vertices[0], actorVertices[2], Point2f{ actorVertices[2] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x + m_CollisionOffset;
	}

	//right
	else if (Raycast(m_Vertices[0], actorVertices[3], Point2f{ actorVertices[3] + futureTranslateX }, info) ||
		Raycast(m_Vertices[0], actorVertices[5], Point2f{ actorVertices[5] + futureTranslateX }, info))
	{
		actorVelocity.x = 0;
		actorHitbox.left = info.intersectPoint.x - actorHitbox.width - m_CollisionOffset;
	}

	//bottom
	else if (Raycast(m_Vertices[0], actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_Vertices[0], actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_Vertices[0], actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_Vertices[0], actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y - actorHitbox.height - m_CollisionOffset;
	}

	actorVertices = GetCollisionVertices(actorHitbox);

	//bottom
	if (Raycast(m_Vertices[0], actorVertices[0], Point2f{ actorVertices[0] + futureTranslateY }, info) ||
		Raycast(m_Vertices[0], actorVertices[5], Point2f{ actorVertices[5] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y + m_CollisionOffset;
	}

	//top
	else if (Raycast(m_Vertices[0], actorVertices[2], Point2f{ actorVertices[2] + futureTranslateY }, info) ||
		Raycast(m_Vertices[0], actorVertices[3], Point2f{ actorVertices[3] + futureTranslateY }, info))
	{
		actorVelocity.y = 0;
		actorHitbox.bottom = info.intersectPoint.y - actorHitbox.height - m_CollisionOffset;
	}
}

void Level::HandleCollisionPlatforms(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec)
{
	for (Platform* it : m_pPlatforms)
	{
		it->HandleCollision(actorHitbox, actorVelocity, elapsedSec);
	}
}

void Level::HandleCollisionBreakables(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec)
{
	for (Breakable* it : m_pBreakables)
	{
		it->HandleCollision(actorHitbox, actorVelocity, elapsedSec);
	}
}

bool Level::CheckForHitSpikes(const Rectf& actorHitbox) const
{
	for (Spike* it : m_pSpikes)
	{
		if (it->CheckForHit(actorHitbox))
		{
			return true;
		}
	}
	return false;
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
	std::vector<Point2f> actorVertices{ GetCollisionVertices(actorHitbox) };

	Vector2f offset{ 0,-1 };
	HitInfo info{};

	if (Raycast(m_Vertices[0], actorVertices[0], Point2f{actorVertices[0] + offset}, info) ||
		Raycast(m_Vertices[0], actorVertices[5], Point2f{actorVertices[5] + offset}, info))
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

	for (Spike* it : m_pSpikes)
	{
		delete it;
		it = nullptr;
	}
	m_pSpikes.clear();

	for (Breakable* it : m_pBreakables)
	{
		delete it;
		it = nullptr;
	}
	m_pBreakables.clear();

	for (PowerUp* it : m_pPowerUps)
	{
		delete it;
		it = nullptr;
	}
	m_pPowerUps.clear();
}

void Level::DrawPlatforms() const
{
	for (Platform* it : m_pPlatforms)
	{
		it->Draw();
	}
}

void Level::UpdateEnemies(float elapsedSec, const Point2f& playerPos)
{
	for (Enemy* it : m_pEnemies)
	{
		it->Update(elapsedSec, playerPos);
	}
}

void Level::DrawEnemies() const
{
	for (Enemy* it : m_pEnemies)
	{
		it->Draw();
	}
}

void Level::Update(float elapsedSec, const Point2f& playerPos)
{
	UpdateEnemies(elapsedSec, playerPos);
	UpdateBreakables(elapsedSec);
}

void Level::HandleAttack(const Rectf& attackRect)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		if (pEnemy->CheckForHit(attackRect))
		{
			pEnemy->HitEnemy();
		}
	}
	for (Breakable* pBreakable : m_pBreakables)
	{
		pBreakable->CheckForHit(attackRect);
	}
}

void Level::DrawBreakables() const
{
	for (Breakable* pBreakable : m_pBreakables)
	{
		pBreakable->Draw();
	}
}

void Level::UpdateBreakables(float elapsedSec)
{
	for (Breakable* pBreakable : m_pBreakables)
	{
		pBreakable->Update(elapsedSec);
	}
}

void Level::DrawPowerUps() const
{
	for (PowerUp* pPowerUp : m_pPowerUps)
	{
		pPowerUp->Draw();
	}
}

#pragma region parsing
void Level::LoadGameObjectDataFromFile(const std::string& filePath)
{
	//reads between <Layer> & </Layer>, passes this to creation
	std::ifstream ifs{ filePath };
	if (!ifs.good())
	{
		std::cout << filePath << " failed to load\n";
		return;
	}

	std::string gameObjects{};
	std::string currentLine{};
	bool isCompleted{ false };

	while (std::getline(ifs, currentLine, '\n') || !isCompleted)
	{
		gameObjects += currentLine;

		if (currentLine == "</GameObjects>")
		{
			isCompleted = true;
		}
	}

	if (isCompleted)
	{
		LoadGameObjectFromString(gameObjects);
	}
	else
	{
		std::cout << "no gameObjects found in " << filePath << '\n';
	}
}

void Level::LoadGameObjectFromString(const std::string& gameObjects)
{
	std::string objects{ GetAttributeValue("GameObjects", gameObjects) };

	bool hasCreatedAllObjects{ false };

	while (!hasCreatedAllObjects)
	{
		std::string currentObject{ GetAttributeValue("GameObject", objects) };

		if (currentObject == "")
		{
			hasCreatedAllObjects = true;
			break;
		}

		CreateGameObject(currentObject);
		DeleteAttribute("GameObject", objects);
	}
}

void Level::CreateGameObject(const std::string& gameObjectData)
{
	GameObjectType id{ static_cast<GameObjectType>(std::stoi(GetAttributeValue("ID", gameObjectData))) };
	switch (id)
	{
	case GameObjectType::platform:
		CreatePlatform(gameObjectData);
		break;
	case GameObjectType::spike:
		CreateSpike(gameObjectData);
		break;
	case GameObjectType::crawlid:
		CreateCrawlid(gameObjectData);
		break;
	case GameObjectType::breakable:
		CreateBreakable(gameObjectData);
		break;
	case GameObjectType::powerUp:
		CreatePowerUp(gameObjectData);
		break;
	}
}

void Level::CreateSpike(const std::string& spikeData)
{
	Rectf hitbox{};
	hitbox.left = std::stof(GetAttributeValue("PosX", spikeData));
	hitbox.bottom = 5103 - std::stof(GetAttributeValue("PosY", spikeData)) - 100;
	hitbox.width = std::stof(GetAttributeValue("Width", spikeData));
	hitbox.height = std::stof(GetAttributeValue("Height", spikeData));

	m_pSpikes.push_back(new Spike{ hitbox });
}

void Level::CreatePlatform(const std::string& platformData)
{
	Point2f pos{};
	PlatformType type{};
	float width{}, height{};
	pos.x = std::stof(GetAttributeValue("PosX", platformData));
	pos.y = 5103.f - std::stof(GetAttributeValue("PosY", platformData)) - 210;
	width = std::stof(GetAttributeValue("Width", platformData));
	height = std::stof(GetAttributeValue("Height", platformData));
	type = static_cast<PlatformType>(std::stoi(GetAttributeValue("Type", platformData)));

	m_pPlatforms.push_back(new Platform{ Rectf{pos.x, pos.y, width, height}, type});
}

void Level::CreateCrawlid(const std::string& crawlidData)
{
	Rectf boundaries{};
	boundaries.left = std::stof(GetAttributeValue("PosX", crawlidData));
	boundaries.bottom = std::stof(GetAttributeValue("PosY", crawlidData));
	boundaries.width = std::stof(GetAttributeValue("Width", crawlidData));
	boundaries.height = std::stof(GetAttributeValue("Height", crawlidData));

	m_pEnemies.push_back(new Crawlid{ boundaries });
}

void Level::CreateBreakable(const std::string& breakableData)
{
	Rectf hitbox{};
	BreakableType type{};
	int maxHealth{};
	bool isSolid{};
	float breakTime{};
	std::string xmlPath{};

	hitbox.left = std::stof(GetAttributeValue("PosX", breakableData));
	hitbox.bottom = std::stof(GetAttributeValue("PosY", breakableData));
	hitbox.width = std::stof(GetAttributeValue("Width", breakableData));
	hitbox.height = std::stof(GetAttributeValue("Height", breakableData));
	maxHealth = std::stoi(GetAttributeValue("Health", breakableData));
	isSolid = static_cast<bool>(std::stoi(GetAttributeValue("Solid", breakableData)));
	type = static_cast<BreakableType>(std::stoi(GetAttributeValue("Type", breakableData)));
	breakTime = std::stof(GetAttributeValue("BreakTime", breakableData));
	switch (type)
	{
	case BreakableType::spikeA:
		xmlPath = "Resources/XML/SpikeAAnim.xml";
		break;
	case BreakableType::spikeB:
		xmlPath = "Resources/XML/SpikeBAnim.xml";
		break;
	case BreakableType::doorway:
		xmlPath = "Resources/XML/DoorwayAnim.xml";
		break;
	default:
		break;
	}

	m_pBreakables.push_back(new Breakable{ maxHealth, hitbox, xmlPath, isSolid, breakTime});
}

void Level::CreatePowerUp(const std::string& powerUpData)
{
	Point2f pos{};

	pos.x = std::stof(GetAttributeValue("PosX", powerUpData));
	pos.y = std::stof(GetAttributeValue("PosY", powerUpData));

	m_pPowerUps.push_back( new PowerUp{pos} );
}
#pragma endregion parsing