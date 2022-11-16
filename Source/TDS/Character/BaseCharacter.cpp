// Created WildReiser @2022


#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	ChangeSettings();
	if (CharacterInfo.ComponentList.Num()>0)
	{
		const int Comp = CharacterInfo.ComponentList.Num();
		for (int i = 0; i < Comp; i++)
		{
			this->AddComponentByClass(CharacterInfo.ComponentList[i], false, FTransform(FVector(0)),true);
			FinishAddComponent(GetComponentByClass(CharacterInfo.ComponentList[i]),false,FTransform(FVector(0)));
		}
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO OWNER --- %i"), CharacterInfo.ComponentList.Num());
		ComponentsAdded.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO OWNER --- HOJlb"));
	}
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

void ABaseCharacter::FireOn()
{	
}

UTDSInventory* ABaseCharacter::GetInventory()
{
	const auto Inventory = FindComponentByClass<UTDSInventory>();
	return Inventory ? (Inventory) : nullptr;
}

UTDSSkillComponent* ABaseCharacter::GetSkillComponent()
{
	const auto SkillComponent = FindComponentByClass<UTDSSkillComponent>();
	return SkillComponent ? (SkillComponent) : nullptr;
}