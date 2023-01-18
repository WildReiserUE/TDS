// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, MaxShield);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOwnerDeath);

USTRUCT(BlueprintType)
struct FHealthSettings //Структура с настройками нужно перенести куда-нить
{
	GENERATED_BODY()
	float Health = 1000.0f;
	float Shield = 1000.0f;
	float ShieldStartDelay = 2.0f;
	float ShieldRecoveryValue = 1.0f;
	float ShieldRecoveryTick = 0.1f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSHealthComponent();
	void InitParams();

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnShieldChange OnShieldChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnOwnerDeath OnOwnerDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FHealthSettings HealthSettings;

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
	float Health;
	float Shield;
	float ShieldStartDelay;
	float ShieldRecoveryValue;
	float ShieldRecoveryTick;
};
