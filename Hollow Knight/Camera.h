#pragma once
#include "structs.h"

class Camera
{
public:
	Camera();
	Camera(float width, float height);

	void Transform(const Rectf& target) const;

	void SetLevelBoundaries(const Rectf& levelBoundaries);

private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};

