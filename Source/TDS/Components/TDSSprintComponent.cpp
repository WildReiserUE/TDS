#include "TDSSprintComponent.h"

UTDSSprintComponent::UTDSSprintComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSSprintComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitSprint();	
}

void UTDSSprintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTDSSprintComponent::InitSprint()
{
	SprintPoint					= SprintSettings.SprintPoint;
	SprintLoseValue				= SprintSettings.SprintLosePoint;
	SprintRecoveryValue			= SprintSettings.SprintRecoveryValue;
	SprintRecoveryTimerStart	= SprintSettings.SprintRecoveryTimerStart;
	SprintTimerTick				= SprintSettings.SprintTimerTick;
}

void UTDSSprintComponent::StartSprint()
{
	OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
	if(GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoveryTimer))
		GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);	
	GetWorld()->GetTimerManager().SetTimer(StaminaLoseTimer,this, &UTDSSprintComponent::DecreaseStamina,SprintTimerTick, true,0.0f);
}

void UTDSSprintComponent::StopSprint()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(StaminaLoseTimer))
		GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryTimer,this,&UTDSSprintComponent::IncreaseStamina,SprintTimerTick,true,SprintSettings.SprintRecoveryTimerStart);
}

void UTDSSprintComponent::DecreaseStamina()
{
	if(SprintPoint>=SprintSettings.SprintLosePoint)
	{
		SprintPoint -= SprintLoseValue;
		if(SprintPoint>=SprintSettings.SprintPoint)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
			SprintPoint = SprintSettings.SprintPoint;
		}
		OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}

void UTDSSprintComponent::IncreaseStamina()
{
	if(SprintPoint<SprintSettings.SprintPoint)
	{
		SprintPoint += SprintRecoveryValue;
		if(SprintPoint>=SprintSettings.SprintPoint)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
			SprintPoint = SprintSettings.SprintPoint;
		}
		OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}