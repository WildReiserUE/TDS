// Fill out your copyright notice in the Description page of Project Settings.


#include "TDSHealthComponent.h"

#include "TDSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTDSHealthComponent::UTDSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTDSHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	const auto ComponentOwner = GetOwner();
	if (!ComponentOwner) return;

	ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTDSHealthComponent::TakeAnyDamage);
	InitParams();
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
			OnShieldChange.Broadcast(Shield,PlayerHealthSettings.Shield);
			ShieldRecovery();
		}
		else if(Shield + Value < 0)
		{
			Shield = 0;
			Health = Health + Shield + Value;
			OnShieldChange.Broadcast(Shield,PlayerHealthSettings.Shield);
			OnHealthChange.Broadcast(Health, PlayerHealthSettings.Health);
			//ShieldRecovery();
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("SHIELD: %f"), Shield));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
	}
	else //Healing or some else
	{

	}
	
}

void UTDSHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	static ATDSCharacter* PlayerInstig = Cast<ATDSCharacter>(InstigatedBy);
	static ATDSCharacter* PlayerCauser = Cast<ATDSCharacter>(DamageCauser);
	
	if(PlayerInstig || PlayerCauser) return;	
	HealthChange(-Damage);
}

void UTDSHealthComponent::ShieldRecovery()
{
	if(!GetWorld()->GetTimerManager().IsTimerActive(ShieldRecoveryTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(ShieldRecoveryTimer, this, &UTDSHealthComponent::ShieldRecoveryStart, PlayerHealthSettings.ShieldRecoveryTick, true,PlayerHealthSettings.ShieldStartDelay);
	}
}

void UTDSHealthComponent::ShieldRecoveryStart()
{
	if(Shield<PlayerHealthSettings.Shield)
	{
		Shield+=PlayerHealthSettings.ShieldRecoveryValue;
		OnShieldChange.Broadcast(Shield,PlayerHealthSettings.Shield);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShieldRecoveryTimer);
	}
}

void UTDSHealthComponent::InitParams()
{
	Health = PlayerHealthSettings.Health;
	Shield = PlayerHealthSettings.Shield;
}