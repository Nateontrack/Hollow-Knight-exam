#pragma once
#include "structs.h"
#include "Vector2f.h"

class Camera
{
public:
	Camera();
	Camera(float width, float height, float windowScaleFactor, const Rectf& target);
	Camera& operator=(Camera&& other) noexcept;

	void Update(const Rectf& target, float elapsedSec);
	void Transform() const;

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	Vector2f GetLastCameraTransform() const;

private:
	float m_Width;
	float m_Height;
	float m_FollowSpeed; //used for lerp, more smoothing as it approaches 0
	float m_WindowScaleFactor;
	Rectf m_LevelBoundaries;
	Vector2f m_LastTransform;
	Point2f m_Position;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
	Point2f GetCenter(const Rectf& target) const;
};

