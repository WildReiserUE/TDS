// Created WildReiser ©2022

#include "TDSItemBase.h"
#include "Kismet/GameplayStatics.h"

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
	ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->Velocity = FVector (0);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 0.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ATDSItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ChangeSettings();
}

void ATDSItemBase::ChangeSettings(){
	if(ItemInfo.ItemMesh){
		ItemMeshComponent->SetStaticMesh(ItemInfo.ItemMesh);
	}
	else
		ItemMeshComponent->SetStaticMesh(nullptr);
}

void ATDSItemBase::SpawnParticleFx(UParticleSystem* NewParticle){
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void ATDSItemBase::SpawnSoundHit(USoundBase* NewSound){
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NewSound, GetOwner()->GetActorLocation());
}

void ATDSItemBase::RenderOn(UPrimitiveComponent* pComponent){
	if(pComponent)
		pComponent->SetRenderCustomDepth(true);
}

void ATDSItemBase::RenderOff(UPrimitiveComponent* pComponent){
	if(!bIsClicked)
		if (pComponent)pComponent->SetRenderCustomDepth(false);
}

void ATDSItemBase::SomeClicked(UPrimitiveComponent* pComponent, FKey pKey){
	UE_LOG(LogViewport, Display, TEXT("SOMEBODY CLICK ME"));
	bIsClicked = true;
	RenderOn(this->ItemMeshComponent);
}

void ATDSItemBase::SpawnBullet()
{
	if(ItemInfo.Weapon.bCanFire){
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SocketLocation = this->ItemMeshComponent->GetSocketLocation(FName("BulletSocket"));
        FTransform SpawnPoint = FTransform(FRotator(0),SocketLocation,FVector(1));
        auto Spawned = GetWorld()->SpawnActorDeferred<ATDSItemBase>(ItemInfo.Weapon.WeaponProjectile, SpawnPoint, this,nullptr, SpawnParams.SpawnCollisionHandlingOverride);
        Spawned->SpawnedName = ItemInfo.Weapon.ProjectileName;
        UGameplayStatics::FinishSpawningActor(Spawned,SpawnPoint);
        Spawned->ChangeSettings();
        UE_LOG(LogTemp, Warning, TEXT("PROJECTILE OWNER IS: %s"), *Spawned->GetOwner()->GetName());
        OnWeaponFire.Broadcast();
	}		
	else if(ItemInfo.Weapon.WeaponClass == EWeaponClass::Handle){
		UE_LOG(LogTemp, Warning, TEXT("WEAPON - MELEEE ATTACK !!!"));
		OnWeaponAttack.Broadcast();
	}
}

void ATDSItemBase::StopAttack(){
	if(GetWorld()->GetTimerManager().IsTimerActive(AttackTimer)){
		GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	}
}

void ATDSItemBase::BeginPlay()
{
	Super::BeginPlay();
	if(ItemInfo.ItemType == EItemType::Projectile)
	{
		ProjectileMovementComponent->ProjectileGravityScale = ItemInfo.Projectile.ProjectileGravity;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		FVector Direction = FVector(0);
		auto PC = GetWorld()->GetFirstPlayerController();
		if(PC)
		{
			auto Player = PC->GetPawn(); //player
			{
				Direction = Player->GetActorForwardVector();
				ProjectileMovementComponent->Velocity = FVector((Direction.X*ItemInfo.Projectile.ProjectileSpeed),(Direction.Y*ItemInfo.Projectile.ProjectileSpeed), 0);
                UE_LOG(LogTemp, Warning, TEXT("PROJECTILE VELOCITY = %f"), ProjectileMovementComponent->Velocity.Y);
                ProjectileMovementComponent->InitialSpeed = ItemInfo.Projectile.ProjectileSpeed;
                ProjectileMovementComponent->MaxSpeed = ItemInfo.Projectile.ProjectileMaxSpeed;
			}
		}
	}
	if(ItemInfo.ItemType == EItemType::Weapon){
		switch (ItemInfo.Weapon.AttackSpeed)
		{
		case EWeaponAttackSpeed::VerySlow:
			AttackRate = 2.f;break;
		case EWeaponAttackSpeed::Slow:
			AttackRate = 1.5f; break;
		case  EWeaponAttackSpeed::Normal:
			AttackRate = 1.f; break;
		case EWeaponAttackSpeed::Fast:
        	AttackRate = 0.5f; break;
		case EWeaponAttackSpeed::VeryFast:
			AttackRate = 0.25f; break;
		default:break;
		}
	}
}