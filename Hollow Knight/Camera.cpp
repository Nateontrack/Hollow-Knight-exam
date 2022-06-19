#include "Camera.h"
#include "utils.h"
#include <SDL_opengl.h>
#include <iostream>
using namespace utils;

Camera::Camera()
	:Camera(0,0,1,Rectf{})
{}

Camera::Camera(float width, float height, float windowScaleFactor, const Rectf& target)
	:m_Width{width},
	m_Height{height},
	m_WindowScaleFactor{windowScaleFactor},
	m_FollowSpeed{10.f},
	m_LevelBoundaries{0,0, width, height}
{
	m_Position = Track(target);
	Clamp(m_Position);
}

Camera& Camera::operator=(Camera&& other) noexcept
{
	if (&other != this)
	{
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_FollowSpeed = other.m_FollowSpeed;
		m_WindowScaleFactor = other.m_WindowScaleFactor;
		m_LevelBoundaries = other.m_LevelBoundaries;
		m_LastTransform = other.m_LastTransform;
		m_Position = other.m_Position;
	}
	return *this;
}

void Camera::Update(const Rectf& target, float elapsedSec)
{
	//time based camera following FollowSpeed determines the speed of camera movement (smoothing)
	Point2f newPos{}, targetPos{Track(target)};
	float currentSmoothing{ m_FollowSpeed * elapsedSec };
	newPos = Lerp(m_Position, targetPos, currentSmoothing);

	Clamp(newPos);
	m_LastTransform = newPos - m_Position;
	m_Position = newPos;
}

Vector2f Camera::GetLastCameraTransform() const
{
	return m_LastTransform;
}

void Camera::Transform() const
{
	glTranslatef(-m_Position.x, -m_Position.y, 0);
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f targetCenter{GetCenter(target)};

	Point2f bottomLeftCamera{targetCenter.x - m_Width / 2 / m_WindowScaleFactor,
		targetCenter.y - m_Height / 5 / m_WindowScaleFactor * 2};
	//target rect is in the middle of camera rect

	return bottomLeftCamera;
}

void Camera::Clamp(Point2f& bottomLeftPos) const
{
	Point2f topRight{ bottomLeftPos.x + m_Width, bottomLeftPos.y + m_Height };

	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}

	if (bottomLeftPos.y < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}

	if (topRight.x > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x -= topRight.x - (m_LevelBoundaries.left + m_LevelBoundaries.width);
	}

	if (topRight.y > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y -= topRight.y - (m_LevelBoundaries.bottom + m_LevelBoundaries.height);
	}
}

Point2f Camera::GetCenter(const Rectf& target) const
{
	Point2f targetCenter{ target.left + target.width / 2,
		target.bottom + target.height / 2 };
	return targetCenter;
}
