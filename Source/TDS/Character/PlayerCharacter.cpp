// Created WildReiser ©2022

#include "PlayerCharacter.h"


#include "K2Node_GetDataTableRow.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "ReloadEndNotify.h"
#include "TDSSkillComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

APlayerCharacter::APlayerCharacter()
{
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
	CameraArm->SetRelativeRotation(FRotator(-65.0f, -10.0f, 0.0f));
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

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bIsALife)
	{
		//Cursor drawing
		if (CursorToWorld != nullptr)
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (PC != nullptr)
				{
					FHitResult TraceHitResult;
					PC->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
					FVector CursorFV = TraceHitResult.ImpactNormal;
					FRotator CursorR = CursorFV.Rotation();
					CursorToWorld->SetWorldLocation(TraceHitResult.Location);
					CursorToWorld->SetWorldRotation(CursorR);
					auto Target = Cast<ATDSItemBase>(TraceHitResult.GetActor());
					if (Target)
					{
						const auto InventoryComponent = FindComponentByClass<UTDSInventoryComponent>();
						if (InventoryComponent && InventoryComponent->FoundAround)
						{
							NotifyActorOnClicked(EKeys::LeftMouseButton);
						}
					}
				}
			}
		}
		CurrentCharSpeed = GetVelocity().Size();
		MeshDirection = UKismetAnimationLibrary::CalculateDirection(GetVelocity(), GetActorRotation());

		AddMovementInput(FVector(1.f, -0.1f, 0.0f), AxisX);
		AddMovementInput(FVector(0.1f, 1.f, 0.0f), AxisY);

		//Character rotation only if Move or Aiming
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController)
		{
			if (AxisX != 0 || AxisY != 0 || (bSniperMode && CurrentWeapon) || bRotateToAttack)
			{
				FHitResult ResultHit;
				myController->GetHitResultUnderCursor(ECC_GameTraceChannel2, false, ResultHit);
				float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(
					GetActorLocation(), ResultHit.Location).Yaw;
				SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);
	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::InputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::InputAxisY);
	NewInputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &APlayerCharacter::InputCameraIn);
	NewInputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &APlayerCharacter::InputCameraOut);
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerCharacter::ActivateSprint);
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerCharacter::DeActivateSprint);
	NewInputComponent->BindAction(TEXT("SniperMode"), IE_Pressed, this, &APlayerCharacter::SniperModeOn);
	NewInputComponent->BindAction(TEXT("SniperMode"), IE_Released, this, &APlayerCharacter::SniperModeOff);
	NewInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, this, &APlayerCharacter::NextWeapon);
	NewInputComponent->BindAction(TEXT("PrevWeapon"), IE_Pressed, this, &APlayerCharacter::PrevWeapon);
	NewInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerCharacter::AttackOn);
	NewInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &APlayerCharacter::AttackOff);
	NewInputComponent->BindAction(TEXT("ReloadWeapon"), IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	bIsALife = true;

}

void APlayerCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void APlayerCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void APlayerCharacter::InputCameraIn()
{
	if (CameraArm->TargetArmLength >= BaseInfo.CameraMinLenght)
		CameraArm->TargetArmLength -= BaseInfo.CameraChangeStep;
}

void APlayerCharacter::InputCameraOut()
{
	if (CameraArm->TargetArmLength <= BaseInfo.CameraMaxLenght)
		CameraArm->TargetArmLength += BaseInfo.CameraChangeStep;
}

void APlayerCharacter::ActivateSprint()
{
	if (GetSkillComponent() && !bSniperMode)
	{
		if (GetSkillComponent()->SprintPoint > GetSkillComponent()->SprintSettings.SprintLosePoint)
		{
			GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed * GetSkillComponent()->SprintCoef;
			GetSkillComponent()->StartSprint();
			bSprintActivate = true;
		}
	}
}

