// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSprint, float, CurrentSprintValue, float, MaxSprintValue);

USTRUCT(BlueprintType)
struct FSkillParams
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float EnergyPoint = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintCoef = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintLosePoint = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintRecoveryValue = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintRecoveryTimerStart = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SprintTimerTick = 0.f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSSkillComponent();

	UPROPERTY(BlueprintAssignable, Category="Sprint")
	FOnSprint OnSprintValueChange;

	FTimerHandle StaminaRecoveryTimer;
	FTimerHandle StaminaLoseTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Sprint")
	bool bCanSprint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Sprint")
	bool bSprintStart = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	AActor* ComponentOwner();
	void InitSprint(FSkillParams SkillParams);
	void StartSprint();
	void DecreaseStamina();
	void StopSprint();
	void IncreaseStamina();

	float SprintPoint = 0.f;
	float CMaxSprintPoint = 0.f;
	float SprintLoseValue = 0.f;
	float SprintRecoveryValue = 0.f;
	float SprintRecoveryTimerStart = 0.f;
	float SprintTimerTick = 0.f;
	float SprintCoef = 0.f;
	bool CanSprint();

private:



protected:
	virtual void BeginPlay() override;

};
