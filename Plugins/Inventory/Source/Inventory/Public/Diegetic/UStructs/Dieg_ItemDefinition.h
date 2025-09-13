// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemDefinition.generated.h"

class UDieg_ItemFragment;

/**
 * @brief Defines the static properties and behavior of an item type in the diegetic inventory system.
 * 
 * FDieg_ItemDefinition serves as the template for creating item instances. It contains all the
 * static data that defines how an item behaves, looks, and functions within the inventory system.
 * This includes visual assets, inventory shape, stacking rules, and functional fragments.
 * 
 * Item definitions are typically created as data assets and referenced by item instances.
 * They provide the blueprint for creating new instances of items in the game.
 * 
 * @note This struct is Blueprint-compatible and can be used in Blueprint graphs.
 * 
 * @see UDieg_ItemInstance
 * @see UDieg_ItemFragment
 * @see UDieg_ItemDefinitionDataAsset
 * 
 * @since 1.0
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_ItemDefinition
{
	GENERATED_BODY()

	/**
	 * @brief The display name of the item.
	 * 
	 * This is the human-readable name that appears in the UI and tooltips.
	 * Should be localized for multi-language support.
	 * 
	 * @see Description
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Display") 
	FText Name;

	/**
	 * @brief The description of the item.
	 * 
	 * Detailed description that appears in tooltips and item details.
	 * Should provide information about the item's purpose and properties.
	 * 
	 * @see Name
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Display") 
	FText Description;

	/**
	 * @brief The gameplay tag that categorizes this item type.
	 * 
	 * Used for filtering, searching, and categorizing items. This tag can be
	 * used to group similar items together or apply restrictions.
	 * 
	 * @see FGameplayTag
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Categorization") 
	FGameplayTag ItemType;

	/**
	 * @brief The 3D mesh used to represent this item in the world.
	 * 
	 * This mesh is displayed when the item is placed in the 3D world or
	 * when previewing the item in the inventory.
	 * 
	 * @note Uses TSoftObjectPtr for lazy loading and memory efficiency.
	 * 
	 * @see Icon2d
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Visuals") 
	TSoftObjectPtr<UStaticMesh> WorldMesh;

	/**
	 * @brief The 2D icon used to represent this item in the UI.
	 * 
	 * This texture is displayed in inventory grids, tooltips, and other UI elements.
	 * Should be optimized for UI display (typically 64x64 or 128x128 pixels).
	 * 
	 * @note Uses TSoftObjectPtr for lazy loading and memory efficiency.
	 * 
	 * @see WorldMesh
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Visuals") 
	TSoftObjectPtr<UTexture2D> Icon2d;

	/**
	 * @brief The default shape of this item in the inventory grid.
	 * 
	 * Defines which grid positions this item occupies when placed in the inventory.
	 * Coordinates are relative to the DefaultShapeRoot point.
	 * 
	 * @note This is used for collision detection and visual representation.
	 * 
	 * @see DefaultShapeRoot
	 * @see UDieg_InventoryComponent::GetRelevantCoordinates
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Item Definition|Diegetic Inventory")
	TArray<FIntPoint> DefaultShape;

	/**
	 * @brief The root point of the item's shape in the inventory grid.
	 * 
	 * Defines the "anchor" point of the item's shape. This is typically (0,0)
	 * and represents the primary position of the item within its shape.
	 * 
	 * @note This is used for rotation calculations and placement logic.
	 * 
	 * @see DefaultShape
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Item Definition|Diegetic Inventory")
	FIntPoint DefaultShapeRoot;

	/**
	 * @brief The maximum stack size for this item type.
	 * 
	 * Defines how many units of this item can be stacked together in a single slot.
	 * A value of 1 means the item cannot be stacked.
	 * 
	 * @note Minimum value is 1 (no stacking).
	 * 
	 * @see UDieg_ItemInstance::Quantity
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Definition|Stacking", meta=(ClampMin="1")) 
	int32 StackSizeMax = 1;

	/**
	 * @brief The functional fragments that define this item's behavior.
	 * 
	 * Array of UDieg_ItemFragment instances that provide the item's functionality.
	 * Each fragment can implement different aspects of the item's behavior,
	 * such as consumable effects, equipment bonuses, or special abilities.
	 * 
	 * @note These fragments are instantiated when creating item instances.
	 * 
	 * @see UDieg_ItemFragment
	 * @see UDieg_ItemInstance::ItemFragments
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category="Game|Dieg|Item Definition|Functionality")
	TArray<TObjectPtr<UDieg_ItemFragment>> Fragments;
};

