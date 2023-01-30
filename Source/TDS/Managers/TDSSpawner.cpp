#include "TDSSpawner.h"

#include "PlayerCharacter.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ATDSSpawner::ATDSSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TroggerCollision"));
	TriggerComponent ->SetCanEverAffectNavigation(false);
	//SetRootComponent(TriggerComponent);
	TriggerComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	TriggerComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATDSSpawner::PlayerOver);

}

void ATDSSpawner::BeginPlay()
{
	Super::BeginPlay();
	MaxSpawnCount = SpawnerInfo.SpawnCount;
}

void ATDSSpawner::SpawnMobs(FSpawnerInfo Info)
{
	if(Info.SpawnedMap.Num() < 1) return;
	if(GetWorld())
	{
		if(MaxSpawnCount -- <= 0)
		{
			if(GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer))
			{
				GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
				return;
			}
		}
		FActorSpawnParameters SpawnRules;
		SpawnRules.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector PointLocation = this->GetActorLocation();
		FTransform SpawnPoint = FTransform(FRotator(0), PointLocation, FVector(1));

		int R = UKismetMathLibrary::RandomIntegerInRange(0,Info.SpawnedMap.Num()-1);
		ATDSAICharacter* Spawned = GetWorld()->SpawnActorDeferred<ATDSAICharacter>(
			Info.SpawnedMap[R].SpawnedClass,
			SpawnPoint,
			nullptr,
			nullptr,
			SpawnRules.SpawnCollisionHandlingOverride);
		Spawned->SpawnedName = Info.SpawnedMap[R].SpawnedName;
		UGameplayStatics::FinishSpawningActor(Spawned, SpawnPoint);
		UE_LOG(LogTemp, Warning,TEXT("END SPAWN --- %s"), *Info.SpawnedMap[R].SpawnedName.ToString());
	}
}

void ATDSSpawner::PlayerOver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer) && (OtherActor->IsA(APlayerCharacter::StaticClass())))
	{
		UE_LOG(LogTemp, Warning,TEXT("OVER SPAWNER"));
		GetWorld()->GetTimerManager().SetTimer(
			SpawnTimer,
			[&]() { this->SpawnMobs(SpawnerInfo); },
			SpawnerInfo.SpawnDelay,
			true,
			SpawnerInfo.FirstSpawnDelay);
	}
}

void ATDSSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

