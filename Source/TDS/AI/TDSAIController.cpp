// Created WildReiser ©2022


#include "TDSAIController.h"

#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


ATDSAIController::ATDSAIController()
{
	AiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AiPerception"));
	AiConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AiConfigSight"));

	if (AiPerception)
	{
		// The following corresponds to the setting of sound perception components in the blueprint
		AiConfigSight->SightRadius = 1000.0f;
		AiConfigSight->LoseSightRadius = 2000.0f;
		AiConfigSight->PeripheralVisionAngleDegrees = 90.0f;
		AiConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
		AiConfigSight->DetectionByAffiliation.bDetectEnemies = true;
		AiConfigSight->DetectionByAffiliation.bDetectFriendlies = true;

		AiPerception->ConfigureSense(*AiConfigSight);
		AiPerception->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	}
}

void ATDSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(InPawn->IsA<ABaseCharacter>())
	{
		ControlledPawn = InPawn;
		UE_LOG(LogTemp,Log,TEXT("POSSES IN PAWN"));
		if(BehaviorTree)
		{
			RunBehaviorTree(BehaviorTree);
			GetBlackboardComponent()->SetValueAsVector(FName("HomeLocation"),ControlledPawn->GetActorLocation());
		}
	}
}
