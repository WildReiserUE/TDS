// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDSPlayerController.generated.h"

UCLASS()
class ATDSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDSPlayerController();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
