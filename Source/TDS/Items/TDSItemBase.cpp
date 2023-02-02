// Created WildReiser ©2022

#include "TDSItemBase.h"
#include "PlayerCharacter.h"
#include "TDSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Damage.h"

ATDSItemBase::ATDSItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMeshComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(ItemMeshComponent);
	ItemMeshComponent->SetCanEverAffectNavigation(false);
	ItemMeshComponent->OnClicked.AddUniqueDynamic(this, &ATDSItemBase::SomeClicked);
	ItemMeshComponent->OnBeginCursorOver.AddUniqueDynamic(this, &ATDSItemBase::RenderOn);
	ItemMeshComponent->OnEndCursorOver.AddUniqueDynamic(this, &ATDSItemBase::RenderOff);
	NiagaraFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraFX->bAutoActivate = false;
	NiagaraFX->SetupAttachment(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->Velocity = FVector(0);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// void ATDSItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
// 	ChangeSettings();
// }

void ATDSItemBase::ChangeSettings()
{
	if (ItemInfo.ItemMesh)
	{
		ItemMeshComponent->SetStaticMesh(ItemInfo.ItemMesh);
	}
	else
		ItemMeshComponent->SetStaticMesh(nullptr);
	
	if(ItemInfo.ItemType == EItemType::Weapon)
	{
		CAttackRate = 60.f / ItemInfo.Weapon.AttackRate; 
		ItemInfo.Weapon.AttackRate = CAttackRate;
		UE_LOG(LogTemp, Log, TEXT("---WEAPON RATE---   %f"), CAttackRate);
	}
}

void ATDSItemBase::SpawnParticleFx(UParticleSystem* NewParticle)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void ATDSItemBase::SpawnSoundHit(USoundBase* NewSound)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NewSound, GetOwner()->GetActorLocation());
}

void ATDSItemBase::RenderOn(UPrimitiveComponent* pComponent)
{
	if (pComponent)
		pComponent->SetRenderCustomDepth(true);
}

void ATDSItemBase::RenderOff(UPrimitiveComponent* pComponent)
{
	if (!bIsClicked)
		if (pComponent)pComponent->SetRenderCustomDepth(false);
}

void ATDSItemBase::SomeClicked(UPrimitiveComponent* pComponent, FKey pKey)
{
	UE_LOG(LogViewport, Display, TEXT("SOMEBODY CLICK ME"));
	bIsClicked = true;
	RenderOn(this->ItemMeshComponent);
}

void ATDSItemBase::StartSpawnBullet()
{
	if (ItemInfo.Weapon.bCanFire)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SocketLocation = this->ItemMeshComponent->GetSocketLocation(FName("BulletSocket"));
		FTransform SpawnPoint = FTransform(FRotator(0), SocketLocation, FVector(1));
		auto Spawned = GetWorld()->SpawnActorDeferred<ATDSItemBase>(ItemInfo.Weapon.WeaponProjectile,
																				SpawnPoint,
																				this,
																				nullptr,
																				SpawnParams.SpawnCollisionHandlingOverride);
		Spawned->SpawnedName = ItemInfo.Weapon.ProjectileName;
		UGameplayStatics::FinishSpawningActor(Spawned, SpawnPoint);
		Spawned->ChangeSettings();
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ItemInfo.Weapon.ShootSound, GetActorLocation());
		ItemInfo.Weapon.Magazine --;
		OnWeaponFire.Broadcast(ItemInfo);
	}
	else if (ItemInfo.Weapon.WeaponClass == EWeaponClass::Handle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPON - MELEEE ATTACK !!!"));
		OnWeaponAttack.Broadcast();
	}
}

//If object shoting without Player
void ATDSItemBase::StopSpawnBullet()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
}

void ATDSItemBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeSettings();
	if (ItemInfo.ItemType == EItemType::Projectile)
	{
		ItemMeshComponent->OnComponentHit.AddDynamic(this, &ATDSItemBase::ProjectileHit);
		ProjectileMovementComponent->ProjectileGravityScale = ItemInfo.Projectile.ProjectileGravity;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;

		const APlayerController* PC = GetWorld()->GetFirstPlayerController();
		const APawn* Player = PC->GetPawn(); //player
		if (PC && Player)
		{
			Direction = Player->GetActorForwardVector();
			ProjectileMovementComponent->Velocity = FVector(
				(Direction.X * ItemInfo.Projectile.ProjectileSpeed),
				(Direction.Y * ItemInfo.Projectile.ProjectileSpeed),
				0);
			ProjectileMovementComponent->InitialSpeed = ItemInfo.Projectile.ProjectileSpeed;
			ProjectileMovementComponent->MaxSpeed = ItemInfo.Projectile.ProjectileMaxSpeed;
		}
	}
}

void ATDSItemBase::ProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	ATDSItemBase* Info = Cast<ATDSItemBase>(this->GetOwner());
	if ((!Player || Info) && Hit.bBlockingHit) //Hit not owner item or item owner
	{
		switch (Info->GetItemInfo().Weapon.ProjectileTypeDamage)
		{
		case EProjectileTypeDamage::Point:
			UGameplayStatics::ApplyDamage(OtherActor,
												Info->ItemInfo.Weapon.PhysicalDamage,
												nullptr,
												this,
												UDamageType::StaticClass());
			break;

		case EProjectileTypeDamage::Radial:
			UGameplayStatics::ApplyRadialDamage(GetWorld(),
												Info->ItemInfo.Weapon.PhysicalDamage,
												Hit.Location,
												Info->ItemInfo.Weapon.DamageRadius,
												UDamageType::StaticClass(),
												Info->ItemInfo.Weapon.IgnoredActors,
												Info);
			break;

		case EProjectileTypeDamage::Visible:
			UGameplayStatics::ApplyRadialDamage(GetWorld(),
												Info->ItemInfo.Weapon.PhysicalDamage,
												Hit.Location,
												Info->ItemInfo.Weapon.DamageRadius,
												UDamageType::StaticClass(),
												Info->ItemInfo.Weapon.IgnoredActors,
												this,
												nullptr,
												true,
												ECollisionChannel::ECC_Visibility);
			break;

		default: break;
		}
		//TODO DamageSensor
		if(Hit.GetActor())
		{
			UAISense_Damage::ReportDamageEvent(GetWorld(),
				Hit.GetActor(),
				GetOwner()->GetOwner(),
				Info->ItemInfo.Weapon.PhysicalDamage,
				GetOwner()->GetOwner()->GetActorLocation(),
				Hit.Location);
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ItemInfo.Projectile.HitParticle, Hit.Location);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ItemInfo.Projectile.HitSound, Hit.Location);
	Destroy();
}
