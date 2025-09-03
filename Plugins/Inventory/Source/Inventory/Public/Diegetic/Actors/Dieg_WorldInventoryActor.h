// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldInventoryActor.generated.h"

UCLASS()
class INVENTORY_API ADieg_WorldInventoryActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADieg_WorldInventoryActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
