#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TDSItemBase.generated.h"

class ATDSItemBase;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item,
	Weapon,
	Armor,
	Arrow
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Light,
	Heavy,
	Magic
};

UENUM(BlueprintType)
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

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	RightHand,
	LeftHand,
	DoubleHand,
	Bow
};

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Knife,
	Sword,
	Blunt,
	DualSword,
	Polearm,
	Bow,
	Shield
};

UENUM(BlueprintType)
enum class EWeaponAttackSpeed : uint8
{
	VerySlow,
	Slow,
	Normal,
	Fast,
	VeryFast
};

UENUM(BlueprintType)
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

USTRUCT(BlueprintType)
struct FArrowInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemGrade ArrowGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* HitFX = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound = nullptr;
};

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsStackable = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="bIsStackable"))
	int Count = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Weight = 0;
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponClass WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition="WeaponClass == EWeaponClass::Bow", EditConditionHides))
	TSubclassOf<ATDSItemBase> WeaponArrow = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemGrade WeaponGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponAttackSpeed AttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int PhysicalDamage = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MagicalDamage = 0;
};

USTRUCT(BlueprintType)
struct FArmorInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorType ArmorType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArmorPart ArmorPart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemGrade ArmorGrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PhysicalDefence = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MagicalDefence = 0.0f;	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings")
	FItemInfo ItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Weapon", EditConditionHides))
	FWeaponInfo WeaponInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Armor", EditConditionHides))
	FArmorInfo ArmorInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ItemSettings", meta = (EditCondition="ItemType == EItemType::Arrow", EditConditionHides))
	FArrowInfo ArrowInfo;
};