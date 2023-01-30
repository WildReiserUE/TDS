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
	ChangeSettings();
	if (CharacterInfo.ComponentList.Num() > 0)
	{
		const int Comp = CharacterInfo.ComponentList.Num();
		for (int i = 0; i < Comp; i++)
		{
			this->AddComponentByClass(CharacterInfo.ComponentList[i], false, FTransform(FVector(0)), true);
			FinishAddComponent(GetComponentByClass(CharacterInfo.ComponentList[i]), false, FTransform(FVector(0)));
		}
		ComponentsAdded.Broadcast();
		if(GetHealthComponent())
		{
			GetHealthComponent()->InitParams(CharacterInfo.MaxHealth,
				CharacterInfo.MaxShield,
				CharacterInfo.ShieldStartDelay,
				CharacterInfo.ShieldRecoveryValue,
				CharacterInfo.SheildRecoveryPeriod);
			GetHealthComponent()->OnOwnerDeath.AddDynamic(this,&ABaseCharacter::DeadEvent);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ADDED COMPONENTS TO OWNER --- NULL"));
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
}

void ABaseCharacter::FireOn()
{
}

UTDSInventory* ABaseCharacter::GetInventoryComponent()
{
	UTDSInventory* Inventory = FindComponentByClass<UTDSInventory>();
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
