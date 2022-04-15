#pragma once
#include <vector>
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"

class Level
{
public:
	Level();
	~Level();

	void DrawBackground() const;
	void DrawForeground() const;

	void HandleCollision(Rectf& actorHitbox, Vector2f& actorVelocity);

	bool IsOnGround(const Rectf& actorHitbox) const;
private:
	std::vector<Point2f> m_Vertices;
	Texture* m_pBackgroundTexture;
	Texture* m_pForegroundTexture;

	void InitLevelVerts();
	void CleanUp();
};

