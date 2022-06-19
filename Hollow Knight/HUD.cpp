#include "HUD.h"

HUD::HUD()
	:m_PosDashIcon{200, 980},
	m_PosHealthIcons{275, 940},
	m_AccumulatedTime{0},
	m_BreakTime{0.5f},
	m_MaxHealth{5},
	m_HealthOffset{90.f},
	m_HasDash{false},
	m_DashIcon{ Spritesheet{ "Resources/XML/Dash.xml", "Resources/Sprites/HealthSheet.png" } }
{
	m_CurrentHealth = m_MaxHealth;
	Initialize();
}

void HUD::Update(float elapsedSec)
{
	for (HealthState& state : m_HealhStates)
	{
		if (state == HealthState::breaking)
		{
			m_AccumulatedTime += elapsedSec;
			if (m_AccumulatedTime > m_BreakTime)
			{
				m_AccumulatedTime = 0;
				state = HealthState::off;
			}
		}
	}

	for (int i = 0; i < m_MaxHealth; i++)
	{
		m_HealthIcons[i].Update(CalculateAnimState(m_HealhStates[i]), elapsedSec);
	}
}

void HUD::Draw() const
{
	if (m_HasDash)
	{
		m_DashIcon.Draw(AnimationState::run, m_PosDashIcon);
	}
	else
	{
		m_DashIcon.Draw(AnimationState::idle, m_PosDashIcon);
	}

	for (int i = 0; i < m_MaxHealth; i++)
	{
		Point2f pos{ m_PosHealthIcons.x + i * m_HealthOffset, m_PosHealthIcons.y };
		m_HealthIcons[i].Draw(CalculateAnimState(m_HealhStates[i]), pos);
	}
}

void HUD::SetHasDash(bool hasCollectedPowerup)
{
	m_HasDash = hasCollectedPowerup;
}

void HUD::HandleHit()
{
	m_CurrentHealth--;
	m_HealhStates[m_CurrentHealth] = HealthState::breaking;
	if (m_CurrentHealth <= 0)
	{
		Respawn();
	}
}

void HUD::Respawn()
{
	m_CurrentHealth = m_MaxHealth;
	for (HealthState& state : m_HealhStates)
	{
		state = HealthState::on;
	}
	for (Spritesheet& sheet : m_HealthIcons)
	{
		sheet.ResetAnim(false);
	}
}

void HUD::Initialize()
{
	for (int i = 0; i < m_MaxHealth; i++)
	{
		m_HealhStates.push_back(HealthState::on);
		m_HealthIcons.push_back(Spritesheet{ "Resources/XML/HealthAnim.xml", "Resources/Sprites/HealthSheet.png" });
	}
}

AnimationState HUD::CalculateAnimState(HealthState m_State) const
{
	switch (m_State)
	{
	case HealthState::on:
		return AnimationState::idle;
		break;
	case HealthState::breaking:
		return AnimationState::hit;
		break;
	case HealthState::off:
		return AnimationState::deathGround;
		break;
	default:
		return AnimationState::deathGround;
		break;
	}
}