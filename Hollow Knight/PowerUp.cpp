#include "PowerUp.h"
#include "utils.h"
#include "TextureManager.h"

using namespace utils;

PowerUp::PowerUp(const Point2f& pos)
	:m_pTexture{ TextureManager::GetInstance()->GetTexture("Resources/Sprites/DashPickup.png")}
{
	m_Hitbox.left = pos.x;
	m_Hitbox.bottom = pos.y;
	m_Hitbox.width = m_pTexture->GetWidth();
	m_Hitbox.height = m_pTexture->GetHeight();
}

void PowerUp::Draw() const
{
	m_pTexture->Draw(Point2f{m_Hitbox.left, m_Hitbox.bottom});
}

bool PowerUp::CheckForHit(const Rectf& combatHitbox)
{
	return IsOverlapping(m_Hitbox, combatHitbox);
}