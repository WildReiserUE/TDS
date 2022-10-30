// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDSAIBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIComponentsAdded);

USTRUCT(BlueprintType)
struct FNPCData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName NPC_Id;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText NPC_Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* NPC_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> NPC_AnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UActorComponent*> ComponentList;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float Mana;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bCanUseShield = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(EditCondition="bCanUseShield == false", EditConditionHides))
	float MaxShield;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float ShieldRecoveryValue;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float SheildRecoveryPeriod;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int Experience;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<UAnimMontage*>AnimMontageArray;
};

UCLASS()
class TDS_API ATDSAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDSAIBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnAIComponentsAdded OnComponentsAdded;
private:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn,AllowPrivateAccess="True"))
	FNPCData NPCData;
};
