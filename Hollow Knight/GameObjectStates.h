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
	invincible,
	spikeDeath,
	still,
	dash,
	dashEffect
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
	death,
	spikeDeath,
	damaged,
	dash
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
	LookDirection lookDirHor;
	AttackDirection attackDir;
	bool isOnGround;
	bool isAttacking;
	bool isInvincible;
	bool isRespawning;
};

enum class GameObjectType
{
	platform,
	spike,
	crawlid,
	breakable,
	powerUp
};

enum class BreakableState
{
	whole,
	breaking,
	broken
};

enum class BreakableType
{
	spikeA,
	spikeB,
	doorway
};

enum class PlatformType
{
	platform01 = 1, //width:128	height:45
	platform02 = 2, //width:84	height:45
	platform03 = 3, //width:79  height:45
	platform04 = 4,	//width:67  height:45
	platform05 = 5,	//width:127	height:45
	platform06 = 6,	//width:100	height:152
	platform07 = 7,	//width:195	height:54
	platform08 = 8,	//width:100	height:100
	platform09 = 9,	//width:100	height:100
};

struct CollisionFunc
{
	Rectf combatHitbox;
	Rectf movementHitbox;
	Rectf attackHitbox;
	Vector2f velocity;
};

enum class HealthState
{
	on,
	breaking,
	off
};