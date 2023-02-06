// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "TDSItemBase.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitch, int, WeaponIndex);

UCLASS()
class APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }
	USpringArmComponent* GetCameraBoom() const { return CameraArm; }

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

	UFUNCTION()
	virtual void AttackOn() override;
	UFUNCTION()
	virtual void AttackOff() override;
	UFUNCTION()
	virtual void LaunchTimer(FTimerHandle &Timer) override;
	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void DecreaseBullet(FItemInfo Info);

	void ReloadWeapon();

	UFUNCTION()
	FName ReloadEnd();

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
	UDecalComponent* CursorToWorld = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Settings")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Settings")
	FVector CursorSize = FVector(10.0f, 20.0f, 20.0f);

	float AxisX = 0.0f;
	float AxisY = 0.0f;
	bool bSprintAllow = false;
	bool bRotateToAttack = false;
	int CurrentWeaponIndex = -1;

private:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSwitch OnWeaponSwitch;

	bool bFireAllow = false;
	FTimerHandle FireTimer;
	bool bSprintActivate = false;
	FTimerHandle WeaponReloadTimer;
};
