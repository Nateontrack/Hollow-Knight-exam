#include "StateHandler.h"
#include "utils.h"
#include "SoundUtils.h"

using namespace soundUtils;

StateHandler::StateHandler(PlayerStates startStates)
	:m_PreviousStates{ startStates },
	m_GeneralTimer{ 0 },
	m_AttackTimer{ 0 },
	m_InvincibilityTimer{ 0 },
	m_DashTime{ 0.35f },
	m_DashCooldownTime{ 1 },
	m_AttackTime{ 0.5f },
	m_DeathTime{ 1.5f },
	m_MaxJumpTime{ 0.8f },
	m_JumpSpeed{ 1300 },
	m_InvincibilityTime{ 1.2f },
	m_RecoilTime{ 2.f / 3.f },
	m_JumpSound{ "Resources/Sounds/Jump.wav" },
	m_DashTimer{},
	m_HasDash{ false },
	m_DashCooldownTimer{0}
{}

void StateHandler::Update(PlayerStates& currentState, Vector2f& velocity, float elapsedSec)
{
	UpdateAttack(currentState, elapsedSec);
	UpdateInvincibility(currentState, elapsedSec);
	m_DashCooldownTimer += elapsedSec;
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
	case MovementState::dash:
		Dash(currentState, elapsedSec);
	}
}

//State functions

void StateHandler::UpdateAttack(PlayerStates& currentState, float elapsedSec)
{
	if (currentState.isAttacking)
	{
		m_AttackTimer += elapsedSec;
		if (m_AttackTimer > m_AttackTime)
		{
			currentState.isAttacking = false;
			m_AttackTimer = 0;
		}
	}
}

void StateHandler::UpdateInvincibility(PlayerStates& currentState, float elapsedSec)
{
	if (currentState.isInvincible)
	{
		m_InvincibilityTimer += elapsedSec;
		if (m_InvincibilityTimer > m_InvincibilityTime)
		{
			m_InvincibilityTimer = 0;
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
			PlaySoundEffect(m_JumpSound, 1);
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
			PlaySoundEffect(m_JumpSound, 1);
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
		m_GeneralTimer += elapsedSec;
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_SPACE])
		{
			if (m_GeneralTimer > m_MaxJumpTime)
			{
				currentState.action = MovementState::fall;
				m_GeneralTimer = 0;
			}
		}
		else
		{
			currentState.action = MovementState::fall;
			if (velocity.y > 0)
			{
				velocity.y = 0;
			}
			m_GeneralTimer = 0;
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
		m_GeneralTimer = 0;
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
	m_GeneralTimer += elapsedSec;
	if(m_GeneralTimer > m_RecoilTime)
	{
		m_GeneralTimer = 0;
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
	m_GeneralTimer += elapsedSec;
	if (m_GeneralTimer > m_DeathTime)
	{
		m_GeneralTimer = 0;
		currentState.isRespawning = true;
	}
}

void StateHandler::Dash(PlayerStates& currentState, float elapsedSec)
{
	m_DashTimer += elapsedSec;
	if (m_DashTimer > m_DashTime)
	{
		m_DashTimer = 0;
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
		currentState.lookDirHor = GetCurrentLookDir(currentState);
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

bool StateHandler::CheckForDash()
{
	if (m_DashCooldownTimer > m_DashCooldownTime && m_HasDash)
	{
		m_DashCooldownTimer = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void StateHandler::SetHasDash(bool hasDash)
{
	m_HasDash = hasDash;
}

LookDirection StateHandler::GetCurrentLookDir(PlayerStates& currentState) const
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (!(pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_D]))
	{
		if (pStates[SDL_SCANCODE_A])
		{
			return LookDirection::left;
		}
		else
		{
			return LookDirection::right;
		}
	}
	else
	{
		return currentState.lookDirHor;
	}
}