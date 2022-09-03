#include "TDSSkillComponent.h"

#include "TDSCharacter.h"

UTDSSkillComponent::UTDSSkillComponent(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSSkillComponent::BeginPlay(){
	Super::BeginPlay();
	auto ComponentOwner = GetOwner();
	if (!ComponentOwner) return;
	InitSprint();	
}

void UTDSSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//CalculateAllowSprint();
}

void UTDSSkillComponent::InitSprint(){
	SprintPoint					= SprintSettings.SprintPoint;
	SprintLoseValue				= SprintSettings.SprintLosePoint;
	SprintRecoveryValue			= SprintSettings.SprintRecoveryValue;
	SprintRecoveryTimerStart	= SprintSettings.SprintRecoveryTimerStart;
	SprintTimerTick				= SprintSettings.SprintTimerTick;
	SprintCoef					= SprintSettings.SprintCoef;
}

void UTDSSkillComponent::StartSprint(){
	auto ComponentOwner = GetOwner();
	if (ComponentOwner){
		OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
		if(GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoveryTimer))
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);	
		GetWorld()->GetTimerManager().SetTimer(StaminaLoseTimer,this, &UTDSSkillComponent::DecreaseStamina,SprintTimerTick, true,0.0f);
		if(ComponentOwner->IsA(ATDSCharacter::StaticClass())){
			UE_LOG(LogTemp,Warning,TEXT("SprintActivated"));
		}
	}
}

void UTDSSkillComponent::StopSprint(){
	auto ComponentOwner = GetOwner();
	if(GetWorld()->GetTimerManager().IsTimerActive(StaminaLoseTimer))
		GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRecoveryTimer,this,&UTDSSkillComponent::IncreaseStamina,SprintTimerTick,true,SprintSettings.SprintRecoveryTimerStart);
	if(ComponentOwner->IsA(ATDSCharacter::StaticClass())){
		UE_LOG(LogTemp,Warning,TEXT("SprintDEActivated"));
	}
}

void UTDSSkillComponent::DecreaseStamina(){
	if(SprintPoint>=SprintSettings.SprintLosePoint){
		SprintPoint -= SprintLoseValue;
		if(SprintPoint>=SprintSettings.SprintPoint){
			GetWorld()->GetTimerManager().ClearTimer(StaminaLoseTimer);
			SprintPoint = SprintSettings.SprintPoint;
		}
		OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}

void UTDSSkillComponent::IncreaseStamina(){
	if(SprintPoint<SprintSettings.SprintPoint){
		SprintPoint += SprintRecoveryValue;
		if(SprintPoint>=SprintSettings.SprintPoint){
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimer);
			SprintPoint = SprintSettings.SprintPoint;
		}
		OnSprintValueChange.Broadcast(SprintPoint, SprintSettings.SprintPoint);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("STAMINA: %f"), SprintPoint));
	}
}