/// h File
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TDSHealthComponent.h"
#include "GameFramework/Character.h"
#include "TDSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentsAdded);

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_BODY()
	float CameraMaxLenght = 1500.f;
	float CameraMinLenght = 300.f;
	float CameraChangeStep = 75.f;
	//TODO: Move to SkillComponent
	float BaseMoveSpeed = 600.f;
	float AimMoveSpeed = 200.f;
};

UCLASS()
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDSCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraArm; }
	/** Returns CursorToWorld subobject **/
	UFUNCTION()
    	void InitParams();
	UFUNCTION()
		void InputAxisY(float Value);
	UFUNCTION()
		void InputAxisX(float Value);
	UFUNCTION()
		void InputCameraIn();
	UFUNCTION()
		void InputCameraOut();
	UFUNCTION()
		UDecalComponent* GetCursorToWorld();
	
	// UFUNCTION(BlueprintCallable)
	// 	ATDSWeaponBase* GetCurrentWeapon();
	// UFUNCTION(BlueprintCallable)
	// 	void SetCurrentWeapon(ATDSWeaponBase* CurWeapon);
	// UFUNCTION(BlueprintCallable)
	// 	void FireOn();
	// UFUNCTION(BlueprintCallable)
	// 	void FireOff();

	void CalculateAllowSprint();

	UFUNCTION()
		void ActivateSprint();	
	UFUNCTION()
		void DeActivateSprint();
	UFUNCTION()
		void SniperModeOn();
	UFUNCTION()
		void SniperModeOff();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterDirection")
	float MeshDirection = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterDirection")
	float CurrentCharSpeed = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateParam")
	bool bSniperMode = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera")
	UDecalComponent* CursorToWorld;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cursor")
	UMaterialInterface* CursorMaterial =nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cursor")
	FVector CursorSize=FVector(10.0f,20.0f,20.0f);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UActorComponent>> ComponentList;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Components")
	FOnComponentsAdded ComponentsAdded;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="CharacterParams")
	FCharacterInfo CharacterInfo;
	
	float AxisX = 0.0f;
	float AxisY = 0.0f;
	bool bSprintAllow = false;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;	

	float MaxCameraLenght = 0.f;
	float MinCameraLenght = 0.f;
	float CameraChangeStep = 0.f;
	float CharBaseMoveSpeed = 0.f;
	float CharAimMoveSpeed = 0.f;
	bool bSprintActivate = false;
	bool bIsALife = true;
};