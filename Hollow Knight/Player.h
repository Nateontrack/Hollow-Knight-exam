#pragma once
#include "GameObjectStates.h"
#include "Spritesheet.h"
#include "structs.h"
#include "Vector2f.h"
class Player
{
public:
	Player(const Point2f& startPos, float width, float height);
	
	void Update(float elapsedSec);
	void Draw() const;
	void HandleKeyDown(const SDL_KeyboardEvent& e);

	
	Point2f GetCenterPos() const;
	void SetIsOnGround(bool isOnGround);

	//vars
	Vector2f m_Velocity;
	Rectf m_Hitbox;

private:
	//states
	PlayerState m_ActionState;
	MoveDirection m_MoveDirState;
	LookDirection m_LookDirState;
	//physics
	const float m_WalkSpeed;
	const float m_TerminalVelocity;
	const float m_AirControl;
	const float m_JumpSpeed;
	const Vector2f m_Gravity;
	
	bool m_IsOnGround;
	//positioning
	
	//animation
	Spritesheet m_Animations;

	//Functionality
	//main loop
	void DrawAnimation() const;
	void UpdateAnimation(float elapsedSec);

	void HandleKeyboardState();
	void CalculateVelocity(float elapsedSec);
	void MovePlayer(float elapsedSec);

	//Getters & Setters
	AnimationState CalculateAnimationState() const;
};

