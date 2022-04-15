#include "Player.h"
#include "utils.h"

using namespace utils;

Player::Player(const Point2f& startPos, float width, float height)
	:m_WalkSpeed{400},
	m_JumpSpeed{12},
	m_TerminalVelocity{-400},
	m_Gravity{0,-24},
	m_AirControl{0.8f},
	m_ActionState{PlayerState::fall},
	m_MoveDirState{MoveDirection::right},
	m_LookDirState{LookDirection::right},
	m_Animations{"Resources/XML/KnightAnimations.xml", "Resources/Sprites/KnightSheet.png"},
	m_IsOnGround{false}
{
	m_Hitbox.left = startPos.x - width / 2;
	m_Hitbox.bottom = startPos.y - height / 2;
	m_Hitbox.width = width;
	m_Hitbox.height = height;
}

void Player::Update(float elapsedSec)
{
	HandleKeyboardState();
	CalculateVelocity(elapsedSec);
	MovePlayer(elapsedSec);
	UpdateAnimation(elapsedSec);
	
}

void Player::Draw() const
{
	Color4f red{ 1,0,0,1 };
	SetColor(red);

	DrawAnimation();
	DrawRect(m_Hitbox);
}

AnimationState Player::CalculateAnimationState() const
{
	switch (m_ActionState)
	{
	case PlayerState::idle:
		return AnimationState::idle;
		break;
	case PlayerState::run:
		return AnimationState::run;
		break;
	case PlayerState::jump:
		return AnimationState::jump;
		break;
	case PlayerState::fall:
		return AnimationState::fall;
		break;
	case PlayerState::attack:
		switch (m_LookDirState)
		{
		case LookDirection::left:
		case LookDirection::right:
			return AnimationState::attackHor;
			break;

		case LookDirection::up:
			return AnimationState::attackUp;
			break;

		case LookDirection::down:
			return AnimationState::attackDown;
			break;
		default:
			return AnimationState::attackHor;
		}
		break;
	default:
		return AnimationState::idle;
		break;
	}
}

Point2f Player::GetCenterPos() const
{
	Point2f centerPos{ m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2 };
	return centerPos;
}

void Player::DrawAnimation() const
{
	m_Animations.Draw(CalculateAnimationState(), GetCenterPos());
}

void Player::UpdateAnimation(float elapsedSec)
{
	m_Animations.Update(CalculateAnimationState(), elapsedSec);
}

void Player::CalculateVelocity(float elapsedSec)
{
	//x component
	switch (m_ActionState)
	{
	case PlayerState::idle:
	case PlayerState::attack:
		m_Velocity.x = 0;
		break;
	case PlayerState::run:
		if (m_MoveDirState == MoveDirection::right)
		{
			m_Velocity.x = m_WalkSpeed * elapsedSec;
		}
		else  if (m_MoveDirState == MoveDirection::left)
		{
			m_Velocity.x = -m_WalkSpeed * elapsedSec;
		}
		break;
	case PlayerState::jump:
	case PlayerState::fall:
		if (m_MoveDirState == MoveDirection::right)
		{
			m_Velocity.x = m_WalkSpeed * m_AirControl * elapsedSec;
		}
		else if (m_MoveDirState == MoveDirection::left)
		{
			m_Velocity.x = -m_WalkSpeed * m_AirControl * elapsedSec;
		}
		break;
	}

	//y component
	if (m_ActionState == PlayerState::jump || m_ActionState == PlayerState::fall)
	{
		m_Velocity += elapsedSec * m_Gravity;
	}
	else
	{
		m_Velocity.y = 0;
	}
}

void Player::MovePlayer(float elapsedSec)
{
	m_Hitbox.left += m_Velocity.x;
	m_Hitbox.bottom += m_Velocity.y;
}

void Player::HandleKeyboardState()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	
	if (pStates[SDL_SCANCODE_A])
	{
		m_MoveDirState = MoveDirection::left;
		m_LookDirState = LookDirection::left;
		if (m_IsOnGround && m_ActionState != PlayerState::attack) m_ActionState = PlayerState::run;
	}
	if (pStates[SDL_SCANCODE_D])
	{
		m_MoveDirState = MoveDirection::right;
		m_LookDirState = LookDirection::right;
		if (m_IsOnGround && m_ActionState != PlayerState::attack) m_ActionState = PlayerState::run;
	}
	if (pStates[SDL_SCANCODE_W])
	{
		m_LookDirState = LookDirection::up;
	}
	if (pStates[SDL_SCANCODE_S])
	{
		m_LookDirState = LookDirection::down;
	}

	if (!pStates[SDL_SCANCODE_A] && !pStates[SDL_SCANCODE_D])
	{
		m_MoveDirState = MoveDirection::none;
		if (!pStates[SDL_SCANCODE_SPACE] && m_IsOnGround)
		{
			m_ActionState = PlayerState::idle;
		}
	}
}

void Player::SetIsOnGround(bool isOnGround)
{
	m_IsOnGround = isOnGround;
}

void Player::HandleKeyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		if (m_IsOnGround)
		{
			m_ActionState = PlayerState::jump;
			m_Velocity.y = m_JumpSpeed;
		}
		break;
	}
}

