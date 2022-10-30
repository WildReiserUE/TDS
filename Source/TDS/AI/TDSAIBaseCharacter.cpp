// Created WildReiser ©2022


#include "TDSAIBaseCharacter.h"


// Sets default values
ATDSAIBaseCharacter::ATDSAIBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDSAIBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (NPCData.ComponentList.Num()>0)
	{
		const int Comp = NPCData.ComponentList.Num();
		for (int i = 0; i < Comp; i++){
			this->AddOwnedComponent(NPCData.ComponentList[i]);
		}
		OnComponentsAdded.Broadcast();
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO NPC --- %i"), NPCData.ComponentList.Num());
	}
}

// Called every frame
void ATDSAIBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDSAIBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

