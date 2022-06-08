#include "GameObjectStates.h"


//Player

PlayerStates::PlayerStates()
	:action{ MovementState::idle },
	moveDir{ MoveDirection::none },
	lookDir{ LookDirection::left },
	isAttacking{ false },
	isOnGround{ true }
{}

PlayerStates::PlayerStates(MovementState movementState, MoveDirection moveDirection, LookDirection lookDirection, bool isAttack, bool isGround)
	:action{ movementState },
	moveDir{ moveDirection },
	lookDir{ lookDirection },
	isAttacking{ isAttack },
	isOnGround{ isGround }
{}




