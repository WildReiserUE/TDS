// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, MaxShield);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOwnerDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSHealthComponent();
	void InitParams(int32 Health, int32 Shield, float ShieldDelay, float ShieldRecValue, float ShieldRecTick);

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnShieldChange OnShieldChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnOwnerDeath OnOwnerDeath;

	FTimerHandle ShieldRecoveryTimer;

	UFUNCTION()
	void HealthChange(float Value);

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                   class AController* InstigatedBy, AActor* DamageCauser);
	void ShieldRecovery();
	void ShieldRecoveryStart();
	AActor* ComponentOwner();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	float CHealth = 0.f;
	float MaxCHealth = 0.f;

	float CShield = 0.f;
	float MaxCShield = 0.f;

	float CShieldStartDelay = 0.f;

	float CShieldRecoveryValue = 0.f;

	float CShieldRecoveryTick = 0.f;

};
