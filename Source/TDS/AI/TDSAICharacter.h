// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/Character.h"
#include "TDSAICharacter.generated.h"

UCLASS()
class TDS_API ATDSAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ATDSAICharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UNavigationInvokerComponent* Navigator;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
