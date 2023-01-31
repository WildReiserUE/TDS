// Created WildReiser ©2022

#include "TDSInventory.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

UTDSInventory::UTDSInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSInventory::BeginPlay()
{
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
	return ComponentOwner ? ComponentOwner : nullptr;
}

void UTDSInventory::OverlapItem(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor)
	{
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if (BaseItem && BaseItem->ItemInfo.ItemType != EItemType::Projectile)
		{
			FoundAround = true;
			AddItem(BaseItem);
		}
	}
}

void UTDSInventory::EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OverlappedActor)
	{
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if (BaseItem)
		{
			FoundAround = false;
		}
	}
}

int UTDSInventory::GetWeaponIndex(FItemInfo ItemInfo)
{
	int n = INDEX_NONE;
	int i = 0;
	for (FItemInfo aItem : WeaponInventory)
	{
		if (aItem.ItemType == EItemType::Weapon)
		{
			n = i;
			break;
		}
		i++;
	}
	return n;
}

void UTDSInventory::AddItem(ATDSItemBase* Item)
{
	const int i = FindInventoryItemById(Item->ItemInfo.ItemID);
	const int y = FindWeaponItemById(Item->ItemInfo.ItemID);
	if (i == INDEX_NONE && y == INDEX_NONE) //если такого предмета нет нигде
	{
		switch (Item->ItemInfo.ItemType) //если оружие добавляем в список оружия если такого оружия нет
		{
		case EItemType::Weapon:
			{
				UE_LOG(LogTemp, Log, TEXT("---ADD NEW--- WEAPON ITEM--- "));
				WeaponInventory.Add(Item->ItemInfo); //добавляем в инвентарь оружия
				OnFindItem.Broadcast(Item->ItemInfo);
				break;
			}
		default:
			{
				Inventory.Add(Item->ItemInfo); //добавляем в общий инвентарь
				UE_LOG(LogTemp, Log, TEXT("---ADD NEW--- INVENTORY ITEM---"));
				OnFindItem.Broadcast(Item->ItemInfo);
				break;
			}
		}
	}
	else
	{
		//если такой предмет есть
		if (Item->ItemInfo.bIsStackable)
		{
			Inventory[i].Count += Item->ItemInfo.Count; //складываем если пачкуется
			UE_LOG(LogTemp, Log, TEXT("---ADD %i COUNT TO INVENTORY ITEM---"), Inventory[i].Count);
			OnCountChange.Broadcast(Inventory[i].Count); //уведомляем об изменении количества
		}
		else
		{
			switch (Item->ItemInfo.ItemType)
			{
			case EItemType::Weapon:
				{
					UE_LOG(LogTemp, Log, TEXT("---AGAIN WEAPON--- NEED ---ADD BULLETS--- %i"),
					       Item->ItemInfo.Weapon.MaxMagazine);
					break;
				}
			case EItemType::Armor:
				{
					UE_LOG(LogTemp, Log, TEXT("---AGAIN ARMOR---   --- WHAT TO DO ??? ---"));
					break;
				}
			default:
				{
					Inventory.Add(Item->ItemInfo); //добавляем в общий инвентарь любой тип
					//кроме описаных правил если не пачкуется
					OnFindItem.Broadcast(Item->ItemInfo);
					break;
				}
			}
		}
	}
	Item->Destroy();
}

int UTDSInventory::FindInventoryItemById(int aId)
{
	int n = INDEX_NONE;
	int i = 0;
	for (FItemInfo aItem : Inventory)
	{
		if (aItem.ItemID == aId)
		{
			n = i;
			break;
		}
		i++;
	}
	return n;
}

int UTDSInventory::FindWeaponItemById(int aId)
{
	int n = INDEX_NONE;
	int i = 0;
	for (FItemInfo aItem : WeaponInventory)
	{
		if (aItem.ItemID == aId)
		{
			n = i;
			break;
		}
		i++;
	}
	return n;
}

bool UTDSInventory::CheckBullets(int ProjectileId)
{
	int i = FindInventoryItemById(ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp, Warning, TEXT("TRY RELOAD BULLET ID: %i ---HET--- COBCEM"), i);
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TRY RELOAD BULLET ID: %i ------ ECTb"), i);
		return true;
	}
}

void UTDSInventory::DecreaseInventoryCount(FItemInfo WeaponInfo)
{
	int i = FindInventoryItemById(WeaponInfo.Weapon.ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp, Warning, TEXT("TRY DECREASE INVENTORY BULLET ---HET--- COBCEM"));
	}
	else
	{
		if (Inventory[i].Count >= 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("DECREASE BULLET FROM INVENTORY ELAPSED: -- %d  --"), Inventory[i].Count);
			int Result = Inventory[i].Count -
				(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.MaxMagazine -
					ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine);
			UE_LOG(LogTemp, Warning, TEXT("DECREASE BULLET FROM INVENTORY: -- %d  --"), Inventory[i].Count);
			if (Result <= 0)
			{
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine =
					Inventory[i].Count + ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine;
				Inventory.RemoveAt(i);
				OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, 0);
			}
			else
			{
				Inventory[i].Count = Result;
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine = WeaponInfo.Weapon.MaxMagazine;
				OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, Inventory[i].Count);
			}
			//ComponentOwner()->FireOn(); //TODO resume fire? 
		}
		else //пустой слот патрона
		{
			UE_LOG(LogTemp, Warning, TEXT("FIRE BULLET ECTb HO = 0"));
		}
	}
}
