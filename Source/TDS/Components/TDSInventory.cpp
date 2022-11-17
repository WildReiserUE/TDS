// Created WildReiser ©2022

#include "TDSInventory.h"
#include "BaseCharacter.h"
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

ABaseCharacter* UTDSInventory::ComponentOwner()
{
	const auto ComponentOwner = Cast<ABaseCharacter>(this->GetOwner());
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

int UTDSInventory::GetWeaponIndex(FItemInfo ItemInfo)
{
	return WeaponInventory.Find(&ItemInfo);
}

void UTDSInventory::AddItem(ATDSItemBase* Item)
{
	const int i = FindItemById(Item->ItemInfo.ItemID);
	if (i == INDEX_NONE)									//если такого предмета нет
	{
		Inventory.Add(Item->ItemInfo);						//добавляем в общий инвентарь
		UE_LOG(LogTemp,Log,TEXT("---AAA--- %x"), &Inventory.Last());
		if(Item->ItemInfo.ItemType == EItemType::Weapon)	//если оружие добавляем в список оружия  TODO:DuplicateWeapon?
    	{
    		WeaponInventory.Add(&Inventory.Last());
			UE_LOG(LogTemp,Log,TEXT("---BBB--- %x"), WeaponInventory.Last());
    	}
		OnFindItem.Broadcast(Item->ItemInfo);
	}
	else
	{														//если такой предмет есть
		if (Item->ItemInfo.bIsStackable)
		{
			Inventory[i].Count += Item->ItemInfo.Count;		//складываем если пачкуется
			OnCountChange.Broadcast(Inventory[i].Count);	//уведомляем об изменении количества
		}
		else
		{
			Inventory.Add(Item->ItemInfo);					//добавляем в общий инвентарь			 если не пачкуется
			OnFindItem.Broadcast(Item->ItemInfo);
		}
	}
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

bool UTDSInventory::CheckBullets(int ProjectileId)
{
	int i = FindItemById(ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY RELOAD BULLET ID: %i ---HET--- COBCEM"), i);
		return false;
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY RELOAD BULLET ID: %i ------ ECTb"), i);
		return true;
	}		
}

void UTDSInventory::DecreaseCount(FItemInfo WeaponInfo)
{	
	int i = FindItemById(WeaponInfo.Weapon.ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp,Warning,TEXT("TRY DECREASE INVENTORY BULLET ---HET--- COBCEM"));
	}
	else
	{
		if(Inventory[i].Count >= 1)
		{
			UE_LOG(LogTemp,Warning,TEXT("DECREASE BULLET FROM INVENTORY ELAPSED: -- %d  --"), Inventory[i].Count);
			int Result = Inventory[i].Count - (ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.MaxMagazine - ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine);
			UE_LOG(LogTemp,Warning,TEXT("DECREASE RESULT BULLET: -- %d  --"), Inventory[i].Count);
			if(Result <= 0)
			{
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine = Inventory[i].Count + ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine;
				Inventory.RemoveAt(i);
				OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, 0);
			}
			else
			{
				Inventory[i].Count = Result;
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine = WeaponInfo.Weapon.MaxMagazine;
				OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, Inventory[i].Count);
			}
			//ComponentOwner()->FireOn();
		}
		else //пустой слот патрона
		{
			UE_LOG(LogTemp,Warning,TEXT("FIRE BULLET ECTb HO = 0"));
		}
	}
}