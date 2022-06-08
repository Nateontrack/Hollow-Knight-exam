#include "Camera.h"
#include "utils.h"
#include <SDL_opengl.h>
#include <iostream>
using namespace utils;

Camera::Camera()
	:Camera(0,0,Rectf{})
{}

Camera::Camera(float width, float height, const Rectf& target)
	:m_Width{width},
	m_Height{height},
	m_LevelBoundaries{0,0, width, height}
{
	m_Position = Track(target);
	Clamp(m_Position);
}

void Camera::Transform(const Rectf& target)
{
		Point2f bottomLeftPos{ Track(target) };
		Clamp(bottomLeftPos);
		m_Position = bottomLeftPos;
		glTranslatef(-bottomLeftPos.x, -bottomLeftPos.y, 0);
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f targetCenter{GetCenter(target)};

	Point2f bottomLeftCamera{targetCenter.x - m_Width / 2,
		targetCenter.y - m_Height / 3};
	//target rect is in the middlebottom of camera rect

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
