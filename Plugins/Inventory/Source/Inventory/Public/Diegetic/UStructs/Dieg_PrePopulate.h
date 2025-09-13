// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dieg_PrePopulate.generated.h"

class UDieg_ItemDefinitionDataAsset;

/**
 * @brief Structure for prepopulating inventory items.
 * 
 * FDieg_PrePopulate defines the data needed to prepopulate an inventory
 * with specific items. It contains the item definition data asset and
 * the quantity to spawn.
 * 
 * This struct is used when setting up inventories with initial items,
 * such as when creating a chest with starting loot or a player inventory
 * with starting equipment.
 * 
 * @note This struct is Blueprint-compatible and can be used in Blueprint graphs.
 * @note The replication properties are commented out but can be enabled for networking.
 * 
 * @see UDieg_ItemDefinitionDataAsset
 * @see UDieg_InventoryComponent
 * 
 * @since 1.0
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_PrePopulate
{
	GENERATED_BODY()

	/**
	 * @brief The item definition data asset for the item to prepopulate.
	 * 
	 * References the data asset that contains the static definition
	 * data for the item to be spawned in the inventory.
	 * 
	 * @see UDieg_ItemDefinitionDataAsset
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Pre Populate|Pre Populate Item" /*, Replicated */) 
	TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDefinitionDataAsset;

	/**
	 * @brief The quantity of the item to prepopulate.
	 * 
	 * Specifies how many instances of the item should be created
	 * when prepopulating the inventory. Defaults to 1.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Pre Populate|Pre Populate Item" /*, Replicated */) 
	int32 Quantity{1};
};
