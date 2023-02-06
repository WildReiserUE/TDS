// Created WildReiser @2022

#pragma once

#include "CoreMinimal.h"
#include "TDSGameInstance.h"
#include "TDSInventoryComponent.h"
#include "TDSSkillComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "TDSHealthComponent.h"
#include "Enums/Race.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentAddComplete);

USTRUCT(BlueprintType)
struct FBaseData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CharacterID = INDEX_NONE;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsPlayer = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString PlayerMail = FString("example@example.ru");
};

USTRUCT(BlueprintType)
struct FPlayerSettings : public FBaseData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition = "bIsPlayer == true", EditConditionHides))
	float CameraChangeStep = 75.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition = "bIsPlayer == true", EditConditionHides))
	float CameraMinLenght = 300.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition = "bIsPlayer == true", EditConditionHides))
	float CameraMaxLenght = 1800.f;
};

USTRUCT(BlueprintType)
struct FBaseHumanoidData : public FPlayerSettings//, public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName NPC_Id = FName("NoName");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Humanoid_Name = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECharacterSex CharacterSex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ERace CharacterRace;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECharacterProfession CharacterProfession;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* Humanoid_Mesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Humanoid_AnimInstance = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UActorComponent>> ComponentList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0"))
	float WalkSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0"))
	float RunSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0"))
	float AimMoveSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FHealthParams HealthParams;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSkillParams SkillParams;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0"))
	int CurrentLevel = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0"))
	int CurrentExperience = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> MontageHandleAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> MontageDead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EWeaponClass, UAnimMontage*> WeaponMontageShotingMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EWeaponClass, UAnimMontage*> WeaponMontageReloadMap;
};

UCLASS()
class TDS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	FBaseData BaseInfo;
	FPlayerSettings PlayerSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Humanoid Settings")
	FBaseHumanoidData CharacterInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnedName", meta=(ExposeOnSpawn))
	FName SpawnedName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	ATDSItemBase* CurrentWeapon = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnComponentAddComplete CompleteAddComponent;

// #if WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
// #endif

	void ChangeSettings();
	virtual void AttackOn();
	virtual void AttackOff();
	virtual void LaunchTimer(FTimerHandle &Timer);

	UTDSInventoryComponent* GetInventoryComponent();
	UTDSSkillComponent* GetSkillComponent();
	UTDSHealthComponent* GetHealthComponent();
	UTDSGameInstance* GetTDSGameInstance();

	UFUNCTION()
	void DeadEvent(AActor* Killer);
	UFUNCTION()
	void StartRagdoll();
	FTimerHandle DeadTimer;
	bool bIsALife = true;
};
