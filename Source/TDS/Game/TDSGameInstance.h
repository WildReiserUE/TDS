// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "TDSGameInstance.generated.h"

UENUM (BlueprintType)
enum class ECharacterState: uint8
{
	Default,
	Idle,
	Walk,
	Run,
	Swimming,
	CombatPose,
	BuffPose,
	DistanceAttack,
	MeleeAttack
};

UENUM (BlueprintType)
enum class ECharacterProfession: uint8
{
	Default,
	Mystic,
	Warrior
};

UENUM (BlueprintType)
enum class ECharacterSex: uint8
{
	Default,
	Mann,
	Woman
};

UCLASS()
class TDS_API UTDSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* ItemTable = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* WeaponTable = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* ArmorTable = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* ProjectileTable = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* AIPresetTable = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UDataTable* BasePlayerPresetTable = nullptr;
};
