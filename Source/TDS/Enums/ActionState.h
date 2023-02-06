#pragma once

UENUM (BlueprintType)
enum class EActionState: uint8
{
	DEFAULT,
	IDLE,
	WALK,
	RUN,
	SWIMMING,
	COMBAT_POSE,
	BUFF_POS,
	DISTANCE_ATTACK,
	MELLEE_ATTACK
};