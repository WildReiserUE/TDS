// Created WildReiser @2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "BaseAnimNotify.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent* );

UCLASS()
class TDS_API UBaseAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	FOnNotifiedSignature OnNotified;
};
