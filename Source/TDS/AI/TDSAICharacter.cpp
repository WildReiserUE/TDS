// Created WildReiser ©2022


#include "TDSAICharacter.h"


ATDSAICharacter::ATDSAICharacter()
{
	Navigator = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("Navigator"));
	Navigator->SetGenerationRadii(3000.f, 3100.f);
	PrimaryActorTick.bCanEverTick = true;
}

void ATDSAICharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetTDSGameInstance())
	{
		if(FBasePlayerData* AIPresetRow = GetTDSGameInstance()->AIPresetTable->FindRow<FBasePlayerData>(SpawnedName, "", true))
		{
			CharacterInfo = *AIPresetRow;
			ChangeSettings();
		}
	}
}
