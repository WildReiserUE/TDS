// Created WildReiser ©2022

#include "TDSPlayerController.h"

#include "BaseCharacter.h"

ATDSPlayerController::ATDSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ATDSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ATDSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATDSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(InPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("POSSES CPP --- %s --- "), *InPawn->GetName());
	}
}

void ATDSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTemp, Log, TEXT("UNPOSSES CPP --- --- --- "));
}
