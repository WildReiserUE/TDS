#include "TDSPlayerController.h"

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