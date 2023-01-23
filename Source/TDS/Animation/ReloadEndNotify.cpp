// Created WildReiser @2022


#include "ReloadEndNotify.h"


void UReloadEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnNotified.Broadcast(MeshComp);
}