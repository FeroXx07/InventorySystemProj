// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "AC_PlugInv_EquipActor.generated.h"

UCLASS()
class INVENTORY_API APlugInv_EquipActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlugInv_EquipActor();

	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(FGameplayTag Type) { EquipmentType = Type; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag EquipmentType;
};
