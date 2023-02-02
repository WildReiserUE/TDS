// Created WildReiser ©2022

#include "TDSInventoryComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

UTDSInventoryComponent::UTDSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!ComponentOwner()) return;
	ComponentOwner()->OnActorBeginOverlap.AddDynamic(this, &UTDSInventoryComponent::OverlapItem);
	ComponentOwner()->OnActorEndOverlap.AddDynamic(this, &UTDSInventoryComponent::EndOverlapItem);
}

void UTDSInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ABaseCharacter* UTDSInventoryComponent::ComponentOwner()
{
	const auto ComponentOwner = Cast<ABaseCharacter>(this->GetOwner());
	return ComponentOwner ? ComponentOwner : nullptr;
}

void UTDSInventoryComponent::OverlapItem(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		ATDSItemBase* BaseItem = Cast<ATDSItemBase>(OtherActor);
		if (BaseItem && BaseItem->ItemInfo.ItemType != EItemType::Projectile)
		{
			FoundAround = true;
			AddItem(BaseItem);
		}
	}
}

void UTDSInventoryComponent::EndOverlapItem(AActor* OverlappedActor, AActor* OtherActor)
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

void UTDSInventoryComponent::AddItem(ATDSItemBase* Item)
{
	const int i = FindInventoryItemById(Item->ItemInfo.ItemID);
	const int w = FindWeaponItemById(Item->ItemInfo.ItemID);
	if (i == INDEX_NONE && w == INDEX_NONE) //если такого предмета нет нигде
	{
		switch (Item->ItemInfo.ItemType) //если оружие добавляем в список оружия
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
	else if(i != INDEX_NONE) //если предмет есть в инвентаре
	{
		if (Item->ItemInfo.bIsStackable)
		{
			Inventory[i].Count += Item->ItemInfo.Count; //складываем если пачкуется
			UE_LOG(LogTemp, Log, TEXT("---ADD %i COUNT TO INVENTORY ITEM---"), Inventory[i].Count);
			OnCountChange.Broadcast(Inventory[i].Count); //уведомляем об изменении количества
		}
		else
		{
			Inventory.Add(Item->ItemInfo); //добавляем в общий инвентарь
			UE_LOG(LogTemp, Log, TEXT("---ADD NEW ITEM TO INVENTORY---"));
			OnFindItem.Broadcast(Item->ItemInfo);
		}
	}
	else if(w != INDEX_NONE) //если есть в списке оружия
	{
		UE_LOG(LogTemp, Log, TEXT("---AGAIN WEAPON--- NEED ---ADD BULLETS--- "));
		const int a = FindInventoryItemById(Item->ItemInfo.Weapon.ProjectileId);
		if(a == INDEX_NONE) //не нашли к чему добавить количество
		{
			UE_LOG(LogTemp, Log, TEXT("---INDEX BULLETS NONE--- "));
			//TODO создать-получить структуру патрона основываясь на его имени из информации оружия и добавить в инвентарь
			//Inventory.Add(Item->ItemInfo);
			//OnFindItem.Broadcast(Item->ItemInfo); //уведомляем о добавлении
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("---INDEX XXXX ADD COUNT--- "));
			Inventory[a].Count += Item->ItemInfo.Weapon.MaxMagazine;
			OnCountChange.Broadcast(Inventory[a].Count); //уведомляем об изменении количества
		}
	}
	Item->Destroy();
}

int UTDSInventoryComponent::FindInventoryItemById(int aId)
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

int UTDSInventoryComponent::FindWeaponItemById(int aId)
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

bool UTDSInventoryComponent::CheckBullets(int ProjectileId)
{
	int i = FindInventoryItemById(ProjectileId);
	if (i == INDEX_NONE) //если элемента нет
	{
		UE_LOG(LogTemp, Warning, TEXT("TRY RELOAD BULLET ID: %i ---HET--- COBCEM"), i);
		return false;
	}
	else
	{
		if(Inventory[i].Count >0)
		{
			UE_LOG(LogTemp, Warning, TEXT("TRY RELOAD BULLET: ECTb   ---   %i"), Inventory[i].Count);
			return true;
		}
		else
		{
			return false;
		}
	}
}

void UTDSInventoryComponent::DecreaseInventoryCount(FItemInfo WeaponInfo)
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
			UE_LOG(LogTemp, Warning, TEXT("DECREASE BULLET FROM INVENTORY --AVAIBLE: -- %d  --"), Inventory[i].Count);
			int Result = Inventory[i].Count -
				(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.MaxMagazine -
					ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine);
			UE_LOG(LogTemp, Warning, TEXT("DECREASE BULLET FROM INVENTORY --ELAPSED: -- %d  --"), Result);
			if (Result <= 0)
			{
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine =
					Inventory[i].Count + ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine;
				Inventory[i].Count = 0;
				//Inventory.RemoveAt(i);
				//OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, Inventory[i].Count );
			}
			else
			{
				Inventory[i].Count = Result;
				ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine = WeaponInfo.Weapon.MaxMagazine;
			}
			OnReloadEnd.Broadcast(ComponentOwner()->CurrentWeapon->ItemInfo.Weapon.Magazine, Inventory[i].Count);
		}
		else //пустой слот патрона
		{
			UE_LOG(LogTemp, Warning, TEXT("FIRE BULLET ECTb HO = 0"));
		}
	}
}
