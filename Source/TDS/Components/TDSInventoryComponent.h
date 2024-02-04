// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "Components/ActorComponent.h"
#include "TDSInventoryComponent.generated.h"

class ABaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFindItem, FMyItemInfo, ItemInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCountChange, int, ItemCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReloadEnd, int, Magazine, int, InventoryBullet);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDS_API UTDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDSInventoryComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FMyItemInfo> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="WeaponInventory")
	TArray<FMyItemInfo> WeaponInventory;

	UPROPERTY(BlueprintAssignable)
	FOnFindItem OnFindItem;

	UPROPERTY(BlueprintAssignable)
	FOnCountChange OnCountChange;

	UPROPERTY(BlueprintAssignable)
	FOnReloadEnd OnReloadEnd;

	UFUNCTION()
	void OverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	ABaseCharacter* ComponentOwner();
	void AddItem(ATDSItemBase* Item);
	int FindInventoryItemById(int aId);
	int FindWeaponItemById(int aId);
	bool CheckBullets(int ProjectileId);
	void DecreaseInventoryCount(FMyItemInfo WeaponInfo);
	bool FoundAround = false;
};
