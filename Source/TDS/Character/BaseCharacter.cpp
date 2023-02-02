// Created WildReiser @2022

#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(GetTDSGameInstance())
	{
		FBaseHumanoidData* PlayerPresetRow = GetTDSGameInstance()->BasePlayerPresetTable->FindRow<FBaseHumanoidData>(SpawnedName, "", true);
		FBaseHumanoidData* AIPresetRow = GetTDSGameInstance()->AIPresetTable->FindRow<FBaseHumanoidData>(SpawnedName, "", true);
		if (PlayerPresetRow)
		{
			CharacterInfo = *PlayerPresetRow;
			ChangeSettings();
		}
		else if(AIPresetRow)
		{
			CharacterInfo = *AIPresetRow;
			ChangeSettings();
		}
	}
}

// #if WITH_EDITOR
// void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
// 	ChangeSettings();
// }
// #endif

void ABaseCharacter::ChangeSettings()
{
	if (CharacterInfo.Humanoid_Mesh)
		GetMesh()->SetSkeletalMesh(CharacterInfo.Humanoid_Mesh);
	else
		GetMesh()->SetSkeletalMesh(nullptr);

	if (CharacterInfo.Humanoid_AnimInstance)
		GetMesh()->SetAnimInstanceClass(CharacterInfo.Humanoid_AnimInstance);
	else
		GetMesh()->SetAnimInstanceClass(nullptr);

	if(CharacterInfo.ComponentList.Num() > 0)
	{
		for(int comp = 0 ; comp <= CharacterInfo.ComponentList.Num() - 1 ; comp++) 
		{
			FTransform ComponentTransform = FTransform(FRotator(0),FVector(0),FVector(1));
			GetOwner()->AddComponentByClass(CharacterInfo.ComponentList[comp],false, ComponentTransform,true);
		}
		//TODO: INITIALIZE COMPONENTS PARAMS
		ComponentsAdded.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("BASE CHAR DELEGATE --- TOTAL ADDED COMPONENTS = %i"), CharacterInfo.ComponentList.Num());
	}
}

void ABaseCharacter::AttackOn()
{
}

void ABaseCharacter::AttackOff()
{
}

void ABaseCharacter::LaunchTimer(FTimerHandle &Timer)
{
}

UTDSInventoryComponent* ABaseCharacter::GetInventoryComponent()
{
	UTDSInventoryComponent* Inventory = FindComponentByClass<UTDSInventoryComponent>();
	return Inventory ? Inventory : nullptr;
}

UTDSSkillComponent* ABaseCharacter::GetSkillComponent()
{
	UTDSSkillComponent* SkillComponent = FindComponentByClass<UTDSSkillComponent>();
	return SkillComponent ? SkillComponent : nullptr;
}

UTDSHealthComponent* ABaseCharacter::GetHealthComponent()
{
	UTDSHealthComponent* HealthComponent = FindComponentByClass<UTDSHealthComponent>();
	return HealthComponent ? HealthComponent : nullptr;
}

UTDSGameInstance* ABaseCharacter::GetTDSGameInstance()
{
	UTDSGameInstance* Instance = GetGameInstance<UTDSGameInstance>();
	return Instance ? Instance : nullptr;
}

void ABaseCharacter::DeadEvent()
{
	bIsALife = false;
	if(CharacterInfo.MontageDead.Num() > 0)
	{
		int32 RND_Montage = UKismetMathLibrary::RandomIntegerInRange(0,CharacterInfo.MontageDead.Num()-1);
		GetMesh()->GetAnimInstance()->Montage_Play(CharacterInfo.MontageDead[RND_Montage]);
		float EndPos = CharacterInfo.MontageDead[RND_Montage]->CalculateSequenceLength();
		DetachFromControllerPendingDestroy();
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
		SetActorEnableCollision(true);

		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;

		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		SetLifeSpan(EndPos + 3.f);
	}
}
