// Created WildReiser ©2022

#include "TDSGameMode.h"
#include "TDSPlayerController.h"
#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATDSGameMode::ATDSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATDSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	 static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("DefaultPawn'/Game/TDS/Blueprints/Character/BP_TDSCharacter'"));
	 if (PlayerPawnBPClass.Class != nullptr){
	 	DefaultPawnClass = PlayerPawnBPClass.Class;
	 }
}