void APlayerCharacter::DeActivateSprint()
{
	if (GetSkillComponent() && !bSniperMode)
	{
		GetSkillComponent()->StopSprint();
		bSprintActivate = false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}

void APlayerCharacter::CalculateAllowSprint()
{
	bSprintAllow = MeshDirection >= -35.0f && MeshDirection <= 35.0f;
	if (!bSprintAllow)
	{
		//if (GetSkillComponent())
		DeActivateSprint();
	}
}

void APlayerCharacter::SniperModeOn()
{
	if (CurrentWeapon)
	{
		if (bSprintActivate)
			DeActivateSprint();
		bSniperMode = true;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.AimMoveSpeed;
	}
}

void APlayerCharacter::SniperModeOff()
{
	if (bSniperMode)
	{
		bSniperMode = false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}


UDecalComponent* APlayerCharacter::GetCursorToWorld()
{
	return CursorToWorld;
}

ATDSItemBase* APlayerCharacter::SpawnWeapon(int WeaponIndex)
{
	if (GetInventoryComponent() && bIsALife)
	{
		if (GetInventoryComponent()->WeaponInventory.Num() > 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			const FVector Loc(0, 0, 0);
			const FTransform SpawnTransform = FTransform(Loc);
			ATDSItemBase* MyWeapon = Cast<ATDSItemBase>(GetWorld()->SpawnActorDeferred<ATDSItemBase>(
				GetInventoryComponent()->WeaponInventory[WeaponIndex].BaseClass,
				SpawnTransform,
				SpawnParams.Owner,
				SpawnParams.Instigator,
				SpawnParams.SpawnCollisionHandlingOverride));
			//MyWeapon->SpawnedName = GetInventoryComponent()->WeaponInventory[WeaponIndex].DTItemName;
			MyWeapon->ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			MyWeapon->ItemInfo = GetInventoryComponent()->WeaponInventory[WeaponIndex];
			UGameplayStatics::FinishSpawningActor(MyWeapon, SpawnTransform);
			if (MyWeapon)
			{
				FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
				MyWeapon->AttachToComponent(GetMesh(), Rule, FName("RightHandSocket"));
				MyWeapon->ChangeSettings();
				CurrentWeapon = MyWeapon;
				bFireAllow = true;
			}
			return CurrentWeapon;
		}
	}
	return nullptr;
}

void APlayerCharacter::PrevWeapon()
{
	if (GetInventoryComponent() && bIsALife)
	{
		if (GetInventoryComponent()->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(
			WeaponReloadTimer))
		{
			if (CurrentWeapon)
			{
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &APlayerCharacter::DecreaseBullet);
				GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);
				//GetInventoryComponent()->WeaponInventory[CurrentWeaponIndex] = CurrentWeapon->ItemInfo;
				//TODO: STORE CURRENT INFO IN INVENTORY
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex --;
			if (CurrentWeaponIndex < 0)
			{
				CurrentWeaponIndex = GetInventoryComponent()->WeaponInventory.Num() - 1;
			}
			StopAnimMontage();
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &APlayerCharacter::DecreaseBullet);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void APlayerCharacter::NextWeapon()
{
	if (GetInventoryComponent() && bIsALife)
	{
		if (GetInventoryComponent()->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(
			WeaponReloadTimer))
		{
			if (CurrentWeapon)
			{
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &APlayerCharacter::DecreaseBullet);
				GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);
				//TODO: STORE CURRENT INFO IN INVENTORY
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex ++;
			if (CurrentWeaponIndex >= GetInventoryComponent()->WeaponInventory.Num())
			{
				CurrentWeaponIndex = 0;
			}
			StopAnimMontage();
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &APlayerCharacter::DecreaseBullet);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void APlayerCharacter::AttackOn() //По нажатию кнопки - стрельба
{
	if (CurrentWeapon && bFireAllow)
	{
		bRotateToAttack = true;
		if (CurrentWeapon->ItemInfo.Weapon.bCanFire && GetInventoryComponent())
		{
			bFireAllow = false;
			LaunchTimer(CurrentWeapon->AttackTimer);
		}
		else
		{
			if (CharacterInfo.MontageHandleAttack.Num() > 0)
			{
				int RndMontage = UKismetMathLibrary::RandomIntegerInRange(0, CharacterInfo.MontageHandleAttack.Num() - 1);
				PlayAnimMontage(CharacterInfo.MontageHandleAttack[RndMontage]);
			}
		}
	}
}

void APlayerCharacter::AttackOff()
{
	if (!bFireAllow)
	{
		bFireAllow = true;
		bRotateToAttack = false;
	}
	else if (!bIsALife)
	{
		bRotateToAttack = false;
	}
}

void APlayerCharacter::LaunchTimer(FTimerHandle &Timer)
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(Timer))
	{
		bFireAllow = false;
	    GetWorld()->GetTimerManager().SetTimer(Timer,this, &APlayerCharacter::StartFire, CurrentWeapon->CAttackRate,true,0.f);
	}
	else
	{
		bFireAllow = true;
	}
}

void APlayerCharacter::StartFire()
{
	if (bIsALife && !bFireAllow)
	{
		if (CurrentWeapon->ItemInfo.Weapon.Magazine > 0 && GetInventoryComponent())
		{
			CurrentWeapon->StartSpawnBullet();
			if (CharacterInfo.WeaponMontageShotingMap.Num() > 0)
			{
				PlayAnimMontage(CharacterInfo.WeaponMontageShotingMap.FindRef(CurrentWeapon->ItemInfo.Weapon.WeaponClass));
			}
		}
		else
		{
			ReloadWeapon();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);
	}
}

void APlayerCharacter::DecreaseBullet(FItemInfo Info)
{
	GetInventoryComponent()->WeaponInventory[CurrentWeaponIndex] = Info;
	UE_LOG(LogTemp, Log, TEXT("WEAPON DELEGATE --- SAVE ITEM %i"), GetInventoryComponent()->WeaponInventory[CurrentWeaponIndex].Weapon.Magazine);
}

void APlayerCharacter::ReloadWeapon()
{
	if (CurrentWeapon
		&& GetInventoryComponent()
		&& CurrentWeapon->ItemInfo.Weapon.Magazine < CurrentWeapon->ItemInfo.Weapon.MaxMagazine
		&& GetInventoryComponent()->CheckBullets(CurrentWeapon->ItemInfo.Weapon.ProjectileId))
	{
		//UE_LOG(LogTemp, Log, TEXT("START RELOAD WEAPON"));
		UAnimMontage* ReloadMontage = CharacterInfo.WeaponMontageReloadMap.FindRef(CurrentWeapon->ItemInfo.Weapon.WeaponClass);
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(ReloadMontage) && ReloadMontage)
		{
			PlayAnimMontage(ReloadMontage);
			if (ReloadMontage->IsNotifyAvailable())
			{
				const TArray<FAnimNotifyEvent> AnimNotifies = ReloadMontage->Notifies;
				for (FAnimNotifyEvent AnimNotify : AnimNotifies)
				{
					if (UReloadEndNotify* ReloadEndNotify = Cast<UReloadEndNotify>(AnimNotify.Notify))
					{
						//UE_LOG(LogTemp, Warning, TEXT("ReloadEnd --- NOTIFY AVAIABLE"));
						if (!ReloadEndNotify->OnNotified.IsBoundToObject(this))
							ReloadEndNotify->OnNotified.AddUFunction(this, FName("ReloadEnd"), AnimNotify.NotifyName);
					}
				}
			}
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("MAGAZINE IS FULL OR NO INVENTORY BULLETS"));
}

FName APlayerCharacter::ReloadEnd()
{
	if (GetInventoryComponent())
	{
		GetInventoryComponent()->DecreaseInventoryCount(CurrentWeapon->ItemInfo);
	}
	return FName();
}