// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Engine/DataTable.h"
#include "TDSItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFire, int, Count);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAttack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileHit);

class ATDSItemBase;
class UNiagaraSystem;
class UNiagaraComponent;

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EItemType : uint8
{
	None,
	Item,
	Weapon,
	Armor,
	Projectile
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EArmorType : uint8
{
	None,
	Light,
	Heavy,
	Magic
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EArmorPart : uint8
{
	None,
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
enum class EWeaponLocation : uint8
{
	None,
	RightHand,
	LeftHand,
	DoubleHand
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponHandSubClass : uint8
{
	None,
	Knife,
	Sword,
	Blunt,
	DualSword,
	Polearm,
	Shield
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponClass : uint8
{
	None,
	Handle,
	H2Shoting,
	H1Shoting,
	Shield
};


UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EWeaponAttackSpeed : uint8
{
	None,
	VerySlow,
	Slow,
	Normal,
	Fast,
	VeryFast
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EItemGrade : uint8
{
	None,
	NoGrade,
	DGrade,
	CGrade,
	BGrade,
	AGrade,
	SGrade,
	S84Grade
};

UENUM(BlueprintType, meta = (ExposeOnSpawn))
enum class EProjectileTypeDamage : uint8
{
	None,
	Point,
	Radial,
	Visible
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FProjectileInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="HitNiagara == nullptr", EditConditionHides))
	UParticleSystem* HitParticle = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="HitParticle == nullptr ", EditConditionHides))
	UNiagaraSystem* HitNiagara = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileMaxSpeed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bBounced = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float ProjectileBouncines = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileGravity = 0.f;
};


USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass != EWeaponClass::H2Shoting"))
	EWeaponLocation WeaponLocation = EWeaponLocation::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponClass WeaponClass = EWeaponClass::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass == EWeaponClass::Handle"))
	EWeaponHandSubClass WeaponHandSubClass = EWeaponHandSubClass::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting"))
	TSubclassOf<ATDSItemBase> WeaponProjectile = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting"))
	EProjectileTypeDamage ProjectileTypeDamage = EProjectileTypeDamage::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting"))
	USoundBase* ShootSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting"))
	FName ProjectileName = FName("NoName");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="WeaponClass == EWeaponClass::H1Shoting || WeaponClass == EWeaponClass::H2Shoting"))
	bool bCanFire = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="-1", EditCondition="bCanFire == true"))
	int ProjectileId = -1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="1", EditCondition="bCanFire == true"))
	int Magazine = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="1", EditCondition="bCanFire == true"))
	int MaxMagazine = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponAttackSpeed AttackSpeed = EWeaponAttackSpeed::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PhysicalDamage = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ProjectileTypeDamage != EProjectileTypeDamage::Point"))
	float DamageRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ProjectileTypeDamage != EProjectileTypeDamage::Point"))
	TArray<AActor*> IgnoredActors;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MagicalDamage = 0.f;
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FArmorInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorType ArmorType = EArmorType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorPart ArmorPart = EArmorPart::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float PhysicalDefence = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	float MagicalDefence = 0.0f;
};

USTRUCT(BlueprintType, meta = (ExposeOnSpawn))
struct FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="0"))
	int ItemID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DTItemName = FName("NoName");
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType = EItemType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemGrade ItemGrade = EItemGrade::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATDSItemBase> BaseClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	FWeaponInfo Weapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ItemType == EItemType::Armor", EditConditionHides))
	FArmorInfo Armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ItemType == EItemType::Projectile", EditConditionHides))
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (EditCondition="ItemType == EItemType::Item", EditConditionHides))
	bool bIsStackable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin="1", EditCondition="bIsStackable == true"))
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

	void StopSpawnBullet();
	void StartSpawnBullet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	UNiagaraComponent* NiagaraFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	FItemInfo ItemInfo;

	FORCEINLINE FItemInfo GetItemInfo() { return ItemInfo; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	FName SpawnedName = FName("NoName");

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnWeaponFire OnWeaponFire;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnProjectileHit OnProjectileHit;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Delegate")
	FOnWeaponAttack OnWeaponAttack;

	bool bIsClicked = false;
	FTimerHandle AttackTimer;
	float AttackRate = 0.f;

private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);
};
