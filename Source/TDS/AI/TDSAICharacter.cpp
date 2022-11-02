// Created WildReiser ©2022


#include "TDSAICharacter.h"


ATDSAICharacter::ATDSAICharacter()
{
	Navigator = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigator"));
	Navigator->SetGenerationRadii(3000.f,3300.f);
	PrimaryActorTick.bCanEverTick = true;

}

void ATDSAICharacter::BeginPlay()
{
	Super::BeginPlay();
	ChangeSettings();  //apply datatable info
	if (CharacterInfo.ComponentList.Num()>0)
	{
		const int Comp = CharacterInfo.ComponentList.Num();
		for (int i = 0; i < Comp; i++)
		{
			this->AddComponentByClass(CharacterInfo.ComponentList[i],false, FTransform(FVector(0)),true);
			FinishAddComponent(GetComponentByClass(CharacterInfo.ComponentList[i]),false,FTransform(FVector(0)));
		}
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO NPC --- %i"), CharacterInfo.ComponentList.Num());
		OnComponentsAdded.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO NPC --- HOJlb"));
	}
}

void ATDSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATDSAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

