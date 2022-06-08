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
	//void HandleKeyDown(const SDL_KeyboardEvent& e);

	void ResetAnimations();

	//getters & setters
	Point2f GetCenterPos() const;
	PlayerStates GetState() const;
	void SetIsOnGround(bool isOnGround);


	//vars
	Vector2f m_Velocity;
	Rectf m_CombatHitbox;
	Rectf m_CollisionHitbox;

private:
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

	//Initialization
	void InitializeHitboxes(const Point2f& startPos);

	//debugging
	void DrawHitboxes() const;
};

