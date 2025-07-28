// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "BPF_PlugInv_DataLibrary.generated.h"

class UPlugInv_InventoryItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_DataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

/** Enum for item categories **/
UENUM(BlueprintType)
enum class EPlugInv_ItemCategory : uint8
{
	None,
	Equippable UMETA(DisplayName = "Equippable"),
	Consumable UMETA(DisplayName = "Consumable"),
	Craftable UMETA(DisplayName = "Craftable"),
};

/** Struct to manage slots availabilities **/
USTRUCT()
struct FPlugInv_SlotAvailability
{
	GENERATED_BODY()
	FPlugInv_SlotAvailability() {}
	
	FPlugInv_SlotAvailability(int32 Index, int32 AmountToFill, bool bItemAtIndex)
		: Index(Index),
		  AmountToFill(AmountToFill),
		  bItemAtIndex(bItemAtIndex)
	{
	}

	// Index of the slot, default -1
	int32 Index{INDEX_NONE};

	// Amount of stacks that can take until full
	int32 AmountToFill{0};

	// Flag for slot occupied
	bool bItemAtIndex{false};
};

/** Struct to manage items additions **/
USTRUCT()
struct FPlugInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FPlugInv_SlotAvailabilityResult() {}

	// Weak ptr to the item to add.
	TWeakObjectPtr<UPlugInv_InventoryItem> Item;
	
	// How much of it can be added into the stack, taking into account the current room.
	int32 TotalRoomToFill{0};

	// The part that cannot be added because it is full
	int32 Remainder{0};

	// Flag for stackability
	bool bStackable{false};

	// Container for the current slots
	TArray<FPlugInv_SlotAvailability> SlotAvailabilities;
};

