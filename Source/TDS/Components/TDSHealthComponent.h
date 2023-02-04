// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSHealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FHealthParams
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Health = 0.f;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bCanUseShield = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float Shield = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float ShieldDelay = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float ShieldRecoveryValue = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float ShieldStartDelay = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(EditCondition="bCanUseShield == true"))
	float ShieldRecoveryTick = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, MaxShield);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwnerDeath, AActor*, Damaged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSHealthComponent();
	void InitParams(FHealthParams HealthParams);

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnShieldChange OnShieldChange;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FOnOwnerDeath OnOwnerDeath;

	FTimerHandle ShieldRecoveryTimer;

	UFUNCTION()
	void HealthChange(AActor* DamageOwner, float Value);

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
