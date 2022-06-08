#include "StateHandler.h"
#include "utils.h"
#include <SDL_keyboard.h>

StateHandler::StateHandler(PlayerStates startStates)
	:m_PreviousStates{ startStates },
	m_AccumulatedTime{ 0 },
	m_AccumulatedAttackTime{ 0 },
	m_DashTime{},
	m_AttackTime{},
	m_DeathTime{},
	m_MaxJumpTime{0.8f}, 
	m_JumpSpeed{ 1500 }
{}

void StateHandler::Update(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{
	UpdateAttack(currentState, elapsedSec);
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

//Check functions

bool StateHandler::CheckDirInput()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_A])
	{
		return true;
	}
	if (pStates[SDL_SCANCODE_D])
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

