#pragma once

#include "CoreMinimal.h"
#include "TDSStateEffect.generated.h"

UCLASS()
class TDS_API UTDSStateEffect : public UObject
{
	GENERATED_BODY()
	
public:

	virtual bool InitObject(APawn* Pawn);
	virtual void ExecuteEffect(float DeltaTime);
	virtual void DestroyObject();
	
};