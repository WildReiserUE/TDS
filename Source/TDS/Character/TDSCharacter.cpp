// Created WildReiser ©2022

#include "TDSCharacter.h"

#include "TDSInventory.h"
#include "TDSItemBase.h"
#include "TDSSkillComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

ATDSCharacter::ATDSCharacter(){
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->bUsePawnControlRotation = false; // Don't want arm to rotate when character does
	CameraArm->TargetArmLength = 1000.f;
	CameraArm->SetRelativeRotation(FRotator(-65.0f,-10.0f,0.0f));
	CameraArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bInheritRoll = false;
	
	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CharacterCameraComponent->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	CharacterCameraComponent->bUsePawnControlRotation = false; 
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDSCharacter::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	if(bIsALife)
	{
		//Cursor drawing
		if (CursorToWorld != nullptr){
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0)){
				if (PC != nullptr){
					FHitResult TraceHitResult;
					PC->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
					FVector CursorFV = TraceHitResult.ImpactNormal;
					FRotator CursorR = CursorFV.Rotation();
					CursorToWorld->SetWorldLocation(TraceHitResult.Location);
					CursorToWorld->SetWorldRotation(CursorR);
					auto Target = Cast<ATDSItemBase>(TraceHitResult.GetActor());
					if(Target){
						const auto InventoryComponent = FindComponentByClass<UTDSInventory>();
						if(InventoryComponent && InventoryComponent->FoundAround){
							NotifyActorOnClicked(EKeys::LeftMouseButton);
						}
					}
				}
			}
		}
		CurrentCharSpeed = GetVelocity().Size();
		MeshDirection = GetMesh()->GetAnimInstance()->CalculateDirection(GetVelocity(),GetActorRotation());
	
		AddMovementInput(FVector(1.f, -0.1f, 0.0f), AxisX);
		AddMovementInput(FVector(0.1f, 1.f, 0.0f), AxisY);

		//Character rotation only if Move or Aiming
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController){
			if (AxisX!=0 || AxisY!=0 || (bSniperMode && CurrentWeapon) || bRotateToAttack){
				FHitResult ResultHit;
				myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ResultHit);
				float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
				SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
			}
		}
	}
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &ATDSCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &ATDSCharacter::InputAxisY);
	NewInputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &ATDSCharacter::InputCameraIn);
	NewInputComponent->BindAction(TEXT("CameraZoomOut"),IE_Pressed, this, &ATDSCharacter::InputCameraOut);
	NewInputComponent->BindAction(TEXT("Sprint"),IE_Pressed, this, &ATDSCharacter::ActivateSprint);
	NewInputComponent->BindAction(TEXT("Sprint"),IE_Released, this, &ATDSCharacter::DeActivateSprint);
	NewInputComponent->BindAction(TEXT("SniperMode"),IE_Pressed, this, &ATDSCharacter::SniperModeOn);
	NewInputComponent->BindAction(TEXT("SniperMode"),IE_Released, this, &ATDSCharacter::SniperModeOff);
	NewInputComponent->BindAction(TEXT("NextWeapon"),IE_Pressed, this, &ATDSCharacter::NextWeapon);
	NewInputComponent->BindAction(TEXT("PrevWeapon"),IE_Pressed, this, &ATDSCharacter::PrevWeapon);
	NewInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&ATDSCharacter::FireOn);
	NewInputComponent->BindAction(TEXT("Fire"),IE_Released,this,&ATDSCharacter::FireOff);
	NewInputComponent->BindAction(TEXT("ReloadWeapon"),IE_Pressed,this,&ATDSCharacter::ReloadWeapon);
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	ChangeSettings();  //apply datatable info
	if (CursorMaterial){
		CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial,CursorSize,FVector(0));
	}

	if (CharacterInfo.ComponentList.Num()>0)
	{
		const int Comp = CharacterInfo.ComponentList.Num();
		for (int i = 0; i < Comp; i++)
		{
			this->AddComponentByClass(CharacterInfo.ComponentList[i], false, FTransform(FVector(0)),true);
			FinishAddComponent(GetComponentByClass(CharacterInfo.ComponentList[i]),false,FTransform(FVector(0)));
		}
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO PLAYER --- %i"), CharacterInfo.ComponentList.Num());
		ComponentsAdded.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp,Log,TEXT("ADDED COMPONENTS TO PLAYER --- HOJlb"));
	}	
}

