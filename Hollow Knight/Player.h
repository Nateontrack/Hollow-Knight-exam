#pragma once
#include "pch.h"
#include "GameObjectStates.h"
#include "Spritesheet.h"
#include "Vector2f.h"
#include "StateHandler.h"


class Player final
{
public:
	Player(const Point2f& startPos);
	
	void Update(float elapsedSec);
	void MovePlayer(float elapsedSec);
	void Draw() const;
	void HandleKeyDown(const SDL_KeyboardEvent& e);
	void HandleKeyUp(const SDL_KeyboardEvent& e);
	bool ExecuteAttack();
	void Dash();

	void ResetAnimations(bool isAttackingAnim);
	bool HitPlayer(bool hitBySpike);

	//getters & setters
	Point2f GetCenterPos() const;
	PlayerStates GetState() const;
	CollisionFunc& GetCollisionFunc();
	void SetIsOnGround(bool isOnGround);
	void SetHasDash(bool hasDash);

private:
	//player stats
	int m_MaxHealth;
	int m_Health;
	int m_Coins;

	//states
	PlayerStates m_PlayerStates;
	StateHandler m_StateHandler;

	//physics
	const float m_WalkSpeed;
	const float m_DashSpeed;
	const float m_TerminalVelocity;
	const float m_AirControl;
	const float m_JumpSpeed;
	const Vector2f m_Gravity;

	float m_HitboxDif;

	//Sounds
	std::string m_NailSwingSound;
	std::string m_FootstepSound;
	std::string m_LandSound;
	std::string m_DashSound;
	std::string m_DeathSound;
	std::string m_HitSound;
	std::string m_CollectSound;

	//Hitboxes
	Point2f m_RespawnPoint;
	Vector2f m_AttackOffset;
	Vector2f m_DashOffset;
	CollisionFunc m_Collision;

	//animation
	Spritesheet m_Animations;
	Texture* m_pLighting;

	//offsets
	float verticalOffsetCenter;

	//Functionality
	//main loop
	void DrawAnimation() const;
	void DrawLight() const;
	void UpdateAnimation(float elapsedSec);
	void UpdateSounds(float elapsedSec);

	void HandleKeyboardState();
	void CalculateVelocity(float elapsedSec);
	void FlipImageHorizontal() const;

	//Getters & Setters
	AnimationState CalculateAnimationState() const;
	AnimationState CalculateEffectAnimation() const;
	Rectf CalculateAttackHitbox() const;

	//Initialization
	void InitializeCollision(const Point2f& startPos);
	void InitializeSounds();

	//debugging
	void DrawHitboxes() const;

	//respawning
	void SoftRespawn();
	void HardRespawn();
	void HandleRespawning();
};