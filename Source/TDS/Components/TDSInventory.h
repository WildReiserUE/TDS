#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "Components/ActorComponent.h"
#include "TDSInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFindItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulletsEnd);

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
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FOnPlayerFindItem OnPlayerFindItem;
	
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Inventory")
	FOnBulletsEnd OnBulletsEnd;

	UFUNCTION()
	void OverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	void AddItem(ATDSItemBase* Item);
	int FindItemById(int aId);
	void DecreaseCount(int WeaponBulletId);
	bool CheckCount(int WeaponBulletId);
	bool FoundAround = false;
};