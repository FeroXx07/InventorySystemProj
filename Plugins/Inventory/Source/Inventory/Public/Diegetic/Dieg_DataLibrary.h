// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dieg_DataLibrary.generated.h"

class UDieg_ItemFragment;
class UDieg_ItemDefinitionDataAsset;

/**
 * @brief Blueprint function library for the diegetic inventory system.
 * 
 * UDieg_DataLibrary provides static utility functions that can be called
 * from Blueprint graphs. This library contains common operations and
 * helper functions used throughout the diegetic inventory system.
 * 
 * @note This is a Blueprint function library and all functions should be
 * marked as UFUNCTION(BlueprintCallable) to be accessible from Blueprint.
 * 
 * @see UBlueprintFunctionLibrary
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_DataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

/**
 * @brief Enumeration defining the possible states of an inventory slot.
 * 
 * EDieg_SlotStatus represents the visual and interactive state of a slot
 * in the inventory grid. Different statuses can have different colors,
 * behaviors, and interaction capabilities.
 * 
 * @note This enum is Blueprint-compatible and can be used in Blueprint graphs.
 * 
 * @see UDieg_Slot
 * @see UDieg_Grid
 * 
 * @since 1.0
 */
UENUM(BlueprintType)
enum class EDieg_SlotStatus : uint8
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

/**
 * @brief Enumeration defining text placement locations for UI elements.
 * 
 * EDieg_TextLocation specifies where text should be positioned relative to
 * a UI element or slot. This is used for displaying item information,
 * quantities, or other text overlays.
 * 
 * @note This enum is Blueprint-compatible and can be used in Blueprint graphs.
 * 
 * @see UDieg_Slot
 * @see ADieg_WorldItemActor
 * 
 * @since 1.0
 */
UENUM(BlueprintType)
enum class EDieg_TextLocation : uint8
{
	/** @brief No text placement */
	None UMETA(DisplayName = "None"),
	
	/** @brief Text positioned at the top-right corner */
	TopRight UMETA(DisplayName = "Top Right"),
	
	/** @brief Text positioned at the top-left corner */
	TopLeft UMETA(DisplayName = "Top Left"),
	
	/** @brief Text positioned at the bottom-right corner */
	BottomRight UMETA(DisplayName = "Bottom Right"),
	
	/** @brief Text positioned at the bottom-left corner */
	BottomLeft UMETA(DisplayName = "Bottom Left"),
};

