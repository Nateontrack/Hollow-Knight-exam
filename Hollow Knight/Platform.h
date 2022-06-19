#pragma once
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include "GameObjectStates.h"
#include <vector>
class Platform
{
public:
	Platform();
	Platform(const Rectf& hitbox, PlatformType type);

	//main loop
	void Draw() const;

	//collision
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, float elapsedSec);
	bool IsOnGround(const Rectf& actorShape) const;

private:
	float m_CollisionRadius;
	PlatformType m_Type;
	Rectf m_Shape;
	std::vector<Point2f> m_PlatformVertices;
	Texture* m_pTexture;

	const float m_CollisionOffset;

	void InitializeTexture();
	Point2f GetCenterPos() const;
};

