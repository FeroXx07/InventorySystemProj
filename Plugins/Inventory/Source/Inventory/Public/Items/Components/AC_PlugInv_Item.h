// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_PlugInv_Item.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_Item : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlugInv_Item();

	FString GetPickupMessage() const {return PickupMessage;}

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FString PickupMessage;
};
