// Created WildReiser @2022


#include "ReloadEndNotify.h"


void UReloadEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	OnNotified.Broadcast(MeshComp);
}