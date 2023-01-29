// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSprint, float, CurrentSprintValue, float, MaxSprintValue);

USTRUCT(BlueprintType)
struct FSprintSettings //Структура с настройками нужно перенести куда-нить
{
	GENERATED_BODY()
	float SprintPoint = 100.0f;
	float SprintCoef = 1.5f;
	float SprintLosePoint = 3.0f;
	float SprintRecoveryValue = 5.0f;
	float SprintRecoveryTimerStart = 2.0f;
	float SprintTimerTick = 0.2f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSSkillComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Sprint")
	FOnSprint OnSprintValueChange;

	FSprintSettings SprintSettings;

	FTimerHandle StaminaRecoveryTimer;
	FTimerHandle StaminaLoseTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Sprint")
	bool bCanSprint = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintReadWrite, Category="Sprint")
	bool bSprintStart = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	AActor* ComponentOwner();
	void InitSprint();
	void StartSprint();
	void DecreaseStamina();
	void StopSprint();
	void IncreaseStamina();

	float SprintPoint = 0.f;
	float SprintLoseValue = 0.f;
	float SprintRecoveryValue = 0.f;
	float SprintRecoveryTimerStart = 0.f;
	float SprintTimerTick = 0.f;
	float SprintCoef = 0.f;

protected:
	virtual void BeginPlay() override;
};
