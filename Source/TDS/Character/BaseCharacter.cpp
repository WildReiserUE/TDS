// Created WildReiser @2022

#include "BaseCharacter.h"

#include "PlayerCharacter.h"
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
		for(int comp = 0 ; comp < CharacterInfo.ComponentList.Num() ; comp++) 
		{
			FTransform ComponentTransform = FTransform(FRotator(0),FVector(0),FVector(1));
			AddComponentByClass(CharacterInfo.ComponentList[comp],false, ComponentTransform,true);
			UE_LOG(LogTemp, Log, TEXT("BASE CHAR LOOP --- ADD COMPONENT --- %s"), *GetName());
		}
		RegisterAllComponents();
		//INITIALIZE COMPONENTS PARAMS
		if(GetHealthComponent())
		{
			GetHealthComponent()->InitParams(CharacterInfo.HealthParams);
			GetHealthComponent()->OnOwnerDeath.AddDynamic(this,&ABaseCharacter::DeadEvent);
		}
		if(GetSkillComponent())
		{
			GetSkillComponent()->InitSprint(CharacterInfo.SkillParams);
		}

		CompleteAddComponent.Broadcast();
		UE_LOG(LogTemp, Log, TEXT("BASE CHAR LOG --- TOTAL ADDED COMPONENTS = %i"), CharacterInfo.ComponentList.Num());
	}
}

AController* ABaseCharacter::GetCurrentController()
{
	AController* CurrentContoller = GetController();
	return CurrentContoller ? CurrentContoller : nullptr;
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

void ABaseCharacter::DeadEvent(AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("DEAD EVENT CPP"));
	bIsALife = false;

	if(!BaseInfo.bIsPlayer && Killer)
	{
		if(Killer->GetOwner()->IsA<APlayerCharacter>())
			UE_LOG(LogTemp, Log, TEXT("DEAD EVENT CPP --- EXPIRIENCE = %i --- NEED ADD TO --- %s"), CharacterInfo.CurrentExperience, *Killer->GetOwner()->GetName());
		if(Killer->GetOwner()->GetOwner()->IsA<APlayerCharacter>())
			UE_LOG(LogTemp, Log, TEXT("DEAD EVENT CPP --- EXPIRIENCE = %i --- NEED ADD TO --- %s"), CharacterInfo.CurrentExperience, *Killer->GetOwner()->GetOwner()->GetName());
	}

	if(CharacterInfo.MontageDead.Num() > 0)
	{
		int32 RND_Montage = UKismetMathLibrary::RandomIntegerInRange(0,CharacterInfo.MontageDead.Num()-1);
		GetMesh()->GetAnimInstance()->Montage_Play(CharacterInfo.MontageDead[RND_Montage]);
		//float EndPosition = CharacterInfo.MontageDead[RND_Montage]->CalculateSequenceLength();

		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
		//SetActorEnableCollision(true);

		//GetMesh()->SetAllBodiesSimulatePhysics(true);
		//GetMesh()->SetSimulatePhysics(true);
		//GetMesh()->WakeAllRigidBodies();
		//GetMesh()->bBlendPhysics = true;

		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		
		//GetWorld()->GetTimerManager().SetTimer(DeadTimer,this,&ABaseCharacter::StartRagdoll, 1.f,false, EndPosition + 1.f);//TODO: Ragdoll
	}
}

void ABaseCharacter::StartRagdoll()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	//TODO Need move to...
	//DetachFromControllerPendingDestroy();
	//GetController()->UnPossess();
	//SetLifeSpan(EndPos + 3.f);
}
