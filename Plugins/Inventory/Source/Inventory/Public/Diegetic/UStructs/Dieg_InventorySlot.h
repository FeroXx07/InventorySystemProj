// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_InventorySlot.generated.h"

class UDieg_ItemInstance;

/**
 * @brief Represents a single slot in the diegetic inventory grid system.
 * 
 * FDieg_InventorySlot is the fundamental unit of storage in the diegetic inventory system.
 * Each slot represents a single grid position that can contain an item instance, along with
 * metadata about its position, rotation, and associated tags.
 * 
 * The slot system supports both single-slot and multi-slot items. For multi-slot items,
 * one slot is designated as the "root slot" and all other occupied slots reference it.
 * This allows for efficient management of items that occupy multiple grid positions.
 * 
 * @note This struct is Blueprint-compatible and can be used in Blueprint graphs.
 * 
 * @see UDieg_ItemInstance
 * @see UDieg_InventoryComponent
 * 
 * @since 1.0
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_InventorySlot
{
	GENERATED_BODY()
	
	/**
	 * @brief The item instance stored in this slot.
	 * 
	 * Points to the UDieg_ItemInstance that occupies this slot. For multi-slot items,
	 * this will be the same instance across all occupied slots.
	 * 
	 * @note This is nullptr for empty slots.
	 * 
	 * @see UDieg_ItemInstance
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	TObjectPtr<UDieg_ItemInstance> ItemInstance;

	/**
	 * @brief Gameplay tags associated with this slot.
	 * 
	 * These tags can be used for filtering, restrictions, or additional metadata.
	 * They are typically set during slot initialization and can be used to
	 * determine what types of items can be placed in this slot.
	 * 
	 * @see FGameplayTagContainer
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	FGameplayTagContainer SlotTags;

	/**
	 * @brief The grid coordinates of this slot.
	 * 
	 * Represents the position of this slot within the inventory grid.
	 * Coordinates are 0-based, with (0,0) typically being the top-left corner.
	 * 
	 * @note Default value is (-1, -1) to indicate uninitialized state.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	FIntPoint Coordinates{-1, -1};

	/**
	 * @brief The root slot coordinates for multi-slot items.
	 * 
	 * For multi-slot items, this points to the root slot that contains the item.
	 * For single-slot items or root slots themselves, this equals Coordinates.
	 * 
	 * @note Default value is (-1, -1) to indicate uninitialized state.
	 * 
	 * @see IsRootSlot
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Inventory Slot|Slot")
	FIntPoint RootSlot{-1, -1};

	/**
	 * @brief The rotation angle of the item in this slot (in degrees).
	 * 
	 * Represents the rotation applied to the item when it was placed.
	 * This is used for visual representation and collision detection.
	 * 
	 * @note Default value is 0.0 degrees (no rotation).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	float Rotation{0.0f};

	/**
	 * @brief Initializes the slot with the specified coordinates and tags.
	 * 
	 * Sets up the slot with its grid position and associated tags. This should
	 * be called when the inventory grid is created.
	 * 
	 * @param SlotPoint The grid coordinates for this slot
	 * @param Tags The gameplay tags to associate with this slot
	 */
	void Initialize(const FIntPoint& SlotPoint, const FGameplayTagContainer& Tags);

	/**
	 * @brief Checks if this slot is currently occupied by an item.
	 * 
	 * @return true if the slot contains an item instance, false otherwise
	 * 
	 * @see ItemInstance
	 */
	bool IsOccupied() const;
	
	/**
	 * @brief Checks if this slot is the root slot for a multi-slot item.
	 * 
	 * @return true if this slot is the root slot, false otherwise
	 * 
	 * @see RootSlot
	 */
	bool IsRootSlot() const { return RootSlot == Coordinates; };
	
	/**
	 * @brief Checks if this slot can accept the specified item.
	 * 
	 * Performs validation to determine if the given item can be placed in this slot,
	 * considering slot tags, item restrictions, and other criteria.
	 * 
	 * @param ItemToTest The item instance to test
	 * @return true if the item can be placed, false otherwise
	 * 
	 * @see SlotTags
	 */
	bool CanAcceptItem(const UDieg_ItemInstance* ItemToTest) const;
	
	/**
	 * @brief Called when an item is placed in this slot.
	 * 
	 * This method is invoked by the inventory system when an item is successfully
	 * placed in this slot. It may trigger item activation events on fragments.
	 * 
	 * @param PlacedItem The item instance that was placed
	 * 
	 * @note This may call PlacedItem->GetFragment<X>()->OnItemActivated() if the slot is active.
	 * 
	 * @see OnItemRemovedFromSlot
	 */
	void OnItemPlacedInSlot(UDieg_ItemInstance* PlacedItem);
	
	/**
	 * @brief Called when an item is removed from this slot.
	 * 
	 * This method is invoked by the inventory system when an item is removed
	 * from this slot. It may trigger item deactivation events on fragments.
	 * 
	 * @note This may call ItemInstance->GetFragment<X>()->OnItemDeactivated() if the slot was active.
	 * 
	 * @see OnItemPlacedInSlot
	 */
	void OnItemRemovedFromSlot();
};
