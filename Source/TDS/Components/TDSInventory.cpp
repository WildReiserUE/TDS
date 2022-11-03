// Created WildReiser ©2022

#include "TDSInventory.h"

#include "Kismet/GameplayStatics.h"

UTDSInventory::UTDSInventory(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSInventory::BeginPlay(){
	Super::BeginPlay();
	if (!ComponentOwner()) return;
	ComponentOwner()->OnActorBeginOverlap.AddDynamic(this, &UTDSInventory::OverlapItem);
	ComponentOwner()->OnActorEndOverlap.AddDynamic(this, &UTDSInventory::EndOverlapItem);
}

void UTDSInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UTDSInventory::ComponentOwner()
{
	const auto ComponentOwner = this->GetOwner();
	return ComponentOwner ? (ComponentOwner) : nullptr;
}

void UTDSInventory::OverlapItem(AActor* OverlappedActor, AActor* OtherActor){
	if(OverlappedActor)
	{
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if(BaseItem && BaseItem->ItemInfo.ItemType != EItemType::Projectile)
		{
			FoundAround=true;
			AddItem(BaseItem);
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
	if (i == INDEX_NONE)								//если такого предмета нет
	{								
		Inventory.Add(Item->ItemInfo);					//добавляем в общий инвентарь
	}
	else
	{													//если такой предмет есть
		if (Item->ItemInfo.bIsStackable)
		{
			Inventory[i].Count += Item->ItemInfo.Count;	//складываем если пачкуется
		}
		else
		{
			Inventory.Add(Item->ItemInfo);				//добавляем в общий инвентарь			 если не пачкуется			
		}
	}
	if(Item->ItemInfo.ItemType ==  EItemType::Weapon)	//если оружие добавляем в список оружия
	{
		FItemInfo NewWeaponItem;
		NewWeaponItem = Item->ItemInfo;
		WeaponInventory.Add(NewWeaponItem);
	}
	OnPlayerFindItem.Broadcast(Item->ItemInfo);		//сообщаем что нашли новый предмет
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

bool UTDSInventory::TryReloadWeapon(int ProjectileId)
{
	int i = FindItemById(ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY RELOAD BULLET ID: %i ---HET--- COBCEM"), i);
		OnBulletsEnd.Broadcast();
		return false;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY RELOAD BULLET ID: %i ------ ECTb"), i);
		return true;
	}
		
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
			Inventory[i].Count -= 1;
			UE_LOG(LogTemp,Warning,TEXT("TRY DECREASE BULLET ---OK--- ELAPSED: -- %d  --"), Inventory[i].Count);
			if(Inventory[i].Count == 0)
			{
				Inventory.RemoveAt(i);
				OnBulletsChanged.Broadcast(0);
				OnBulletsEnd.Broadcast();
			}
			else
				OnBulletsChanged.Broadcast(Inventory[i].Count);
		}
		else //по логике сюда не зайдем никогда... но мало ли...
		{
			UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET ECTb HO SHOT = 0"));
			OnBulletsEnd.Broadcast();
		}		
	}
}

bool UTDSInventory::CheckCount(int WeaponBulletId)
{
	bool BulletAviable;
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
			BulletAviable =  true;
		}
		else // элемент есть но пустой
		{
		//UE_LOG(LogTemp,Warning,TEXT("CHECK BULLET --- ECTb HO COUNT = 0"));
			BulletAviable = false;
		}
	}
	return BulletAviable;
}