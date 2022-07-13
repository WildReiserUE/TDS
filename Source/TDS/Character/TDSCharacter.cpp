#include "TDSCharacter.h"
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
	
	// Don't rotate character to camera direction
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
			}
		}
	}
	
	//Movement logic	
	CurrentCharSpeed = GetVelocity().Size();
	MeshDirection = GetMesh()->GetAnimInstance()->CalculateDirection(GetVelocity(),GetActorRotation());
	
	CalculateAllowSprint();
	
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

	//Character rotation to Cursor
	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (myController){
		if (AxisX!=0 || AxisY!=0){
			FHitResult ResultHit;
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, ResultHit);
			float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));
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
	// NewInputComponent->BindAction(TEXT("SniperMode"),IE_Pressed, this, &ATDSCharacter::SniperModeOn);
	// NewInputComponent->BindAction(TEXT("SniperMode"),IE_Released, this, &ATDSCharacter::SniperModeOff);
	// NewInputComponent->BindAction(TEXT("Fire"),IE_Pressed,this,&ATDSCharacter::FireOn);
	// NewInputComponent->BindAction(TEXT("Fire"),IE_Released,this,&ATDSCharacter::FireOff);
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial){
		CursorToWorld = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial,CursorSize,FVector(0));
	}
}

void ATDSCharacter::InputAxisY(float Value){
	AxisY = Value;
}

void ATDSCharacter::InputAxisX(float Value){
	AxisX = Value;
}

void ATDSCharacter::InputCameraIn(){
	if(CameraArm->TargetArmLength <= 300){
	}
	else
		CameraArm->TargetArmLength -= 75.0f;
}

void ATDSCharacter::InputCameraOut(){
	if(CameraArm->TargetArmLength >= 1500){
	}
	else
		CameraArm->TargetArmLength += 75.0f;
}

void ATDSCharacter::ActivateSprint()
{
	const auto SprintComponent = FindComponentByClass<UTDSSprintComponent>();
	if (SprintComponent){
		if (SprintComponent->SprintPoint > SprintComponent->SprintSettings.SprintLosePoint){
			GetCharacterMovement()->MaxWalkSpeed = 900.0f;
			SprintComponent->StartSprint();
		}
	}
}

void ATDSCharacter::DeActivateSprint()
{
	const auto SprintComponent = FindComponentByClass<UTDSSprintComponent>();
	if (SprintComponent){
		SprintComponent->StopSprint();
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}


// void ATDSCharacter::SniperModeOn(){
// 	if(bSprintActivate)
// 		bSprintActivate=false;
// 	bSniperMode = true;
// 	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
// }

// void ATDSCharacter::SniperModeOff(){
// 	if(bSniperMode)
// 		bSniperMode = false;
// 	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
// }

void ATDSCharacter::CalculateAllowSprint()
{
	bSprintAllow = MeshDirection >= -35.0f && MeshDirection <=35.0f;
	if(!bSprintAllow){
		const auto SprintComponent = FindComponentByClass<UTDSSprintComponent>();
		if (SprintComponent)
			DeActivateSprint();
	}
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
	return CursorToWorld;
}

// void ATDSCharacter::FireOn()
// {
// 	if(CurrentWeapon->WeaponCanFire)
// 	{
// 		//UE_LOG(LogViewport, Display, TEXT("Command to Weapon - Fire"));
// 		CurrentWeapon->Fire();		
// 	}	
// }
// void ATDSCharacter::FireOff()
// {
// 	//UE_LOG(LogViewport, Display, TEXT("Command to Weapon - Stop Fire"));
// 	CurrentWeapon->StopFire();
// }