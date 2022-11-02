// Created WildReiser @2022


#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

}

void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ChangeSettings();
}

void ABaseCharacter::ChangeSettings()
{
	if(CharacterInfo.Humanoid_Mesh)
		GetMesh()->SetSkeletalMesh(CharacterInfo.Humanoid_Mesh);
	else	
		GetMesh()->SetSkeletalMesh(nullptr);
	if(CharacterInfo.Humanoid_AnimInstance)
		GetMesh()->SetAnimInstanceClass(CharacterInfo.Humanoid_AnimInstance);
	else
		GetMesh()->SetAnimInstanceClass(nullptr);
}
