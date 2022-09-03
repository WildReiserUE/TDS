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
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ATDSItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent){
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ChangeSettings();
}

void ATDSItemBase::ChangeSettings(){
	if(ItemInfo.ItemMesh)
	{
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
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderON"));
	//UE_LOG(LogViewport, Display, TEXT("Command to RENDER ON"));
	if(pComponent)
		pComponent->SetRenderCustomDepth(true);
}

void ATDSItemBase::RenderOff(UPrimitiveComponent* pComponent){
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, TEXT("Item: Switch RenderOFF"));
	//UE_LOG(LogViewport, Display, TEXT("Command to RENDER OFF"));
	if(!bIsClicked)
		if (pComponent)pComponent->SetRenderCustomDepth(false);
}

void ATDSItemBase::SomeClicked(UPrimitiveComponent* pComponent, FKey pKey){
	UE_LOG(LogViewport, Display, TEXT("SOMEBODY CLICK ME"));
	bIsClicked = true;
	RenderOn(this->ItemMeshComponent);
}

void ATDSItemBase::BeginPlay(){
	Super::BeginPlay();
	if(ItemInfo.ItemType == EItemType::Projectile)
		UE_LOG(LogViewport, Display, TEXT("PROJECTILE INFO"));
}