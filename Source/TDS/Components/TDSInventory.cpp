#include "TDSInventory.h"
#include "Kismet/GameplayStatics.h"

UTDSInventory::UTDSInventory(){
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UTDSInventory::BeginPlay(){
	Super::BeginPlay();
	const auto ComponentOwner = GetOwner();
	if (!ComponentOwner) return;
	ComponentOwner->OnActorBeginOverlap.AddDynamic(this, &UTDSInventory::OverlapItem);
	ComponentOwner->OnActorEndOverlap.AddDynamic(this, &UTDSInventory::EndOverlapItem);
}

void UTDSInventory::OverlapItem(AActor* OverlappedActor, AActor* OtherActor){
	ATDSItemBase* BaseActor = Cast<ATDSItemBase>(OtherActor);
	if(BaseActor) //TODO: правило поднятия предмета
	{
		AddItem(BaseActor);
		UE_LOG(LogViewport, Log,TEXT("FOUND ITEM"));
		FoundAround=true;
		//OtherActor->Destroy();
	}		
}

void UTDSInventory::EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor)
		FoundAround=false;
}

void UTDSInventory::AddItem(ATDSItemBase* Item)
{
	const int i = FindItemById(Item->ItemInfo.ItemID);
	if (i == INDEX_NONE)
	{
		FItemInfo NewItem;
		NewItem = Item->ItemInfo;
		Inventory.Add(NewItem);
		OnPlayerFindItem.Broadcast();
	}
	else{
		if (Item->ItemInfo.bIsStackable){
			Inventory[i].Count += Item->ItemInfo.Count;
		}
		else{
			FItemInfo NewItem;
            NewItem = Item->ItemInfo;
            Inventory.Add(NewItem);
		}		
		OnPlayerFindItem.Broadcast();
	}
}

int UTDSInventory::FindItemById(int aId)
{
	int n = INDEX_NONE;
	int i = 0;
	for (FItemInfo aItem : Inventory)	{
		if (aItem.ItemID == aId)
		{
			n = i;
			break;
		}
		i++;
	}
	return n;
}