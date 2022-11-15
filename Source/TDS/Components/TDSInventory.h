// Created WildReiser ©2022

#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "Components/ActorComponent.h"
#include "TDSInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerFindItem,FItemInfo, ItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulletsEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletsChanged,int,Count);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDSInventory();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; 
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<FItemInfo> Inventory;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<FItemInfo> WeaponInventory;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerFindItem OnPlayerFindItem;
	
	UPROPERTY(BlueprintAssignable)
	FOnBulletsEnd OnBulletsEnd;

	UPROPERTY(BlueprintAssignable)
	FOnBulletsChanged OnBulletsChanged;

	UFUNCTION()
	void OverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	AActor* ComponentOwner();
	void AddItem(ATDSItemBase* Item);
	int FindItemById(int aId);
	bool TryReloadWeapon(int ProjectileId);
	void DecreaseCount(int WeaponBulletId);
	bool CheckCount(int WeaponMagazineBullet);
	int FindMagazBullet(int aId);
	bool FoundAround = false;
};