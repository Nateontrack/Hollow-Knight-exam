#pragma once
#include "pch.h"
#include <vector>
#include "Spritesheet.h"
#include "GameObjectStates.h"

class HUD
{
public:
	HUD();

	void Update(float elapsedSec);
	void Draw() const;

	void SetHasDash(bool hasCollectedPowerup);
	void HandleHit();
	

private:
	Point2f m_PosDashIcon;
	Point2f m_PosHealthIcons;
	int m_MaxHealth;
	int m_CurrentHealth;
	bool m_HasDash;
	std::vector<HealthState> m_HealhStates;
	std::vector<Spritesheet> m_HealthIcons;
	Spritesheet m_DashIcon;
	float m_HealthOffset;
	float m_AccumulatedTime;
	float m_BreakTime;

	void Initialize();
	void Respawn();
	AnimationState CalculateAnimState(HealthState m_State) const;
};

