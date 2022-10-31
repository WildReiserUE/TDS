// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "TDSHealthComponent.h"
#include "TDSItemBase.h"
#include "GameFramework/Character.h"
#include "TDSCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComponentsAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitch,FItemInfo,WeaponInfo);

USTRUCT(BlueprintType) //Структура с настройками Игрока
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

	FORCEINLINE class UCameraComponent* GetCharacterCameraComponent() const { return CharacterCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraArm; }

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
	UFUNCTION(BlueprintCallable)
		ATDSItemBase* SpawnWeapon(int WeaponIndex);
	void PrevWeapon();
	void NextWeapon();
	int Some();
	
	UFUNCTION(BlueprintCallable)
		void FireOn();
	UFUNCTION()
		void StartFire();
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
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Player Settings")
	TArray<TSubclassOf<UActorComponent>> ComponentList;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnComponentsAdded ComponentsAdded;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnWeaponSwitch OnWeaponSwitch;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Player Settings")
	FCharacterInfo CharacterInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	ATDSItemBase* CurrentWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Player Settings")
	UAnimMontage* MontageHandleAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Player Settings")
	UAnimMontage* Montage2HAttack = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Player Settings")
	UAnimMontage* Montage2HReload = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Player Settings")
	UAnimMontage* MontageDead = nullptr;
	
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

	float MaxCameraLenght = 0.f;
	float MinCameraLenght = 0.f;
	float CameraChangeStep = 0.f;
	float CharBaseMoveSpeed = 0.f;
	float CharAimMoveSpeed = 0.f;
	bool bSprintActivate = false;
	bool bIsALife = true;
	FTimerHandle WeaponReloadTimer;
};