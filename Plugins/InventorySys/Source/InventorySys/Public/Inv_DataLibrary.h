// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_DataLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_DataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EInv_SlotState : uint8
{
	/** @brief Default state with no special behavior */
	None UMETA(DisplayName = "None"),
	
	/** @brief Slot is being hovered by the mouse cursor */
	Hover UMETA(DisplayName = "Hover"),
	
	/** @brief Slot is being pressed/clicked */
	Pressed UMETA(DisplayName = "Pressed"),
	
	/** @brief Slot is locked and cannot be interacted with */
	Locked UMETA(DisplayName = "Locked"),
	
	/** @brief Slot is occupied by an item */
	Occupied UMETA(DisplayName = "Occupied"),
};
