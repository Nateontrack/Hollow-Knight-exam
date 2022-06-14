#pragma once
#include "structs.h"
#include "Texture.h"
#include "Vector2f.h"
#include <vector>
class Platform
{
public:
	Platform();
	Platform(const Point2f& bottomLeft);

	//main loop
	void Draw() const;

	//collision
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity);
	bool IsOnGround(const Rectf& actorShape) const;

private:
	Rectf m_Shape;
	std::vector<Point2f> m_PlatformVertices;
	Texture* m_pTexture;

	const float m_CollisionOffset;
};