void ATDSCharacter::InputAxisY(float Value){
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value){
	AxisX = Value;
}

void ATDSCharacter::InputCameraIn(){
	if(CameraArm->TargetArmLength <= BaseInfo.CameraMinLenght){
	}
	else
		CameraArm->TargetArmLength -= BaseInfo.CameraChangeStep;
}

void ATDSCharacter::InputCameraOut(){
	if(CameraArm->TargetArmLength >= BaseInfo.CameraMaxLenght){
	}
	else
		CameraArm->TargetArmLength += BaseInfo.CameraChangeStep;
}

void ATDSCharacter::ActivateSprint(){
	const auto SkillComponent = FindComponentByClass<UTDSSkillComponent>();
	if (SkillComponent && !bSniperMode){
		if (SkillComponent->SprintPoint > SkillComponent->SprintSettings.SprintLosePoint){
			GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed*SkillComponent->SprintCoef;
			SkillComponent->StartSprint();
			bSprintActivate=true;
		}
	}
}

void ATDSCharacter::DeActivateSprint(){
	const auto SkillComponent = FindComponentByClass<UTDSSkillComponent>();
	if (SkillComponent && !bSniperMode){
		SkillComponent->StopSprint();
		bSprintActivate=false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}

void ATDSCharacter::CalculateAllowSprint()
{
	bSprintAllow = MeshDirection >= -35.0f && MeshDirection <=35.0f;
	if(!bSprintAllow){
		const auto SkillComponent = FindComponentByClass<UTDSSkillComponent>();
		if (SkillComponent)
			DeActivateSprint();
	}
}

void ATDSCharacter::SniperModeOn(){	
	if(CurrentWeapon)
	{
		if(bSprintActivate)
     		DeActivateSprint();
		bSniperMode = true;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.AimMoveSpeed;
	}
}

void ATDSCharacter::SniperModeOff(){
	if(bSniperMode)
	{
		bSniperMode = false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}

UDecalComponent* ATDSCharacter::GetCursorToWorld(){
	return CursorToWorld;
}

ATDSItemBase* ATDSCharacter::SpawnWeapon(int WeaponIndex)
{
	const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
	if(PlayerInventory && bIsALife)
	{
		if(PlayerInventory->WeaponInventory.Num() > 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetInstigator();
			const FVector Loc(0,0,0);
			FTransform SpawnTransform = FTransform(Loc);
			ATDSItemBase* MyWeapon = Cast<ATDSItemBase>(GetWorld()->SpawnActorDeferred<ATDSItemBase>(PlayerInventory->WeaponInventory[WeaponIndex].BaseClass,
				SpawnTransform, SpawnParams.Owner, SpawnParams.Instigator,SpawnParams.SpawnCollisionHandlingOverride));
			MyWeapon->SpawnedName = PlayerInventory->WeaponInventory[WeaponIndex].DTItemName;
			MyWeapon->ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UGameplayStatics::FinishSpawningActor(MyWeapon,SpawnTransform);
			if (MyWeapon)
			{
				FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
				MyWeapon->AttachToComponent(GetMesh(), Rule, FName("RightHandSocket"));
				MyWeapon->ChangeSettings();
			}
			//UE_LOG(LogTemp, Warning, TEXT("My weapon Name: %s"), *MyWeapon->SpawnedName.ToString());
			CurrentWeapon = MyWeapon;
			return CurrentWeapon;
		}
	}
	return nullptr;
}

void ATDSCharacter::PrevWeapon()
{
	if(bIsALife)
	{
		const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
		if(PlayerInventory->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(WeaponReloadTimer))
		{
			if(CurrentWeapon)
			{
				CurrentWeapon->StopSpawnBullet();
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex --;
			if(CurrentWeaponIndex<0)
			{
				CurrentWeaponIndex = PlayerInventory->WeaponInventory.Num()-1;
			}
			StopAnimMontage();
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void ATDSCharacter::NextWeapon()
{
	if(bIsALife)
	{
		const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
		if(PlayerInventory->WeaponInventory.Num()> 0 && !GetWorld()->GetTimerManager().IsTimerActive(WeaponReloadTimer))
		{
			if(CurrentWeapon)
			{
				CurrentWeapon->StopSpawnBullet();
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex ++;
			if(CurrentWeaponIndex>=PlayerInventory->WeaponInventory.Num())
			{
				CurrentWeaponIndex = 0;
			}
			StopAnimMontage();
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void ATDSCharacter::FireOn()  //По нажатию кнопки - стрельба
{		
	if(CurrentWeapon)
	{
		const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
		if(PlayerInventory)
		{
			bool BulletsAviable = CurrentWeapon->ItemInfo.Weapon.Magazine > 0;
			if(BulletsAviable && CurrentWeapon->ItemInfo.Weapon.bCanFire)
			{
				UE_LOG(LogTemp, Warning, TEXT("BULLET ECTb -- Command to Weapon -Fire-"));
				bRotateToAttack = true;
				if(!GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->AttackTimer)){
					GetWorld()->GetTimerManager().SetTimer(CurrentWeapon->AttackTimer, this, &ATDSCharacter::StartFire, CurrentWeapon->AttackRate,true,0.f);
				}
			}
			else if (!CurrentWeapon->ItemInfo.Weapon.bCanFire)
			{				
				UE_LOG(LogTemp, Warning, TEXT("MELLEE ATTACK"));
				bRotateToAttack = true;
				CurrentWeapon->StartSpawnBullet();
				if(CharacterInfo.MontageHandleAttack.Num()>0)
				{
					int RndMontage = UKismetMathLibrary::RandomIntegerInRange(0,CharacterInfo.MontageHandleAttack.Num()-1);
					PlayAnimMontage(CharacterInfo.MontageHandleAttack[RndMontage]);
				}					
			}
		}
	}
}
		
void ATDSCharacter::StartFire()
{
	const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
	if(PlayerInventory)
	{
		//PlayerInventory->DecreaseCount(CurrentWeapon->ItemInfo.Weapon.ProjectileId);
		//UE_LOG(LogTemp,Log,TEXT("TRY BULLET ID = %i"), CurrentWeapon->ItemInfo.Weapon.ProjectileId)
		UE_LOG(LogTemp,Log,TEXT("BULLET IN MAGAZINE = %i"), CurrentWeapon->ItemInfo.Weapon.Magazine);
		int Result = CurrentWeapon->ItemInfo.Weapon.Magazine -= 1;
		UE_LOG(LogTemp,Log,TEXT("SHOT BULLET IN MAGAZINE = %i"), CurrentWeapon->ItemInfo.Weapon.Magazine);
		if( Result >=0)
		{
			CurrentWeapon->StartSpawnBullet();
			PlayAnimMontage(Montage2HAttack);
		}
		else
		{
			if(GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->AttackTimer))
			{
				GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);		
			}
			ReloadWeapon();
		}
	}
}

void ATDSCharacter::ReloadWeapon()
{	
 	if(CurrentWeapon)
 	{
 		const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
 		if(PlayerInventory
 			&& PlayerInventory->TryReloadWeapon(CurrentWeapon->ItemInfo.Weapon.ProjectileId)
 			&& (CurrentWeapon->ItemInfo.Weapon.Magazine < CurrentWeapon->ItemInfo.Weapon.MaxMagazine))
 		{
 			UE_LOG(LogTemp,Log,TEXT("BULLET IN MAGAZINE = %i"), CurrentWeapon->ItemInfo.Weapon.Magazine);
 			UE_LOG(LogTemp,Log,TEXT("NEED FIND BULLET ID = %i"), CurrentWeapon->ItemInfo.Weapon.ProjectileId);
 			UE_LOG(LogTemp,Log,TEXT("START RELOAD WEAPON"));
 		}
 		else 
 			UE_LOG(LogTemp,Log,TEXT("MAGAZINE IS FULL OR NO INVENTORY BULLETS"));
 	}
}
	
void ATDSCharacter::FireOff()
{
	if(CurrentWeapon && GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->AttackTimer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Command to Weapon - STOP Fire"));
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);		
		bRotateToAttack = false;
		CurrentWeapon->StopSpawnBullet();
	}
}