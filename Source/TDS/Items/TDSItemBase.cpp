#include "TDSItemBase.h"
#include "Kismet/GameplayStatics.h"

ATDSItemBase::ATDSItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMeshComponent->SetGenerateOverlapEvents(true);
	ItemMeshComponent->SetCanEverAffectNavigation(false);
	SetRootComponent(ItemMeshComponent);
	ItemMeshComponent->OnClicked.AddUniqueDynamic(this, &ATDSItemBase::SomeClicked);
	ItemMeshComponent->OnBeginCursorOver.AddUniqueDynamic(this, &ATDSItemBase::RenderOn);
	ItemMeshComponent->OnEndCursorOver.AddUniqueDynamic(this, &ATDSItemBase::RenderOff);
}

void ATDSItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ChangeSettings();
}

void ATDSItemBase::ChangeSettings()
{
	if(ItemInfo.ItemMesh)
	{
		ItemMeshComponent->SetStaticMesh(ItemInfo.ItemMesh);
	}
	else
		ItemMeshComponent->SetStaticMesh(nullptr);
}

void ATDSItemBase::SpawnParticleFx(UParticleSystem* NewParticle)
{
	if(!GetWorld() || !NewParticle) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NewParticle, GetOwner()->GetActorLocation());
}

void ATDSItemBase::SpawnSoundHit(USoundBase* NewSound){
	if (!GetWorld() || !NewSound) return;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NewSound, GetOwner()->GetActorLocation());
}

void ATDSItemBase::RenderOn(UPrimitiveComponent* pComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Item: Switch RenderON"));
	UE_LOG(LogViewport, Display, TEXT("Command to RENDER ON"));
	if(pComponent)
	{
		pComponent->SetRenderCustomDepth(true);
	}
}

void ATDSItemBase::RenderOff(UPrimitiveComponent* pComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Item: Switch RenderOFF"));
	UE_LOG(LogViewport, Display, TEXT("Command to RENDER OFF"));
	if (pComponent)
	{
		pComponent->SetRenderCustomDepth(false);
	}
}

void ATDSItemBase::SomeClicked(UPrimitiveComponent* pComponent, FKey pKey)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item: SOME Click ME"));
	UE_LOG(LogViewport, Display, TEXT("SOME CLICK ME"));
	bIsClicked = true;
}