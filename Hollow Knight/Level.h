#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include "Platform.h"
#include "Crawlid.h"
#include "Spike.h"
#include "Breakable.h"
#include "PowerUp.h"

class Level final
{
public:
	Level(const std::string& filePath, const Point2f& startPos);
	~Level();

	void Update(float elapsedSec, const Point2f& playerPos);
	void DrawEntities() const;
	void DrawForeground() const;
	
	void HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec);
	void HandleAttack(const Rectf& attackRect);
	bool CheckForHitSpikes(const Rectf& actorHitbox) const;
	bool CheckForHitEnemies(const Rectf& actorHitbox) const;
	bool CheckForHitPowerUp(const Rectf& actorHitbox);
	bool IsOnGround(const Rectf& actorHitbox) const;
	
	Rectf GetBoundaries() const;
private:
	std::vector<std::vector <Point2f>> m_Vertices;
	std::vector<Platform*> m_pPlatforms;
	std::vector<Enemy*> m_pEnemies;
	std::vector<Spike*> m_pSpikes;
	std::vector<Breakable*> m_pBreakables;
	std::vector<PowerUp*> m_pPowerUps;
	Rectf m_Boundaries;

	const float m_CollisionOffset;
	Point2f m_RespawnPos;

	std::string m_AmbientSound;

	//functions
	void InitLevelVerts();
	void InitLevelBoundaries();
	void CleanUp();

	void HandleCollisionLevel(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec);
	void HandleCollisionPlatforms(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec);
	void HandleCollisionBreakables(Rectf& actorHitbox, Vector2f& actorVelocity, float elapsedSec);
	
	bool IsOnGroundLevel(const Rectf& actorHitbox) const;
	bool IsOnGroundPlatforms(const Rectf& actorHitbox) const;
	void DrawPlatforms() const;
	void UpdateEnemies(float elapsedSec, const Point2f& playerPos);
	void DrawEnemies() const;
	void UpdateBreakables(float elapsedSec);
	void DrawBreakables() const;
	void DrawPowerUps() const;
	

	//parsing
	void LoadGameObjectDataFromFile(const std::string& filePath);
	void LoadGameObjectFromString(const std::string& gameObjects);
	void CreateGameObject(const std::string& gameObjectData);
	void CreateSpike(const std::string& spikeData);
	void CreatePlatform(const std::string& platformData);
	void CreateCrawlid(const std::string& crawlidData);
	void CreateBreakable(const std::string& breakableData);
	void CreatePowerUp(const std::string& powerUpData);
};

