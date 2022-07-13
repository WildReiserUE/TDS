#include "TDSPlayerController.h"

ATDSPlayerController::ATDSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ATDSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ATDSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ATDSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
}