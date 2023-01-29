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

	UPROPERTY(VisibleAnywhere)
	UNavigationInvokerComponent* Navigator;
};
