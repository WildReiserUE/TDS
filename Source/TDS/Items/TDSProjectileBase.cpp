// Created WildReiser @2022


#include "TDSProjectileBase.h"


// Sets default values
ATDSProjectileBase::ATDSProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDSProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDSProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}