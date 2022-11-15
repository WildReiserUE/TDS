// Created WildReiser ©2022

#include "TDSHealthComponent.h"

#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"

UTDSHealthComponent::UTDSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSHealthComponent::BeginPlay(){
	Super::BeginPlay();
	
	if (!ComponentOwner()) return;
	UE_LOG(LogTemp, Log, TEXT("HEALTH COMPONENT OWNER --- %s"), *ComponentOwner()->GetName());
	ComponentOwner()->OnTakeAnyDamage.AddDynamic(this, &UTDSHealthComponent::TakeAnyDamage);
	InitParams();
}

void UTDSHealthComponent::InitParams()
{
	Health = HealthSettings.Health;
	Shield = HealthSettings.Shield;
}

void UTDSHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTDSHealthComponent::HealthChange(float Value)
{
	if(Value < 0 )
	{
		if(Shield + Value >= 0)
		{
			Shield+=Value;
			OnShieldChange.Broadcast(Shield,HealthSettings.Shield);
			ShieldRecovery();
		}
		else if(Shield + Value < 0)
		{
			Shield = 0;
			Health = Health + Shield + Value;
			OnShieldChange.Broadcast(Shield,HealthSettings.Shield);
			OnHealthChange.Broadcast(Health, HealthSettings.Health);
			if(Health <= 0)
			{
				Health = 0;
				UE_LOG(LogViewport, Display, TEXT("Player is Dead"));
				GetOwner()->SetActorEnableCollision(ECollisionResponse::ECR_Ignore);
				if(GetWorld()->GetTimerManager().IsTimerActive(ShieldRecoveryTimer))
					GetWorld()->GetTimerManager().ClearTimer(ShieldRecoveryTimer);
				OnOwnerDeath.Broadcast();
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("SHIELD: %f"), Shield));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
	}
	else
	{ //Healing or some else if -Damage
	
	}
}

void UTDSHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{	
	UE_LOG(LogTemp, Log, TEXT("%s Recive %f DAMAGE FROM  %s"), *ComponentOwner()->GetName(), Damage, *DamageCauser->GetName());
	HealthChange(-Damage);
}

void UTDSHealthComponent::ShieldRecovery()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(ShieldRecoveryTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(ShieldRecoveryTimer, this, &UTDSHealthComponent::ShieldRecoveryStart, HealthSettings.ShieldRecoveryTick, true,HealthSettings.ShieldStartDelay);
	}
}

void UTDSHealthComponent::ShieldRecoveryStart()
{
	if(Shield<HealthSettings.Shield)
	{
		Shield+=HealthSettings.ShieldRecoveryValue;
		OnShieldChange.Broadcast(Shield,HealthSettings.Shield);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShieldRecoveryTimer);
	}
}

AActor* UTDSHealthComponent::ComponentOwner()
{
	auto ComponentOwner = this->GetOwner();
	return ComponentOwner ? (ComponentOwner) : nullptr;
}