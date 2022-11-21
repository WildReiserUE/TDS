// Created WildReiser @2022

#pragma once

#include "CoreMinimal.h"
#include "KismetAnimationLibrary.h"
#include "TDSInventory.h"
#include "TDSSkillComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentsAdded);

USTRUCT(BlueprintType)
struct FBaseData
{
	GENERATED_BODY()
	float CameraMaxLenght = 1500.f;
	float CameraMinLenght = 300.f;
	float CameraChangeStep = 75.f;	
};

USTRUCT(BlueprintType)
struct FBaseHumanoidData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName NPC_Id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Humanoid_Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* Humanoid_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Humanoid_AnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UActorComponent>> ComponentList;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	float Mana;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	float AimMoveSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bCanUseShield = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float MaxShield;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float ShieldRecoveryValue;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float SheildRecoveryPeriod;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(ClampMin="0"))
	int Experience;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<UAnimMontage*>MontageHandleAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*>MontageDead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EWeaponClass,UAnimMontage*> WeaponMontageShotingMap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EWeaponClass,UAnimMontage*> WeaponMontageReloadMap;
};

UCLASS()
class TDS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	FORCEINLINE class UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraArm; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	FBaseData BaseInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Humanoid Settings")
	FBaseHumanoidData CharacterInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpawnedName", meta=(ExposeOnSpawn))
	FName SpawnedName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	ATDSItemBase* CurrentWeapon = nullptr;

	UPROPERTY(BlueprintAssignable)
	FOnComponentsAdded ComponentsAdded;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	
#endif

	void ChangeSettings();
	virtual void FireOn();
	UTDSInventory* GetInventoryComp();
	UTDSSkillComponent* GetSkillComponent();
};
