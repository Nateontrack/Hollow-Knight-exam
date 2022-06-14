#include "Player.h"
#include "utils.h"
#include <iostream>

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
	m_LookDirHor{LookDirection::right},
	m_AttackOffset{-90, -10},
	m_MaxHealth{5},
	m_Coins{}
{
	m_Health = m_MaxHealth;
	InitializeCollision(startPos); 
}

void Player::Update(float elapsedSec)
{
	HandleKeyboardState();
	m_StateHandler.Update(m_PlayerStates, m_Collision.velocity, elapsedSec);
	CalculateVelocity(elapsedSec);
	MovePlayer(elapsedSec);
	UpdateAnimation(elapsedSec);
	HandleRespawning();
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
	if (m_PlayerStates.isAttacking)
	{
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
	}
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
	case MovementState::damaged:
		return AnimationState::hit;
	case MovementState::death:
		return AnimationState::deathGround;
		break;
	case MovementState::spikeDeath:
		return AnimationState::spikeDeath;
		break;
	default:
		return AnimationState::idle;
		break;
	}
}

Point2f Player::GetCenterPos() const
{
	Point2f centerPos{ m_Collision.combatHitbox.left + m_Collision.combatHitbox.width / 2,
		m_Collision.combatHitbox.bottom + m_Collision.combatHitbox.height / 2 + verticalOffsetCenter };
	return centerPos;
}

void Player::DrawAnimation() const
{
	Point2f centerPos{ GetCenterPos() };
	if (m_PlayerStates.isInvincible)
	{
		m_Animations.Draw(AnimationState::invincible, centerPos);
	}

	m_Animations.Draw(CalculateAnimationState(), centerPos);
	
	if (m_PlayerStates.isAttacking)
	{
		m_Animations.Draw(CalculateEffectAnimation(), centerPos + m_AttackOffset);
	}
}

void Player::UpdateAnimation(float elapsedSec)
{
	m_Animations.Update(CalculateAnimationState(), elapsedSec);
	if (m_PlayerStates.isInvincible)
	{
		m_Animations.Update(AnimationState::invincible, elapsedSec);
	}
	if (m_PlayerStates.isAttacking)
	{
		m_Animations.Update(CalculateEffectAnimation(), elapsedSec);
	}
}

void Player::CalculateVelocity(float elapsedSec)
{
	//x component
	switch (m_PlayerStates.action)
	{
	case MovementState::idle:
		m_Collision.velocity.x = 0;
	case MovementState::run:
		if (m_PlayerStates.moveDir == MoveDirection::right)
		{
			m_Collision.velocity.x = m_WalkSpeed;
		}
		else  if (m_PlayerStates.moveDir == MoveDirection::left)
		{
			m_Collision.velocity.x = -m_WalkSpeed;
		}
		else
		{
			m_Collision.velocity.x = 0;
		}
		break;
	case MovementState::jump:
	case MovementState::fall:
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (m_PlayerStates.moveDir == MoveDirection::left)
		{
			m_Collision.velocity.x = -m_WalkSpeed * m_AirControl;
			break;
		}
		if (m_PlayerStates.moveDir == MoveDirection::right)
		{
			m_Collision.velocity.x = m_WalkSpeed * m_AirControl;
			break;
		}
		m_Collision.velocity.x = 0;
		break;
	}

	//y component
	if (m_PlayerStates.action == MovementState::jump || m_PlayerStates.action == MovementState::fall)
	{
		m_Collision.velocity += elapsedSec * m_Gravity;
	}
	else
	{
		m_Collision.velocity.y = 0;
	}
}

void Player::MovePlayer(float elapsedSec)
{
	m_Collision.combatHitbox.left += elapsedSec * m_Collision.velocity.x;
	m_Collision.combatHitbox.bottom += elapsedSec * m_Collision.velocity.y;
	m_Collision.movementHitbox.left += elapsedSec * m_Collision.velocity.x;
	m_Collision.movementHitbox.bottom += elapsedSec * m_Collision.velocity.y;
}

void Player::HandleKeyboardState()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	
	if (pStates[SDL_SCANCODE_A])
	{
		m_PlayerStates.moveDir = MoveDirection::left;
	}
	if (pStates[SDL_SCANCODE_D])
	{
		m_PlayerStates.moveDir = MoveDirection::right;
	}
	if (pStates[SDL_SCANCODE_A] == pStates[SDL_SCANCODE_D])
	{
		m_PlayerStates.moveDir = MoveDirection::none;
	}
}

void Player::SetIsOnGround(bool isOnGround)
{
	m_PlayerStates.isOnGround = isOnGround;
}

void Player::HandleKeyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_w:
		m_PlayerStates.lookDir = LookDirection::up;
		break;
	case SDLK_s:
		m_PlayerStates.lookDir = LookDirection::down;
		break;
	case SDLK_a:
		m_PlayerStates.lookDir = LookDirection::left;
		m_LookDirHor = LookDirection::left;
		break;
	case SDLK_d:
		m_PlayerStates.lookDir = LookDirection::right;
		m_LookDirHor = LookDirection::right;
		break;
	}
}

void Player::FlipImageHorizontal() const
{
	glTranslatef(m_Collision.combatHitbox.left * 2 + m_Collision.combatHitbox.width, 0, 0);
	glScalef(-1, 1, 1);
}

PlayerStates Player::GetState() const
{
	return m_PlayerStates;
}

