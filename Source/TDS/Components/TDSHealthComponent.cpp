// Created WildReiser ©2022

#include "TDSHealthComponent.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UTDSHealthComponent::UTDSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSHealthComponent::InitParams(int32 Health, int32 Shield, float ShieldDelay, float ShieldRecValue, float ShieldRecTick)
{
	MaxCHealth = CHealth = Health;
	MaxCShield = CShield = Shield;

	UE_LOG(LogTemp, Log, TEXT("--- COMPONENT HEALTH --- %f"), MaxCHealth);
	UE_LOG(LogTemp, Log, TEXT("--- COMPONENT SHIELD --- %f"), MaxCShield);
	CShieldStartDelay = ShieldDelay;
	CShieldRecoveryValue = ShieldRecValue;
	CShieldRecoveryTick = ShieldRecTick;
}

void UTDSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!ComponentOwner()) return;
	UE_LOG(LogTemp, Log, TEXT("HEALTH COMPONENT OWNER --- %s"), *ComponentOwner()->GetName());
	ComponentOwner()->OnTakeAnyDamage.AddDynamic(this, &UTDSHealthComponent::TakeAnyDamage);
}

void UTDSHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTDSHealthComponent::HealthChange(float Value)
{
	if (Value < 0)
	{
		if (CShield + Value >= 0)
		{
			CShield += Value;
			OnShieldChange.Broadcast(CShield, MaxCShield);
			ShieldRecovery();
		}
		else if (CShield + Value < 0)
		{
			CShield = 0;
			CHealth = CHealth + CShield + Value;
			OnShieldChange.Broadcast(CShield, MaxCShield);
			OnHealthChange.Broadcast(CHealth, MaxCHealth);
			if (CHealth <= 0)
			{
				CHealth = 0;
				GetOwner()->SetActorEnableCollision(ECollisionResponse::ECR_Ignore);
				if (GetWorld()->GetTimerManager().IsTimerActive(ShieldRecoveryTimer))
					GetWorld()->GetTimerManager().ClearTimer(ShieldRecoveryTimer);
				OnOwnerDeath.Broadcast();
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("SHIELD: %f"), CShield));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), CHealth));
	}
	else
	{
		//Healing or some else if -Damage
	}
}

void UTDSHealthComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                        AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("%s Recive %f DAMAGE FROM  %s"), *ComponentOwner()->GetName(), Damage,
	       *DamageCauser->GetName());
	HealthChange(-Damage);
}

void UTDSHealthComponent::ShieldRecovery()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(ShieldRecoveryTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(ShieldRecoveryTimer, this, &UTDSHealthComponent::ShieldRecoveryStart,
		                                       CShieldRecoveryTick, true,
		                                       CShieldStartDelay);
	}
}

void UTDSHealthComponent::ShieldRecoveryStart()
{
	if (CShield < MaxCShield)
	{
		CShield += CShieldRecoveryValue;
		OnShieldChange.Broadcast(CShield, MaxCShield);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(ShieldRecoveryTimer);
	}
}

AActor* UTDSHealthComponent::ComponentOwner()
{
	auto ComponentOwner = this->GetOwner();
	return ComponentOwner ? ComponentOwner : nullptr;
}
