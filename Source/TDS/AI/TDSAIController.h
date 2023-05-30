// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TDSAIController.generated.h"

UCLASS()
class TDS_API ATDSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATDSAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sensing")
	UAIPerceptionComponent* AiPerception;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sensing")
	UAISenseConfig_Sight* AiConfigSight;

	UPROPERTY()
	APawn* ControlledPawn = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Behavior")
	UBehaviorTree* BehaviorTree = nullptr;

	virtual void OnPossess(APawn* InPawn) override;
};
