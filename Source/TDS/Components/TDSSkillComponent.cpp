// Created WildReiser ©2022

#include "TDSSkillComponent.h"

#include "PlayerCharacter.h"

UTDSSkillComponent::UTDSSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!ComponentOwner()) return;
	UE_LOG(LogTemp, Log, TEXT("SKILL COMPONENT OWNER --- %s"), *ComponentOwner()->GetName());
}

bool UTDSSkillComponent::CanSprint()
{
	if (ComponentOwner() && ComponentOwner()->IsA(ABaseCharacter::StaticClass()) && SprintPoint >= SprintLoseValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint AVAIBLE"));
        return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint UN---AVAIBLE"));
		return false;
	}
}

void UTDSSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UTDSSkillComponent::ComponentOwner()
{
	AActor* ComponentOwner = this->GetOwner();
	return ComponentOwner ? ComponentOwner : nullptr;
}

void UTDSSkillComponent::InitSprint(FSkillParams SkillParams)
{
	SprintPoint = CMaxSprintPoint = SkillParams.EnergyPoint; //init point as max point
	SprintLoseValue = SkillParams.SprintLosePoint;
	SprintRecoveryValue = SkillParams.SprintRecoveryValue;
	SprintRecoveryTimerStart = SkillParams.SprintRecoveryTimerStart;
	SprintTimerTick = SkillParams.SprintTimerTick;
	SprintCoef = SkillParams.SprintCoef;
}

void UTDSSkillComponent::StartSprint()
{
	if (CanSprint())
	{
		OnSprintValueChange.Broadcast(SprintPoint, CMaxSprintPoint);
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoveryTimer))
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
		GetWorld()->GetTimerManager().SetTimer(StaminaLoseTimer, this, &UTDSSkillComponent::DecreaseStamina,
		                                       SprintTimerTick, true, 0.0f);
		if (ComponentOwner()->IsA(APlayerCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("SprintActivated"));
		}
	}
}

void UTDSSkillComponent::StopSprint()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(StaminaLoseTimer))
		GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryTimer, this, &UTDSSkillComponent::IncreaseStamina,
	                                       SprintTimerTick, true, SprintRecoveryTimerStart);
	if (ComponentOwner()->IsA(APlayerCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintDEActivated"));
	}
}

void UTDSSkillComponent::DecreaseStamina()
{
	if (SprintPoint >= SprintLoseValue)
	{
		SprintPoint -= SprintLoseValue;
		if (SprintPoint <= 0.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
			SprintPoint = 0.f;
		}
		OnSprintValueChange.Broadcast(SprintPoint, CMaxSprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}

void UTDSSkillComponent::IncreaseStamina()
{
	if (SprintPoint < CMaxSprintPoint)
	{
		SprintPoint += SprintRecoveryValue;
		if (SprintPoint >= CMaxSprintPoint)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
			SprintPoint = CMaxSprintPoint;
		}
		OnSprintValueChange.Broadcast(SprintPoint, CMaxSprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}
