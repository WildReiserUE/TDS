// Created WildReiser ©2022

#include "TDSInventory.h"
#include "Kismet/GameplayStatics.h"

UTDSInventory::UTDSInventory(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSInventory::BeginPlay(){
	Super::BeginPlay();
	const auto ComponentOwner = GetOwner();
	if (!ComponentOwner) return;
	ComponentOwner->OnActorBeginOverlap.AddDynamic(this, &UTDSInventory::OverlapItem);
	ComponentOwner->OnActorEndOverlap.AddDynamic(this, &UTDSInventory::EndOverlapItem);
}

void UTDSInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTDSInventory::OverlapItem(AActor* OverlappedActor, AActor* OtherActor){
	if(OverlappedActor)
	{
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if(BaseItem){
			FoundAround=true;
			switch (BaseItem->ItemInfo.ItemType)
			{
			case EItemType::Item:
				AddItem(BaseItem);break;
			case EItemType::Weapon:
				AddItem(BaseItem);break;
			case EItemType::Armor:
				AddItem(BaseItem);break;
			default:
				break;
			}
		}
	}
}

void UTDSInventory::EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OverlappedActor){
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if(BaseItem)
		{
			FoundAround=false;
		}
	}
}

void UTDSInventory::AddItem(ATDSItemBase* Item)
{
	const int i = FindItemById(Item->ItemInfo.ItemID);
	if (i == INDEX_NONE){
		FItemInfo NewItem;
		NewItem = Item->ItemInfo;
		Inventory.Add(NewItem);							//добавляем в общий инвентарь
		if(NewItem.ItemType ==  EItemType::Weapon){
			FItemInfo NewWeaponItem;
			NewWeaponItem = Item->ItemInfo;
			WeaponInventory.Add(NewWeaponItem);			//если оружие добавляем в список оружия
		}
	}
	else{
		if (Item->ItemInfo.bIsStackable){
			Inventory[i].Count += Item->ItemInfo.Count;	//складываем если такой предмет есть и пачкуется
		}
		else{
			FItemInfo NewItem;
			NewItem = Item->ItemInfo;
			Inventory.Add(NewItem);						//добавляем в общий инвентарь если не пачкуется
			if(NewItem.ItemType ==  EItemType::Weapon){
				FItemInfo NewWeaponItem;
				NewWeaponItem = Item->ItemInfo;
				WeaponInventory.Add(NewWeaponItem);		//если оружие добавляем в список оружия
			}
		}
	}
	OnPlayerFindItem.Broadcast(Item->ItemInfo);			//сообщаем что нашли новый предмет
	Item->Destroy();
}

int UTDSInventory::FindItemById(int aId){
	int n = INDEX_NONE;
	int i = 0;
	for (FItemInfo aItem : Inventory){
		if (aItem.ItemID == aId){
			n = i;
			break;
		}
		i++;
	}
	return n;
}

void UTDSInventory::DecreaseCount(int WeaponBulletId)
{	
	int i = FindItemById(WeaponBulletId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY DECREASE BULLET ---HET--- COBCEM"));
		OnBulletsEnd.Broadcast();
	}
	else
	{
		if(Inventory[i].Count >= 1)
		{
			UE_LOG(LogTemp,Warning,TEXT("TRY DECREASE BULLET ---OK--- ELAPSED: -- %d  --"), Inventory[i].Count);
			Inventory[i].Count -= 1;
			if(Inventory[i].Count == 0)
			{
				Inventory.RemoveAt(i);
				OnBulletsEnd.Broadcast();
			}
		}
		else //по логике сюда не зайдем никогда... но мало ли...
		{
			UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET ECTb HO SHOT = 0"));
			OnBulletsEnd.Broadcast();
		}
		OnBulletsChanged.Broadcast(Inventory[i].Count);
	}
}

bool UTDSInventory::CheckCount(int WeaponBulletId)
{
	bool BulletAviable = false;
	int InventoryBulletCount = 0;
	int i = FindItemById(WeaponBulletId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET: --- HET COBCEM ---"));
		BulletAviable = false;
	}
	else
	{
		if(Inventory[i].Count >= 1) // рабочий элемент, нашли
		{
			UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET: --- ECTb --- %d --- COUNT "), Inventory[i].Count);
			InventoryBulletCount = Inventory[i].Count;
			BulletAviable =  true;
		}
		else // элемент есть но пустой
		{
		//UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET --- ECTb HO COUNT = 0"));
			BulletAviable = false;
			InventoryBulletCount = 0;
		}
	}
	return BulletAviable;
}