#pragma once
#include "structs.h"
#include "Vector2f.h"

class Camera
{
public:
	Camera();
	Camera(float width, float height, const Rectf& target);

	void Transform(const Rectf& target);

	void SetLevelBoundaries(const Rectf& levelBoundaries);

private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	Point2f m_Position;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
	Point2f GetCenter(const Rectf& target) const;
};

