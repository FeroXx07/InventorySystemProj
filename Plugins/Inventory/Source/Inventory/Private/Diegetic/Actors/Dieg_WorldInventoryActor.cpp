// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Actors/Dieg_WorldInventoryActor.h"


// Sets default values
ADieg_WorldInventoryActor::ADieg_WorldInventoryActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADieg_WorldInventoryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADieg_WorldInventoryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

