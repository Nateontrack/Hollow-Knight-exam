#pragma once
#include "Vector2f.h"

enum class AnimationState
{
	idle,
	run,
	jump,
	fall,
	attackHor,
	slashEffect,
	attackUp,
	attackDown,
	deathAir,
	deathGround,
	turn,
	hit,
	invincible
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

enum class MovementState
{
	idle,
	run,
	jump,
	fall,
	attack,
	damaged
};

enum class AttackDirection
{
	horizontal,
	up,
	down
};

enum class CrawlidState
{
	walk,
	turn,
	deadAir,
	deadGround
};

struct PlayerStates
{
	PlayerStates();
	explicit PlayerStates(MovementState movementState, MoveDirection moveDirection, LookDirection lookDirection, AttackDirection attack, bool isAttack, bool isGround);

	MovementState action;
	MoveDirection moveDir;
	LookDirection lookDir;
	AttackDirection attackDir;
	bool isOnGround;
	bool isAttacking;
	bool isInvincible;
};

enum class GameObjectType
{
	Platform,
	Spike,
	Crawlid
};

struct CollisionFunc
{
	Rectf combatHitbox;
	Rectf movementHitbox;
	Rectf attackHitbox;
	Vector2f velocity;
};