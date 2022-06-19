#include "Button.h"
#include "TextureManager.h"
#include "utils.h"

using namespace utils;

Button::Button(const std::string& buttonText, const Point2f& pos, bool volumeButton)
	:m_TextSize{50},
	m_IsHoovering{false},
	m_pSelectedTexture{TextureManager::GetInstance()->GetTexture("Resources/Sprites/MenuSelect.png")},
	m_IsVolumeButton{volumeButton}
{
	Color4f white{ 1,1,1,1 };
	m_pButtonTexture = new Texture{ buttonText, "Resources/Fonts/Trajan pro regular.ttf", m_TextSize, white };
	if (m_IsVolumeButton)
	{
		m_Hitbox.left = pos.x;
	}
	else
	{
		m_Hitbox.left = 960 - m_pButtonTexture->GetWidth() / 2;
	}
	m_Hitbox.bottom = pos.y;
	m_Hitbox.width = m_pButtonTexture->GetWidth();
	m_Hitbox.height = m_pButtonTexture->GetHeight();
}

Button::~Button()
{
	delete m_pButtonTexture;
	m_pButtonTexture = nullptr;
}

void Button::Draw() const
{
	m_pButtonTexture->Draw(Point2f{ m_Hitbox.left, m_Hitbox.bottom });
	if (m_IsHoovering) DrawHooverTexture();
}

void Button::DrawHooverTexture() const
{
	if (m_IsHoovering)
	{
		Point2f drawPositionLeft{ m_Hitbox.left - m_pSelectedTexture->GetWidth(), m_Hitbox.bottom };
		Point2f drawPositionRight{ m_Hitbox.left + m_Hitbox.width, m_Hitbox.bottom };

		m_pSelectedTexture->Draw(drawPositionLeft);
		glPushMatrix();
		{
			glTranslatef(drawPositionRight.x * 2 + m_pSelectedTexture->GetWidth(), 0, 0);
			glScalef(-1, 1, 1);
			m_pSelectedTexture->Draw(drawPositionRight);
		}
		glPopMatrix();
	}
}

void Button::CheckIsHoovering(const Point2f& mousePos)
{
	m_IsHoovering = IsPointInRect(mousePos, m_Hitbox);
}

bool Button::CheckForHit(const Point2f& mousePos) const
{
	return IsPointInRect(mousePos, m_Hitbox);
}