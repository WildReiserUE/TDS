// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TDSItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAttack);

class ATDSItemBase;

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EItemType : uint8
{
	Item,
	Weapon,
	Armor,
	Projectile
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EArmorType : uint8
{
	Light,
	Heavy,
	Magic
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EArmorPart : uint8
{
	Helmet,
	UpperBody,
	LowerBody,
	FullBody,
	Gloves,
	Boots,
	Shield,
	Underwear
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponType : uint8
{
	RightHand,
	LeftHand,
	DoubleHand
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponClass : uint8
{
	Knife,
	Sword,
	Blunt,
	DualSword,
	Polearm,
	H2Shoting,
	H1Shoting,
	Shield
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponAttackSpeed : uint8
{
	VerySlow,
	Slow,
	Normal,
	Fast,
	VeryFast
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EItemGrade : uint8
{
	NoGrade,
	DGrade,
	CGrade,
	BGrade,
	AGrade,
	SGrade,
	S84Grade
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FProjectileInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* HitFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileMaxSpeed = 0.f;
	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	// FVector ProjectileDirection = FVector(0);
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	bool bBounced = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileBouncines = 0.3f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float ProjectileGravity = 0.f;
};


USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting", EditConditionHides))
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponClass WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting", EditConditionHides))
	TSubclassOf<ATDSItemBase> WeaponProjectile = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting", EditConditionHides))
	FName ProjectileName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponAttackSpeed AttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0", EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting", EditConditionHides))
	int PhysicalDamage = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0", EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting", EditConditionHides))
	int MagicalDamage = 0;
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FArmorInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorType ArmorType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorPart ArmorPart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float PhysicalDefence = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float MagicalDefence = 0.0f;
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FItemInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	int ItemID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemGrade ItemGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATDSItemBase> BaseClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	FWeaponInfo Weapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	bool bCanFire = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="-1", EditCondition="bCanFire == true", EditConditionHides))
	int ProjectileId = -1;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="ItemType == EItemType::Armor", EditConditionHides))
	FArmorInfo Armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="ItemType == EItemType::Projectile", EditConditionHides))
	FProjectileInfo Projectile;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* ItemMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* PickupSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* PickupFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* DropFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="ItemType == EItemType::Item", EditConditionHides))
	bool bIsStackable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0", EditCondition="bIsStackable == true"))
	int Count = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	int Cost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	int Weight = 0;
};

UCLASS()
class TDS_API ATDSItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	ATDSItemBase();

	// UFUNCTION()
	// void ArrowHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void SpawnParticleFx(UParticleSystem* NewParticle);
	void SpawnSoundHit(USoundBase* NewSound);

	UFUNCTION()
	void RenderOn(UPrimitiveComponent* pComponent);

	UFUNCTION()
	void RenderOff(UPrimitiveComponent* pComponent);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void ChangeSettings();
	
	UFUNCTION()
	void SomeClicked(UPrimitiveComponent* pComponent, FKey pKey);
	void Attack();
	void StopAttack();
	void SpawnBullet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMeshComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	FItemInfo ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	FName SpawnedName;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnWeaponFire OnWeaponFire;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnWeaponAttack OnWeaponAttack;

	bool bIsClicked = false;
	FTimerHandle AttackTimer;
	float AttackRate = 0.f;
	
private:
	virtual void BeginPlay() override;
};