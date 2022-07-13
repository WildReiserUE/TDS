#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, MaxShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIDeath);

USTRUCT(BlueprintType)
struct FHealthSettings
{
	GENERATED_BODY()
	float Health = 100.0f;
	float Shield = 50.0f;
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

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnHealthChange OnHealthChange;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnShieldChange OnShieldChange;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnAIDeath OnAIDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health, meta = (AllowPrivateAccess = "true"))
	FHealthSettings PlayerHealthSettings;

	FTimerHandle ShieldRecoveryTimer;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void HealthChange(float Value);
	
	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	void ShieldRecovery();
	void ShieldRecoveryStart();
	void InitParams();

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	float Shield;
	float ShieldStartDelay;
	float ShieldRecoveryValue;
	float ShieldRecoveryTick;
};