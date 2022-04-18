#include "Camera.h"
#include "utils.h"
#include <SDL_opengl.h>

using namespace utils;

Camera::Camera()
	:Camera(0,0)
{}

Camera::Camera(float width, float height)
	:m_Width{width},
	m_Height{height},
	m_LevelBoundaries{0,0, width, height}
{}

void Camera::Transform(const Rectf& target) const
{
	Point2f bottomLeftPos{Track(target)};
	Clamp(bottomLeftPos);
	glTranslatef(-bottomLeftPos.x, -bottomLeftPos.y, 0);
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f targetCenter{ target.left + target.width / 2,
		target.bottom + target.height / 2 };

	Point2f bottomLeftCamera{targetCenter.x - m_Width / 2,
		targetCenter.y - m_Height / 2}; //target rect is in the middle of camera rect

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