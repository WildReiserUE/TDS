// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDSInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTDSInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Metods
 */
class TDS_API ITDSInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	bool AviableForEffects();
};
