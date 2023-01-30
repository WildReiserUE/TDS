#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TDS/AI/TDSAICharacter.h"
#include "TDSSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnedPair
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	FName SpawnedName = FName("NoName");
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATDSAICharacter> SpawnedClass;
};

USTRUCT(BlueprintType)
struct FSpawnerInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnedPair> SpawnedMap;
	UPROPERTY(EditDefaultsOnly)
	float SpawnDelay = 0.f;
	UPROPERTY(EditDefaultsOnly)
	int SpawnCount = 0;
	UPROPERTY(EditDefaultsOnly)
	float FirstSpawnDelay = 0.f;

};

UCLASS()
class TDS_API ATDSSpawner : public AActor
{
	GENERATED_BODY()

public:
	ATDSSpawner();

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerComponent;

	void SpawnMobs(FSpawnerInfo Info);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnerInfo SpawnerInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> NamesArray;

	int MaxSpawnCount = 0;

protected:
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimer;

	UFUNCTION()
	void PlayerOver(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

};
