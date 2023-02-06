// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "TDSGameInstance.generated.h"

UENUM (BlueprintType)
enum class EPlayerProfession: uint8
{
	Default,
	Mystic,
	Warrior
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
