// Created WildReiser ©2022


#include "TDSAIController.h"


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
