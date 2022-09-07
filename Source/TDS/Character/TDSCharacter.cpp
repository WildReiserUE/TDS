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
					auto Target = Cast<ATDSItemBase>(TraceHitResult.Actor);
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
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial){
		CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial,CursorSize,FVector(0));
	}

	if (ComponentList.Num()>0)
	{
		const int Comp = ComponentList.Num();
		for (int i = 0; i < Comp; i++){
			this->AddComponentByClass(ComponentList[i], false, FTransform(FVector(0)),false);
		}
		ComponentsAdded.Broadcast();
	}
	InitParams();
	
}

void ATDSCharacter::InitParams(){
	MaxCameraLenght = CharacterInfo.CameraMaxLenght;
	MinCameraLenght = CharacterInfo.CameraMinLenght;
	CameraChangeStep = CharacterInfo.CameraChangeStep;
	CharBaseMoveSpeed = CharacterInfo.BaseMoveSpeed;
	CharAimMoveSpeed = CharacterInfo.AimMoveSpeed;
}

void ATDSCharacter::InputAxisY(float Value){
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value){
	AxisX = Value;
}

void ATDSCharacter::InputCameraIn(){
	if(CameraArm->TargetArmLength <= MinCameraLenght){
	}
	else
		CameraArm->TargetArmLength -= CameraChangeStep;
}

void ATDSCharacter::InputCameraOut(){
	if(CameraArm->TargetArmLength >= MaxCameraLenght){
	}
	else
		CameraArm->TargetArmLength += CameraChangeStep;
}

void ATDSCharacter::ActivateSprint(){
	const auto SkillComponent = FindComponentByClass<UTDSSkillComponent>();
	if (SkillComponent && !bSniperMode){
		if (SkillComponent->SprintPoint > SkillComponent->SprintSettings.SprintLosePoint){
			GetCharacterMovement()->MaxWalkSpeed = CharBaseMoveSpeed*SkillComponent->SprintCoef;
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
		GetCharacterMovement()->MaxWalkSpeed = CharBaseMoveSpeed;
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
		GetCharacterMovement()->MaxWalkSpeed = CharAimMoveSpeed;
	}
}

void ATDSCharacter::SniperModeOff(){
	if(bSniperMode)
	{
		bSniperMode = false;
		GetCharacterMovement()->MaxWalkSpeed = CharBaseMoveSpeed;
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
			MyWeapon->SpawnedName = PlayerInventory->WeaponInventory[WeaponIndex].ItemName;
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
				CurrentWeapon->StopAttack();
				//CurrentWeapon->ItemInfo.Weapon.WeaponClass == EWeaponClass::Shoting;
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBulletCount);
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex --;
			if(CurrentWeaponIndex<0)
			{
				CurrentWeaponIndex = PlayerInventory->WeaponInventory.Num()-1;
			}
			if(MontageHandleAttack)
				StopAnimMontage(MontageHandleAttack);
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBulletCount);
			OnWeaponSwitch.Broadcast();
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
				CurrentWeapon->StopAttack();
				//CurrentWeapon->WeaponInfo.WeaponType=EWeaponType::NoWeapon;
				CurrentWeapon->OnWeaponFire.RemoveDynamic(this, &ATDSCharacter::DecreaseBulletCount);
				CurrentWeapon->Destroy();
			}
			CurrentWeaponIndex ++;
			if(CurrentWeaponIndex>=PlayerInventory->WeaponInventory.Num())
			{
				CurrentWeaponIndex = 0;
			}
			if(MontageHandleAttack)
				StopAnimMontage(MontageHandleAttack);
			PlayerInventory->OnBulletsEnd.RemoveDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon = SpawnWeapon(CurrentWeaponIndex);
			PlayerInventory->OnBulletsEnd.AddDynamic(this, &ATDSCharacter::FireOff);
			CurrentWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::DecreaseBulletCount);
			OnWeaponSwitch.Broadcast();
		}
	}
}
void ATDSCharacter::DecreaseBulletCount()
{
	const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
	if(PlayerInventory)
	{		
		UE_LOG(LogTemp, Warning, TEXT("Command to DECREASE PROJECTILE ID --- %i"), CurrentWeapon->ItemInfo.ProjectileId);
		PlayerInventory->DecreaseCount(CurrentWeapon->ItemInfo.ProjectileId);
	}
}

void ATDSCharacter::FireOn()
{		
	if(CurrentWeapon)
	{
		const auto PlayerInventory = FindComponentByClass<UTDSInventory>();
		if(PlayerInventory)
		{
			bool BulletsAviable = PlayerInventory->CheckCount(CurrentWeapon->ItemInfo.ProjectileId);
			if(BulletsAviable && CurrentWeapon->ItemInfo.bCanFire)
			{
				UE_LOG(LogTemp, Warning, TEXT("BULLET FOUND -- Command to Weapon -Fire-"));
				bRotateToAttack = true;
				PlayerInventory->DecreaseCount(CurrentWeapon->ItemInfo.ProjectileId);
				CurrentWeapon->Attack();
				PlayAnimMontage(Montage2HAttack);
			}
			else if (!CurrentWeapon->ItemInfo.bCanFire)
			{				
				UE_LOG(LogTemp, Warning, TEXT("MELLEE ATTACK"));
				bRotateToAttack = true;
				CurrentWeapon->Attack();
				PlayAnimMontage(MontageHandleAttack);
			}
		}
	}
}

void ATDSCharacter::FireOff()
{
	if(CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Command to Weapon - STOP Fire"));
		bRotateToAttack = false;
		CurrentWeapon->StopAttack();
	}
}