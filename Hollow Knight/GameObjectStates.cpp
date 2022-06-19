#include "GameObjectStates.h"


//Player

PlayerStates::PlayerStates()
	:action{ MovementState::idle },
	moveDir{ MoveDirection::none },
	lookDir{ LookDirection::left },
	lookDirHor{ LookDirection::left },
	attackDir{ AttackDirection::horizontal },
	isAttacking{ false },
	isOnGround{ true },
	isInvincible{ false },
	isRespawning{false}
{}

PlayerStates::PlayerStates(MovementState movementState, MoveDirection moveDirection, LookDirection lookDirection, AttackDirection attack, bool isAttack, bool isGround)
	:action{ movementState },
	moveDir{ moveDirection },
	lookDir{ lookDirection },
	lookDirHor{lookDirection},
	attackDir{ attack },
	isAttacking{ isAttack },
	isOnGround{ isGround },
	isInvincible{false},
	isRespawning{false}
{}