void Player::ResetAnimations(bool isAttackingAnim)
{
	m_Animations.ResetAnim(isAttackingAnim);
}

void Player::InitializeCollision(const Point2f& startPos)
{
	verticalOffsetCenter = 15.f;
	float combatWidth{65}, combatHeight{95}, collisionWidth{40}, collisionHeight{combatHeight},
		attackWidth{150}, attackHeight{114};

	m_HitboxDif = (combatWidth - collisionWidth) / 2;

	m_Collision.combatHitbox.left = startPos.x - combatWidth / 2;
	m_Collision.combatHitbox.bottom = startPos.y - (combatHeight) / 2 - verticalOffsetCenter;
	m_Collision.combatHitbox.width = combatWidth;
	m_Collision.combatHitbox.height = combatHeight;

	m_Collision.movementHitbox.left = m_Collision.combatHitbox.left + m_HitboxDif;
	m_Collision.movementHitbox.bottom = m_Collision.combatHitbox.bottom;
	m_Collision.movementHitbox.width = collisionWidth;
	m_Collision.movementHitbox.height = collisionHeight;

	m_Collision.attackHitbox.width = attackWidth;
	m_Collision.attackHitbox.height = attackHeight;
}

void Player::DrawHitboxes() const
{
	Color4f red{ 1,0,0,1 }, green{0,1,0,1};
	SetColor(red);;
	DrawRect(m_Collision.combatHitbox);
	//SetColor(green);
	//DrawRect(m_CollisionHitbox);
}

bool Player::ExecuteAttack()
{
	if (!m_PlayerStates.isAttacking)
	{
		m_PlayerStates.isAttacking = true;
		if (m_LookDirHor == LookDirection::left)
		{
			m_Collision.attackHitbox.left = m_Collision.combatHitbox.left - m_Collision.attackHitbox.width;
		}
		else
		{
			m_Collision.attackHitbox.left = m_Collision.combatHitbox.left + m_Collision.combatHitbox.width;
		}
		
		m_Collision.attackHitbox.bottom = m_Collision.combatHitbox.bottom;
		return true;
	}
	else return false;
}

Rectf Player::CalculateAttackHitbox() const
{
	//90 left 10 down
	if (m_PlayerStates.isAttacking)
	{
		switch (m_PlayerStates.attackDir)
		{
		case AttackDirection::horizontal:
			return Rectf{};
			break;
		case AttackDirection::up:
			return Rectf{};
			break;
		case AttackDirection::down:
			return Rectf{};
			break;
		default:
			return Rectf{};
			break;
		}
	}
	else
	{
		return Rectf{};
	}
}

AnimationState Player::CalculateEffectAnimation() const
{
	switch (m_PlayerStates.attackDir)
	{
	case AttackDirection::horizontal:
		return AnimationState::slashEffect;
		break;
	case AttackDirection::up:
		return AnimationState::slashEffect;
		break;
	case AttackDirection::down:
		return AnimationState::slashEffect;
		break;
	default:
		return AnimationState::slashEffect;
		break;
	}
}

void Player::HandleKeyUp(const SDL_KeyboardEvent& e)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	//makes sure player faces the right direction when both keys were pressed after release of 1
	switch (e.keysym.sym)
	{
	case SDLK_a:
		if (pStates[SDL_SCANCODE_D])
		{
			m_PlayerStates.lookDir = LookDirection::right;
			m_LookDirHor = LookDirection::right;
		}
		break;
	case SDLK_d:
		if (pStates[SDL_SCANCODE_A])
		{
			m_PlayerStates.lookDir = LookDirection::left;
			m_LookDirHor = LookDirection::left;
		}
		break;
	}
}

CollisionFunc& Player::GetCollisionFunc()
{
	return m_Collision;
}

void Player::HitPlayer(bool hitBySpike)
{
	//invincibility is not for spikes, only enemies
	if (m_PlayerStates.action != MovementState::death && m_PlayerStates.action != MovementState::spikeDeath)
	{
		if (hitBySpike)
		{
			m_Health--;
			std::cout << m_Health << std::endl;
			if (m_Health <= 0)
			{
				m_PlayerStates.action = MovementState::death;
				m_Collision.velocity = Vector2f{ 0,0 };
			}
			else
			{
				m_PlayerStates.action = MovementState::spikeDeath;
				m_Collision.velocity = Vector2f{ 0,0 };
			}
		}
		else if (!m_PlayerStates.isInvincible)
		{
			m_Health--;
			std::cout << m_Health << std::endl;
			if (m_Health <= 0)
			{
				m_PlayerStates.action = MovementState::death;
				m_Collision.velocity = Vector2f{ 0,0 };
			}
			else
			{
				m_PlayerStates.isInvincible = true;
				m_PlayerStates.action = MovementState::damaged;
				m_Collision.velocity = Vector2f{ 0,0 };
			}
		}
	}
}

void Player::HardRespawn()
{
	m_Health = m_MaxHealth;
	SoftRespawn();
}

void Player::SoftRespawn()
{
	m_Collision.combatHitbox.left = 200;
	m_Collision.combatHitbox.bottom = 200;
	m_PlayerStates.action = MovementState::idle;
}

void Player::HandleRespawning()
{
	if (m_PlayerStates.isRespawning)
	{
		m_PlayerStates.isRespawning = false;
		switch (m_PlayerStates.action)
		{
		case MovementState::death:
			HardRespawn();
			break;
		case MovementState::spikeDeath:
			SoftRespawn();
			break;
		}
	}
}