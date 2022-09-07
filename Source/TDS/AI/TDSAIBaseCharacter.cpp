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
	if (ComponentList.Num()>0)
	{
		const int Comp = ComponentList.Num();
		for (int i = 0; i < Comp; i++){
			this->AddComponentByClass(ComponentList[i], false, FTransform(FVector(0)),false);
		}
		OnComponentsAdded.Broadcast();
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

