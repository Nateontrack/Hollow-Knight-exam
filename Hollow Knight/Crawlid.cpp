#include "Crawlid.h"
#include "utils.h"
using namespace utils;

Crawlid::Crawlid(const Rectf& boundaries)
	:Enemy(Rectf{ boundaries.left,boundaries.bottom,110,80 }, boundaries, 2),
	m_pAnimations{new Spritesheet{"Resources/XML/CrawlidAnimations.xml", "Resources/Sprites/Crawlidsheet.png"}},
	m_ActionState{CrawlidState::walk},
	m_deathAirTime{0.25f},
	m_TurnTime{0.1667f},
	m_AccumulatedTime{},
	m_WalkSpeed{250}
{}

Crawlid::~Crawlid()
{
	delete m_pAnimations;
	m_pAnimations = nullptr;
}

void Crawlid::Update(float elapsedSec)
{
	HandleState(elapsedSec);
	HandleHit(elapsedSec);
	CalculateVelocity();
	UpdatePos(elapsedSec);
	if (Clamp())
	{
		m_ActionState = CrawlidState::turn;
		m_pAnimations->ResetAnim(false);
		if (m_MoveDirState == MoveDirection::left)
		{
			m_MoveDirState = MoveDirection::right;
		}
		else
		{
			m_MoveDirState = MoveDirection::left;
		}
		m_AccumulatedTime = 0;
		CalculateVelocity();
		UpdatePos(elapsedSec);
	}
	m_pAnimations->Update(CalculateAnimationState(), elapsedSec);
}

void Crawlid::Draw() const
{
	Color4f red{ 1,0,0,1 };
	SetColor(red);
	if (m_MoveDirState == MoveDirection::left && m_ActionState != CrawlidState::turn
		|| m_MoveDirState == MoveDirection::right && m_ActionState == CrawlidState::turn)
	{
		DrawAnimations();
		//DrawRect(m_Hitbox);
	}
	else
	{
		glPushMatrix();
		FlipImage();
		DrawAnimations();
		//DrawRect(m_Hitbox);
		
		glPopMatrix();
	}
	
}

void Crawlid::CalculateVelocity()
{
	switch (m_MoveDirState)
	{
	case MoveDirection::left:
		m_Velocity.x = -m_WalkSpeed;
		break;
	case MoveDirection::right:
		m_Velocity.x = m_WalkSpeed;
		break;
	case MoveDirection::none:
		m_Velocity = Vector2f{ 0,0 };
		break;
	default:
		break;
	}
}

void Crawlid::UpdatePos(float elapsedSec)
{
	m_Hitbox.left += m_Velocity.x * elapsedSec;
}

void Crawlid::HandleState(float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;
	switch (m_ActionState)
	{
	case CrawlidState::turn:
		if (m_AccumulatedTime > m_TurnTime)
		{
			m_AccumulatedTime = 0;
			m_ActionState = CrawlidState::walk;
		}
		break;
	case CrawlidState::deadAir:
		if (m_AccumulatedTime > m_deathAirTime)
		{
			m_AccumulatedTime = 0;
			m_ActionState = CrawlidState::deadGround;
		}
		break;
	}
}

AnimationState Crawlid::CalculateAnimationState() const
{
	switch (m_ActionState)
	{
	case CrawlidState::walk:
		return AnimationState::run;
		break;
	case CrawlidState::turn:
		return AnimationState::turn;
		break;
	case CrawlidState::deadAir:
		return AnimationState::deathAir;
		break;
	case CrawlidState::deadGround:
		return AnimationState::deathGround;
		break;
	default:
		return AnimationState::run;
		break;
	}
}

void Crawlid::FlipImage() const
{
	glTranslatef(m_Hitbox.left * 2 + m_Hitbox.width, 0, 0);
	glScalef(-1, 1, 1);
}

//bool Crawlid::HandleCollision(const Rectf& actorHitbox)
//{
//	if (m_ActionState != CrawlidState::deadGround)
//	{
//		if (IsOverlapping(m_Hitbox, actorHitbox))
//		{
//			return true;
//		}
//	}
//	return false;
//}

void Crawlid::HitEnemy()
{
	if (m_ActionState != CrawlidState::deadGround)
	{
		--m_Health;
		if (m_Health <= 0)
		{
			m_ActionState = CrawlidState::deadAir;
			m_MoveDirState = MoveDirection::none;
			m_IsDead = true;
			m_AccumulatedTime = 0;
		}
		m_IsHit = true;
	}
}

void Crawlid::HandleHit(float elapsedSec)
{
	if (m_IsHit)
	{
		m_AccumulatedHitTime += elapsedSec;
		if (m_AccumulatedHitTime > m_HitTime)
		{
			m_IsHit = false;
			m_AccumulatedHitTime = 0;
			m_pHitAnimation->ResetAnim(true);
		}
		else
		{
			m_pHitAnimation->Update(AnimationState::hit, elapsedSec);
		}
	}
}

void Crawlid::DrawAnimations() const
{
	Point2f centerPos{ GetCenterPos() };
	m_pAnimations->Draw(CalculateAnimationState(), centerPos);

	if (m_IsHit)
	{
		m_pHitAnimation->Draw(AnimationState::hit, centerPos);
	}
}