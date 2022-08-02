// Created WildReiser @2022

#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TDSProjectileBase.generated.h"

UCLASS()
class TDS_API ATDSProjectileBase : public ATDSItemBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATDSProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;	
};