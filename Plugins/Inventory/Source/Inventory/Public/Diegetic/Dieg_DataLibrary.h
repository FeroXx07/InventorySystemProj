// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dieg_DataLibrary.generated.h"

class UDieg_ItemFragment;
class UDieg_ItemDefinitionDataAsset;
/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_DataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EDieg_SlotStatus : uint8
{
	None,
	Hover UMETA(DisplayName = "Hover"),
	Pressed UMETA(DisplayName = "Pressed"),
	Locked UMETA(DisplayName = "Locked"),
	Occupied UMETA(DisplayName = "Occupied"),
};

UENUM(BlueprintType)
enum class EDieg_TextLocation : uint8
{
	None,
	TopRight UMETA(DisplayName = "Top Right"),
	TopLeft UMETA(DisplayName = "Top Left"),
	BottomRight UMETA(DisplayName = "Bottom Right"),
	BottomLeft UMETA(DisplayName = "Bottom Left"),
};

