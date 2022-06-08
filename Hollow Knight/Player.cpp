#include "Player.h"
#include "utils.h"

using namespace utils;

Player::Player(const Point2f& startPos)
	:m_WalkSpeed{400},
	m_JumpSpeed{23},
	m_TerminalVelocity{-400},
	m_Gravity{0,-2000},
	m_AirControl{1},
	m_PlayerStates{},
	m_Animations{"Resources/XML/KnightAnimations.xml", "Resources/Sprites/KnightSheet.png"},
	m_StateHandler{m_PlayerStates},
	m_LookDirHor{LookDirection::right}
{
	InitializeHitboxes(startPos); 
}

void Player::Update(float elapsedSec)
{
	HandleKeyboardState();
	m_StateHandler.Update(m_PlayerStates, m_Velocity, elapsedSec);
	CalculateVelocity(elapsedSec);
	MovePlayer(elapsedSec);
	UpdateAnimation(elapsedSec);
}

void Player::Draw() const
{
	switch (m_LookDirHor)
	{
	case LookDirection::left:
		DrawAnimation();
		DrawHitboxes();
		break;
	case LookDirection::right:
		glPushMatrix();
		{
			FlipImageHorizontal();
			DrawAnimation();
			DrawHitboxes();
		}
		glPopMatrix();
		break;
	}
}

AnimationState Player::CalculateAnimationState() const
{
	switch (m_PlayerStates.action)
	{
	case MovementState::idle:
		return AnimationState::idle;
		break;
	case MovementState::run:
		return AnimationState::run;
		break;
	case MovementState::jump:
		return AnimationState::jump;
		break;
	case MovementState::fall:
		return AnimationState::fall;
		break;
	case MovementState::attack:
		switch (m_PlayerStates.lookDir)
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
	Point2f centerPos{ m_CombatHitbox.left + m_CombatHitbox.width / 2, m_CombatHitbox.bottom + m_CombatHitbox.height / 2 + verticalOffsetCenter };
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
	switch (m_PlayerStates.action)
	{
	case MovementState::idle:
		m_Velocity.x = 0;
	case MovementState::run:
		if (m_PlayerStates.moveDir == MoveDirection::right)
		{
			m_Velocity.x = m_WalkSpeed;
		}
		else  if (m_PlayerStates.moveDir == MoveDirection::left)
		{
			m_Velocity.x = -m_WalkSpeed;
		}
		break;
	case MovementState::jump:
	case MovementState::fall:
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_A])
		{
			m_Velocity.x = -m_WalkSpeed * m_AirControl;
			break;
		}
		if (pStates[SDL_SCANCODE_D])
		{
			m_Velocity.x = m_WalkSpeed * m_AirControl;
			break;
		}
		m_Velocity.x = 0;
		break;
	}

	//y component
	if (m_PlayerStates.action == MovementState::jump || m_PlayerStates.action == MovementState::fall)
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
	m_CombatHitbox.left += elapsedSec * m_Velocity.x;
	m_CombatHitbox.bottom += elapsedSec * m_Velocity.y;
	m_CollisionHitbox.left += elapsedSec * m_Velocity.x;
	m_CollisionHitbox.bottom += elapsedSec * m_Velocity.y;
}

void Player::HandleKeyboardState()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	
	if (pStates[SDL_SCANCODE_A])
	{
		m_PlayerStates.moveDir = MoveDirection::left;
		m_PlayerStates.lookDir = LookDirection::left;
		m_LookDirHor = LookDirection::left;
	}
	if (pStates[SDL_SCANCODE_D])
	{
		m_PlayerStates.moveDir = MoveDirection::right;
		m_PlayerStates.lookDir = LookDirection::right;
		m_LookDirHor = LookDirection::right;
	}
	if (pStates[SDL_SCANCODE_W])
	{
		m_PlayerStates.lookDir = LookDirection::up;
	}
	if (pStates[SDL_SCANCODE_S])
	{
		m_PlayerStates.lookDir = LookDirection::down;
	}

	if (!pStates[SDL_SCANCODE_A] && !pStates[SDL_SCANCODE_D])
	{
		m_PlayerStates.moveDir = MoveDirection::none;
	}
}

void Player::SetIsOnGround(bool isOnGround)
{
	m_PlayerStates.isOnGround = isOnGround;
}

//void Player::HandleKeyDown(const SDL_KeyboardEvent& e)
//{
//	switch (e.keysym.sym)
//	{
//	case SDLK_SPACE:
//		if (m_PlayerStates.isOnGround)
//		{
//			m_Velocity.y = m_JumpSpeed;
//			m_CombatHitbox.bottom += 1;
//		}
//		break;
//	}
//}

void Player::FlipImageHorizontal() const
{
	glTranslatef(m_CombatHitbox.left * 2 + m_CombatHitbox.width, 0, 0);
	glScalef(-1, 1, 1);
}

PlayerStates Player::GetState() const
{
	return m_PlayerStates;
}

void Player::ResetAnimations()
{
	m_Animations.ResetAnim();
}

void Player::InitializeHitboxes(const Point2f& startPos)
{
	verticalOffsetCenter = 15.f;
	float combatWidth{65}, combatHeight{95}, collisionWidth{40}, collisionHeight{combatHeight};

	m_HitboxDif = (combatWidth - collisionWidth) / 2;

	m_CombatHitbox.left = startPos.x - combatWidth / 2;
	m_CombatHitbox.bottom = startPos.y - (combatHeight) / 2 - verticalOffsetCenter;
	m_CombatHitbox.width = combatWidth;
	m_CombatHitbox.height = combatHeight;

	m_CollisionHitbox.left = m_CombatHitbox.left + m_HitboxDif;
	m_CollisionHitbox.bottom = m_CombatHitbox.bottom;
	m_CollisionHitbox.width = collisionWidth;
	m_CollisionHitbox.height = collisionHeight;
}

void Player::DrawHitboxes() const
{
	Color4f red{ 1,0,0,1 }, green{0,1,0,1};
	SetColor(red);;
	DrawRect(m_CombatHitbox);
	SetColor(green);
	DrawRect(m_CollisionHitbox);
}