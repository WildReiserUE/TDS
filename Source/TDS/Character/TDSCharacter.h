// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "TDSItemBase.h"
#include "GameFramework/Character.h"
#include "TDSCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitch,int,WeaponIndex);

UCLASS()
class ATDSCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ATDSCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	FORCEINLINE class UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraArm; }
	
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
	UFUNCTION(BlueprintCallable)
	ATDSItemBase* SpawnWeapon(int WeaponIndex);
	void PrevWeapon();
	void NextWeapon();
	int Some();
	
	UFUNCTION(BlueprintCallable)
	void FireOn();
	UFUNCTION()
	void StartFire();
	void ReloadWeapon();
	UFUNCTION()
	FName Example();
	UFUNCTION(BlueprintCallable)
	void FireOff();

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Settings")
	UDecalComponent* CursorToWorld;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Settings")
	UMaterialInterface* CursorMaterial =nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Settings")
	FVector CursorSize=FVector(10.0f,20.0f,20.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	ATDSItemBase* CurrentWeapon = nullptr;
	
	float AxisX = 0.0f;
	float AxisY = 0.0f;
	bool bSprintAllow = false;
	bool bRotateToAttack = false;
	int CurrentWeaponIndex = -1;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharacterCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;	

	UPROPERTY(BlueprintAssignable)
	FOnComponentsAdded ComponentsAdded;
	
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSwitch OnWeaponSwitch;
	
	bool bSprintActivate = false;
	bool bIsALife = true;
	FTimerHandle WeaponReloadTimer;
};