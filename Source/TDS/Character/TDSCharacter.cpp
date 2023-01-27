// Created WildReiser ©2022

#include "TDSCharacter.h"

#include "ReloadEndNotify.h"
#include "TDSSkillComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"

ATDSCharacter::ATDSCharacter()
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

void ATDSCharacter::Tick(float DeltaSeconds)
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
						const auto InventoryComponent = FindComponentByClass<UTDSInventory>();
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
				myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ResultHit);
				float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(
					GetActorLocation(), ResultHit.Location).Yaw;
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
	NewInputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &ATDSCharacter::InputCameraOut);
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ATDSCharacter::ActivateSprint);
	NewInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ATDSCharacter::DeActivateSprint);
	NewInputComponent->BindAction(TEXT("SniperMode"), IE_Pressed, this, &ATDSCharacter::SniperModeOn);
	NewInputComponent->BindAction(TEXT("SniperMode"), IE_Released, this, &ATDSCharacter::SniperModeOff);
	NewInputComponent->BindAction(TEXT("NextWeapon"), IE_Pressed, this, &ATDSCharacter::NextWeapon);
	NewInputComponent->BindAction(TEXT("PrevWeapon"), IE_Pressed, this, &ATDSCharacter::PrevWeapon);
	NewInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATDSCharacter::FireOn);
	NewInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATDSCharacter::FireOff);
	NewInputComponent->BindAction(TEXT("ReloadWeapon"), IE_Pressed, this, &ATDSCharacter::ReloadWeapon);
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CursorMaterial)
	{
		CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void ATDSCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void ATDSCharacter::InputCameraIn()
{
	if (CameraArm->TargetArmLength >= BaseInfo.CameraMinLenght)
		CameraArm->TargetArmLength -= BaseInfo.CameraChangeStep;
}

void ATDSCharacter::InputCameraOut()
{
	if (CameraArm->TargetArmLength <= BaseInfo.CameraMaxLenght)
		CameraArm->TargetArmLength += BaseInfo.CameraChangeStep;
}

void ATDSCharacter::ActivateSprint()
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

void ATDSCharacter::DeActivateSprint()
{
	if (GetSkillComponent() && !bSniperMode)
	{
		GetSkillComponent()->StopSprint();
		bSprintActivate = false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}

void ATDSCharacter::CalculateAllowSprint()
{
	bSprintAllow = MeshDirection >= -35.0f && MeshDirection <= 35.0f;
	if (!bSprintAllow)
	{
		//if (GetSkillComponent())
		DeActivateSprint();
	}
}

void ATDSCharacter::SniperModeOn()
{
	if (CurrentWeapon)
	{
		if (bSprintActivate)
			DeActivateSprint();
		bSniperMode = true;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.AimMoveSpeed;
	}
}

void ATDSCharacter::SniperModeOff()
{
	if (bSniperMode)
	{
		bSniperMode = false;
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo.RunSpeed;
	}
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
	return CursorToWorld;
}

ATDSItemBase* ATDSCharacter::SpawnWeapon(int WeaponIndex)
{
	if (GetInventoryComp() && bIsALife)
	{
		if (GetInventoryComp()->WeaponInventory.Num() > 0)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = GetInstigator();
			const FVector Loc(0, 0, 0);
			const FTransform SpawnTransform = FTransform(Loc);
			ATDSItemBase* MyWeapon = Cast<ATDSItemBase>(GetWorld()->SpawnActorDeferred<ATDSItemBase>(
				GetInventoryComp()->WeaponInventory[WeaponIndex].BaseClass,
				SpawnTransform,
				SpawnParams.Owner,
				SpawnParams.Instigator,
				SpawnParams.SpawnCollisionHandlingOverride));
			MyWeapon->SpawnedName = GetInventoryComp()->WeaponInventory[WeaponIndex].DTItemName;
			MyWeapon->ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UGameplayStatics::FinishSpawningActor(MyWeapon, SpawnTransform);
			if (MyWeapon)
			{
				FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
				MyWeapon->AttachToComponent(GetMesh(), Rule, FName("RightHandSocket"));
				MyWeapon->ChangeSettings();
			}
			CurrentWeapon = MyWeapon;
			return CurrentWeapon;
		}
	}
	return nullptr;
}

void ATDSCharacter::PrevWeapon()
{
	if (GetInventoryComp() && bIsALife)
	{
		if (GetInventoryComp()->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(
			WeaponReloadTimer))
		{
			if (CurrentWeapon)
			{
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBullet);
				CurrentWeapon->StopSpawnBullet();
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex --;
			if (CurrentWeaponIndex < 0)
			{
				CurrentWeaponIndex = GetInventoryComp()->WeaponInventory.Num() - 1;
			}
			StopAnimMontage();
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBullet);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void ATDSCharacter::NextWeapon()
{
	if (GetInventoryComp() && bIsALife)
	{
		if (GetInventoryComp()->WeaponInventory.Num() > 0 && !GetWorld()->GetTimerManager().IsTimerActive(
			WeaponReloadTimer))
		{
			if (CurrentWeapon)
			{
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBullet);
				CurrentWeapon->StopSpawnBullet();
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex ++;
			if (CurrentWeaponIndex >= GetInventoryComp()->WeaponInventory.Num())
			{
				CurrentWeaponIndex = 0;
			}
			StopAnimMontage();
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBullet);
			OnWeaponSwitch.Broadcast(CurrentWeaponIndex);
		}
	}
}

void ATDSCharacter::FireOn() //По нажатию кнопки - стрельба
{
	if (CurrentWeapon && GetInventoryComp())
	{
		if (CurrentWeapon->ItemInfo.Weapon.bCanFire)
		{
			UE_LOG(LogTemp, Log, TEXT("Command to Weapon ---Fire---"));
			bRotateToAttack = true;
			if (!GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->AttackTimer))
			{
				GetWorld()->GetTimerManager().SetTimer(
					CurrentWeapon->AttackTimer,
					this,
					&ATDSCharacter::StartFire,
					CurrentWeapon->AttackRate,
					true,
					0.f);
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Command to Weapon ---ATTACK---"));
			bRotateToAttack = true;
			CurrentWeapon->StartSpawnBullet();
			if (CharacterInfo.MontageHandleAttack.Num() > 0)
			{
				int RndMontage = UKismetMathLibrary::RandomIntegerInRange(
					0, CharacterInfo.MontageHandleAttack.Num() - 1);
				PlayAnimMontage(CharacterInfo.MontageHandleAttack[RndMontage]);
			}
		}
	}
}

void ATDSCharacter::StartFire()
{
	if (bIsALife && GetInventoryComp())
	{
		UE_LOG(LogTemp, Log, TEXT("BULLET IN MAGAZINE = %i"), CurrentWeapon->ItemInfo.Weapon.Magazine);
		if (CurrentWeapon->ItemInfo.Weapon.Magazine > 0)
		{
			CurrentWeapon->StartSpawnBullet();
			if (CharacterInfo.WeaponMontageShotingMap.Num() > 0)
			{
				auto Montag = CharacterInfo.WeaponMontageShotingMap.FindRef(CurrentWeapon->ItemInfo.Weapon.WeaponClass);
				PlayAnimMontage(Montag);
			}
		}
		else
		{
			FireOff();
			ReloadWeapon();
		}
	}
}

void ATDSCharacter::DecreaseBullet(int BulletInMagazine)
{
	UE_LOG(LogTemp, Log, TEXT("WEAPON DELEGATE --- %i"), BulletInMagazine);
}

void ATDSCharacter::ReloadWeapon()
{
	if (CurrentWeapon
		&& GetInventoryComp()
		&& CurrentWeapon->ItemInfo.Weapon.Magazine < CurrentWeapon->ItemInfo.Weapon.MaxMagazine
		&& GetInventoryComp()->CheckBullets(CurrentWeapon->ItemInfo.Weapon.ProjectileId))
	{
		UE_LOG(LogTemp, Log, TEXT("START RELOAD WEAPON"));
		auto Montag = CharacterInfo.WeaponMontageReloadMap.FindRef(CurrentWeapon->ItemInfo.Weapon.WeaponClass);
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montag))
		{
			PlayAnimMontage(Montag);
			if (Montag->IsNotifyAvailable())
			{
				const auto AnimNotifies = Montag->Notifies;
				for (const auto& AnimNotify : AnimNotifies)
				{
					auto ReloadEndNotify = Cast<UReloadEndNotify>(AnimNotify.Notify);
					if (ReloadEndNotify)
					{
						UE_LOG(LogTemp, Warning, TEXT("ReloadEnd --- NOTIFY AVAIABLE"));
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

FName ATDSCharacter::ReloadEnd()
{
	if (GetInventoryComp())
	{
		GetInventoryComp()->DecreaseCount(CurrentWeapon->ItemInfo);
	}
	return FName();
}

void ATDSCharacter::FireOff()
{
	if (CurrentWeapon && GetWorld()->GetTimerManager().IsTimerActive(CurrentWeapon->AttackTimer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Command to Weapon - STOP Fire"));
		GetWorld()->GetTimerManager().ClearTimer(CurrentWeapon->AttackTimer);
		bRotateToAttack = false;
		CurrentWeapon->StopSpawnBullet();
	}
}
