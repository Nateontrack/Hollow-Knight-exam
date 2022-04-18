#pragma once
enum class AnimationState
{
	idle,
	run,
	jump,
	fall,
	attackHor,
	attackUp,
	attackDown,
	deathAir,
	deathGround,
	turn
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

enum class CrawlidState
{
	walk,
	turn,
	deadAir,
	deadGround
};