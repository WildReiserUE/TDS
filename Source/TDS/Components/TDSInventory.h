#pragma once

#include "CoreMinimal.h"
#include "TDSItemBase.h"
#include "Components/ActorComponent.h"
#include "TDSInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFindItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UTDSInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTDSInventory();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Inventory")
	TArray<FItemInfo> Inventory;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category="Health")
	FOnPlayerFindItem OnPlayerFindItem;

	UFUNCTION()
	void OverlapItem(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
	void EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor);

	void AddItem(ATDSItemBase* Item);
	int FindItemById(int aId);
	bool FoundAround = false;
};