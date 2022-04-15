#pragma once
enum class AnimationState
{
	idle,
	run,
	jump,
	fall,
	attackHor,
	attackUp,
	attackDown
};

enum class MoveDirection
{
	left,
	right,
	none
};

enum class LookDirection
{
	left,
	right,
	up,
	down
};

enum class PlayerState
{
	idle,
	run,
	jump,
	fall,
	attack
};