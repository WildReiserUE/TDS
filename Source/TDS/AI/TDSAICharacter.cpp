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
}

void ATDSAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATDSAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

