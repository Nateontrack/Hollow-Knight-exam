#pragma once
#include "GameObjectStates.h"
#include "Spritesheet.h"
#include "structs.h"
#include "Vector2f.h"
#include "StateHandler.h"

class Player
{
public:
	Player(const Point2f& startPos);
	
	void Update(float elapsedSec);
	void Draw() const;
	void HandleKeyDown(const SDL_KeyboardEvent& e);
	void HandleKeyUp(const SDL_KeyboardEvent& e);
	bool ExecuteAttack();

	void ResetAnimations(bool isAttackingAnim);
	void HitPlayer(bool hitBySpike);

	//getters & setters
	Point2f GetCenterPos() const;
	PlayerStates GetState() const;
	CollisionFunc& GetCollisionFunc();
	void SetIsOnGround(bool isOnGround);

private:
	//player stats
	int m_MaxHealth;
	int m_Health;
	int m_Coins;

	//states
	PlayerStates m_PlayerStates;
	LookDirection m_LookDirHor;
	StateHandler m_StateHandler;

	//physics
	const float m_WalkSpeed;
	const float m_TerminalVelocity;
	const float m_AirControl;
	const float m_JumpSpeed;
	const Vector2f m_Gravity;

	float m_HitboxDif;

	//Hitboxes
	Vector2f m_AttackOffset;
	CollisionFunc m_Collision;

	//animation
	Spritesheet m_Animations;

	//offsets
	float verticalOffsetCenter;

	//Functionality
	//main loop
	void DrawAnimation() const;
	void UpdateAnimation(float elapsedSec);

	void HandleKeyboardState();
	void CalculateVelocity(float elapsedSec);
	void MovePlayer(float elapsedSec);
	void FlipImageHorizontal() const;

	//Getters & Setters
	AnimationState CalculateAnimationState() const;
	AnimationState CalculateEffectAnimation() const;
	Rectf CalculateAttackHitbox() const;

	//Initialization
	void InitializeCollision(const Point2f& startPos);

	//debugging
	void DrawHitboxes() const;

	//respawning
	void SoftRespawn();
	void HardRespawn();
	void HandleRespawning();
};