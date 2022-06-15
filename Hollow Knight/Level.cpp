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
	m_AmbientSound{"Resources/Sounds/CaveAmbience.wav"}
{
	//PlaySoundStream(m_AmbientSound);
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
	m_Boundaries.width = 2581;
	m_Boundaries.height = 1665;
}

void Level::DrawEntities() const
{
	DrawEnemies();
	DrawBreakables();
}

void Level::DrawForeground() const
{
	DrawPlatforms();
	DrawDebugRectsSpikes();
}

void Level::HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity)
{
	HandleCollisionLevel(actorHitbox, actorVelocity);
	HandleCollisionPlatforms(actorHitbox, actorVelocity);
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
	UpdateBreakables(elapsedSec);
}

void Level::DrawDebugRectsSpikes() const
{
	for (Spike* it : m_pSpikes)
	{
		it->DrawHitbox();
	}
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
	}
}

void Level::CreateSpike(const std::string& spikeData)
{
	Rectf hitbox{};
	hitbox.left = std::stof(GetAttributeValue("PosX", spikeData));
	hitbox.bottom = std::stof(GetAttributeValue("PosY", spikeData));
	hitbox.width = std::stof(GetAttributeValue("Width", spikeData));
	hitbox.height = std::stof(GetAttributeValue("Height", spikeData));

	m_pSpikes.push_back(new Spike{ hitbox });
}

void Level::CreatePlatform(const std::string& platformData)
{
	Point2f pos{};
	pos.x = std::stof(GetAttributeValue("PosX", platformData));
	pos.y = std::stof(GetAttributeValue("PosY", platformData));

	m_pPlatforms.push_back(new Platform{ pos });
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
#pragma endregion parsing