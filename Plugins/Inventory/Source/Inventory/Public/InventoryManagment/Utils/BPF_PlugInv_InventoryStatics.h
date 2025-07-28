// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_InventoryStatics.generated.h"

class UPlugInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UPlugInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);
};
