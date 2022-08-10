// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSprint, float, CurrentSprintValue, float, MaxSprintValue);

USTRUCT()
struct FSprintSettings
{
	GENERATED_BODY()
	float SprintPoint = 100.0f;
	float SprintLosePoint = 3.0f;
	float SprintRecoveryValue = 5.0f;
	float SprintRecoveryTimerStart = 2.0f;
	float SprintTimerTick = 0.2f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
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

	//Initialize sprint params
	UFUNCTION()
	void InitSprint();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void StartSprint();	
	void DecreaseStamina();
	void StopSprint();
	void IncreaseStamina();

	float SprintPoint;
	float SprintLoseValue;
	float SprintRecoveryValue;
	float SprintRecoveryTimerStart;
	float SprintTimerTick;
};
