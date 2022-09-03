// Created WildReiser @2022


#include "TDSArrowBase.h"


// Sets default values
ATDSArrowBase::ATDSArrowBase(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDSArrowBase::BeginPlay(){
	Super::BeginPlay();
	
}

// Called every frame
void ATDSArrowBase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

}