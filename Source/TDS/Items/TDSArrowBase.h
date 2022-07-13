// Created WildReiser @2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDSArrowBase.generated.h"

UCLASS()
class TDS_API ATDSArrowBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDSArrowBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};