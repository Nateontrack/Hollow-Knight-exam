#pragma once
#include "GameObjectStates.h"
#include"pch.h"
#include "Vector2f.h"

class StateHandler final
{
public:
	StateHandler(PlayerStates startStates);

	void Update(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);

	bool CheckForDash();
	void SetHasDash(bool hasDash);

private:
	PlayerStates m_PreviousStates;
	float m_GeneralTimer;
	float m_AttackTimer;
	float m_InvincibilityTimer;
	float m_DashTimer;
	float m_DashCooldownTimer;
	const float m_DashTime;
	const float m_DashCooldownTime;
	const float m_AttackTime;
	const float m_DeathTime;
	const float m_MaxJumpTime;
	const float m_JumpSpeed;
	const float m_InvincibilityTime;
	const float m_RecoilTime;
	bool m_HasDash;

	//State functions
	void Idle(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Run(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Jump(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Fall(PlayerStates& currentState, float elapsedSec);
	void Damaged(PlayerStates& currentState, float elapsedSec);
	void Death(PlayerStates& currentState, float elapsedSec);
	void Dash(PlayerStates& currentState, float elapsedSec);
	void UpdateAttack(PlayerStates& currentState, float elapsedSec);
	void UpdateInvincibility(PlayerStates& currentState, float elapsedSec);

	//Sounds
	std::string m_JumpSound;

	//Check functions
	bool CheckDirInput();
	bool CheckJumpInput();
	LookDirection GetCurrentLookDir(PlayerStates& currentState) const;
};