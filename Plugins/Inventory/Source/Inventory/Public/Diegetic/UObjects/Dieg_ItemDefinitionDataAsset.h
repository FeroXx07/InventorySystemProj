// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/UStructs/Dieg_ItemDefinition.h"
#include "Engine/DataAsset.h"
#include "Dieg_ItemDefinitionDataAsset.generated.h"

class UDieg_ItemFragment;

/**
 * @brief Data asset for item definitions in the diegetic inventory system.
 * 
 * UDieg_ItemDefinitionDataAsset is a primary data asset that contains the
 * static definition data for items in the diegetic inventory system. It serves
 * as the data container for item definitions that can be created and configured
 * in the editor.
 * 
 * This data asset provides:
 * - Item definition data storage
 * - Editor integration for item configuration
 * - Automatic shape root calculation
 * - Data validation and cleanup
 * 
 * @note This is a primary data asset and can be referenced by other systems.
 * @note This data asset contains FDieg_ItemDefinition data.
 * 
 * @see FDieg_ItemDefinition
 * @see UDieg_ItemInstance
 * @see UPrimaryDataAsset
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_ItemDefinitionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief The item definition data for this asset.
	 * 
	 * Contains all the static properties and behavior data for the item,
	 * including visual assets, inventory shape, stacking rules, and fragments.
	 * 
	 * @see FDieg_ItemDefinition
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Item Definition Data Asset|Definition", meta = (AllowPrivateAccess = "true"))
	FDieg_ItemDefinition ItemDefinition;

	/**
	 * @brief Called when a property is changed in the editor.
	 * 
	 * Handles property changes and updates related data as needed.
	 * 
	 * @param PropertyChangedEvent Information about the property change
	 */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	/**
	 * @brief Called when the asset is saved.
	 * 
	 * Performs any necessary cleanup or validation when the asset is saved.
	 * 
	 * @param ObjectSaveContext Context information about the save operation
	 */
	virtual void PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext) override;

	/**
	 * @brief Sets the shape root for an item definition.
	 * 
	 * Calculates and sets the root position of the item's shape based on
	 * its shape data. This is used to ensure proper positioning of items
	 * in the inventory grid.
	 * 
	 * @param ItemDef The item definition to update
	 * 
	 * @note This is a static function that can be called from Blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Definition Data Asset")
	static void SetItemDefinitionShapeRoot(FDieg_ItemDefinition& ItemDef);
};
