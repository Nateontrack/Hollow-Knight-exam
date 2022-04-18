#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include "Platform.h"
#include "Crawlid.h"

class Level
{
public:
	Level();
	~Level();

	void Update(float elapsedSec);
	void DrawBackground() const;
	void DrawForeground() const;
	
	void HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity);
	bool IsOnGround(const Rectf& actorHitbox) const;
	


	Rectf GetBoundaries() const;
private:
	std::vector<std::vector <Point2f>> m_Vertices;
	std::vector<Platform*> m_pPlatforms;
	std::vector<Enemy*> m_pEnemies;
	Texture* m_pBackgroundTexture;
	Texture* m_pForegroundTexture;
	Rectf m_Boundaries;

	const float m_CollisionOffset;

	//debug
	

	void InitLevelVerts();
	void InitLevelBoundaries();
	void InitPlatforms();
	void InitEnemies();
	void CleanUp();

	void HandleCollisionLevel(Rectf& actorHitbox, Vector2f& actorVelocity);
	void HandleCollisionPlatforms(Rectf& actorHitbox, Vector2f& actorVelocity);
	bool IsOnGroundLevel(const Rectf& actorHitbox) const;
	bool IsOnGroundPlatforms(const Rectf& actorHitbox) const;
	void DrawPlatforms() const;
	void UpdateEnemies(float elapsedSec);
	void DrawEnemies() const;
};

