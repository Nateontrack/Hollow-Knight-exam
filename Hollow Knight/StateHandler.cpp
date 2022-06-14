#include "StateHandler.h"
#include "utils.h"
#include <SDL_keyboard.h>

StateHandler::StateHandler(PlayerStates startStates)
	:m_PreviousStates{ startStates },
	m_AccumulatedTime{ 0 },
	m_AccumulatedAttackTime{ 0 },
	m_AccumulatedInvTime{0},
	m_DashTime{},
	m_AttackTime{0.5f},
	m_DeathTime{1.5f},
	m_MaxJumpTime{0.8f}, 
	m_JumpSpeed{ 1300 },
	m_InvincibilityTime{1.2f},
	m_RecoilTime{2.f / 3.f}
{}

void StateHandler::Update(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{
	UpdateAttack(currentState, elapsedSec);
	UpdateInvincibility(currentState, elapsedSec);
	switch (currentState.action)
	{
	case MovementState::idle:
		Idle(currentState, velocity, elapsedSec);
		break;
	case MovementState::run:
		Run(currentState, velocity, elapsedSec);
		break;
	case MovementState::jump:
		Jump(currentState, velocity, elapsedSec);
		break;
	case MovementState::fall:
		Fall(currentState, elapsedSec);
		break;
	case MovementState::damaged:
		Damaged(currentState, elapsedSec);
		break;
	case MovementState::death:
	case MovementState::spikeDeath:
		Death(currentState, elapsedSec);
		break;
	}
}

//State functions

void StateHandler::UpdateAttack(PlayerStates& currentState, float elapsedSec)
{
	if (currentState.isAttacking)
	{
		m_AccumulatedAttackTime += elapsedSec;
		if (m_AccumulatedAttackTime > m_AttackTime)
		{
			currentState.isAttacking = false;
			m_AccumulatedAttackTime = 0;
		}
	}
}

void StateHandler::UpdateInvincibility(PlayerStates& currentState, float elapsedSec)
{
	if (currentState.isInvincible)
	{
		m_AccumulatedInvTime += elapsedSec;
		if (m_AccumulatedInvTime > m_InvincibilityTime)
		{
			m_AccumulatedInvTime = 0;
			currentState.isInvincible = false;
		}
	}
}

void StateHandler::Idle(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{
	if (currentState.isOnGround)
	{
		if (CheckDirInput())
		{
			currentState.action = MovementState::run;
		}

		if (CheckJumpInput())
		{
			currentState.action = MovementState::jump;
			velocity.y = m_JumpSpeed;
		}
	}
	else
	{
		currentState.action = MovementState::fall;
	}
}

void StateHandler::Run(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{
	if (currentState.isOnGround)
	{
		if (!CheckDirInput())
		{
			currentState.action = MovementState::idle;
		}
		if (CheckJumpInput())
		{
			currentState.action = MovementState::jump;
			velocity.y = m_JumpSpeed;
		}
	}
	else
	{
		currentState.action = MovementState::fall;
	}
}

void StateHandler::Jump(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{

	if (!currentState.isOnGround)
	{
		m_AccumulatedTime += elapsedSec;
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_SPACE])
		{
			if (m_AccumulatedTime > m_MaxJumpTime)
			{
				currentState.action = MovementState::fall;
				m_AccumulatedTime = 0;
			}
		}
		else
		{
			currentState.action = MovementState::fall;
			if (velocity.y > 0)
			{
				velocity.y = 0;
			}
			m_AccumulatedTime = 0;
		}
	}
	else
	{
		if (CheckDirInput())
		{
			currentState.action = MovementState::run;
		}
		else
		{
			currentState.action = MovementState::idle;
		}
		m_AccumulatedTime = 0;
	}
}

void StateHandler::Fall(PlayerStates& currentState, float elapsedSec)
{
	if (currentState.isOnGround)
	{
		if (CheckDirInput())
		{
			currentState.action = MovementState::run;
		}
		else
		{
			currentState.action = MovementState::idle;
		}
	}
}

void StateHandler::Damaged(PlayerStates& currentState, float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;
	if(m_AccumulatedTime > m_RecoilTime)
	{
		m_AccumulatedTime = 0;
		if (currentState.isOnGround)
		{
			if (CheckDirInput())
			{
				currentState.action = MovementState::run;
			}
			else
			{
				currentState.action = MovementState::idle;
			}
		}
		else
		{
			currentState.action = MovementState::fall;
		}
	}
}

void StateHandler::Death(PlayerStates& currentState, float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;
	if (m_AccumulatedTime > m_DeathTime)
	{
		m_AccumulatedTime = 0;
		currentState.isRespawning = true;
	}
}

//Check functions

bool StateHandler::CheckDirInput()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	//XOR gate, if both or none are pressed player shouldnt move
	if (pStates[SDL_SCANCODE_A] != pStates[SDL_SCANCODE_D])
	{
		return true;
	}
	return false;
}

bool StateHandler::CheckJumpInput()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_SPACE])
	{
		return true;
	}
	return false;
}

