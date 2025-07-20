// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_DataLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_DataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EPlugInv_ItemCategory : uint8
{
	Equippable UMETA(DisplayName = "Equippable"),
	Consumable UMETA(DisplayName = "Consumable"),
	Craftable UMETA(DisplayName = "Craftable"),
};
