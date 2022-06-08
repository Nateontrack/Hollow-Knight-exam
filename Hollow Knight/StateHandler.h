#pragma once
#include "GameObjectStates.h"
#include <SDL_keyboard.h>
#include <SDL_events.h>
#include "Vector2f.h"

class StateHandler final
{
public:
	StateHandler(PlayerStates startStates);

	void Update(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
private:
	PlayerStates m_PreviousStates;
	float m_AccumulatedTime;
	float m_AccumulatedAttackTime;
	const float m_DashTime;
	const float m_AttackTime;
	const float m_DeathTime;
	const float m_MaxJumpTime;
	const float m_JumpSpeed;

	//State functions
	void Idle(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Run(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Jump(PlayerStates& currentState, Vector2f& velocity, float elapsedSec);
	void Fall(PlayerStates& currentState, float elapsedSec);
	void UpdateAttack(PlayerStates& currentState, float elapsedSec);

	//Check functions
	bool CheckDirInput();
	bool CheckJumpInput();
};