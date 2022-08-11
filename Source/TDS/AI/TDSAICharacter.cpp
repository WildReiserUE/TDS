// Created WildReiser @2022


#include "TDSAICharacter.h"

// Sets default values
ATDSAICharacter::ATDSAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw=false;
	NetDormancy=ENetDormancy::DORM_Initial;
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void ATDSAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDSAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

