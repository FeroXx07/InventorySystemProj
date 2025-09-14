// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_ItemDefinitionDataAsset.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemInstance.generated.h"

class UDieg_ItemFragment;
class UDieg_ItemDefinitionDataAsset;

/**
 * @brief Represents a specific instance of an item in the diegetic inventory system.
 * 
 * UDieg_ItemInstance is the runtime representation of an item that exists in the game world.
 * It contains the actual data for a specific item, including its quantity, instance-specific
 * tags, and instantiated fragments that provide the item's functionality.
 * 
 * Each item instance is created from an item definition and can be placed in inventory slots,
 * stacked with other instances of the same type, and manipulated by the inventory system.
 * 
 * @note This class is marked with EditInlineNew and DefaultToInstanced to allow it to exist
 * as an instanced subobject, similar to how components work in Unreal Engine.
 * 
 * @see FDieg_ItemDefinition
 * @see UDieg_ItemFragment
 * @see UDieg_ItemDefinitionDataAsset
 * @see FDieg_InventorySlot
 * 
 * @since 1.0
 */
UCLASS(EditInlineNew, DefaultToInstanced)
class INVENTORY_API UDieg_ItemInstance : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief The data asset containing the item definition for this instance.
	 * 
	 * References the UDieg_ItemDefinitionDataAsset that contains the static
	 * definition data for this item type. This is used to access the base
	 * properties like name, description, and visual assets.
	 * 
	 * @see UDieg_ItemDefinitionDataAsset
	 * @see GetItemDefinition
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Item Instance|Pre Populate Item", meta = (AllowPrivateAccess = "true")) 
	TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDefinitionDataAsset;

	/**
	 * @brief The instantiated fragments that provide this item's functionality.
	 * 
	 * Array of UDieg_ItemFragment instances that define the specific behavior
	 * of this item. These are instantiated from the fragments defined in the
	 * item definition during initialization.
	 * 
	 * @see UDieg_ItemFragment
	 * @see GetFragment
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta = (AllowPrivateAccess = "true")) 
	TArray<TObjectPtr<UDieg_ItemFragment>> ItemFragments;

	/**
	 * @brief The quantity of this item instance.
	 * 
	 * Represents how many units of this item are contained in this instance.
	 * For stackable items, this can be greater than 1. For non-stackable items,
	 * this should always be 1.
	 * 
	 * @note Minimum value is 1.
	 * 
	 * @see FDieg_ItemDefinition::StackSizeMax
	 * @see CanStackWith
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta=(ClampMin="1", AllowPrivateAccess = "true")) 
	int32 Quantity = 1;

	/**
	 * @brief Instance-specific gameplay tags for this item.
	 * 
	 * These tags are specific to this item instance and can be used for
	 * filtering, searching, or applying instance-specific behavior.
	 * They are separate from the tags defined in the item definition.
	 * 
	 * @see FGameplayTagContainer
	 * @see FDieg_ItemDefinition::ItemType
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item" , meta = (AllowPrivateAccess = "true")) 
	FGameplayTagContainer ItemTags;

	/**
	 * @brief Flag indicating whether this item instance has been initialized.
	 * 
	 * Set to true after successful initialization. Used to prevent operations
	 * on uninitialized item instances.
	 * 
	 * @see Initialize
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Instance|Item", meta = (AllowPrivateAccess = "true")) 
	bool bIsInitialized{false};

	/**
	 * @brief Initializes the item instance with the specified definition and quantity.
	 * 
	 * This method must be called before the item instance can be used. It creates
	 * instances of all fragments from the item definition and sets up the item
	 * with the specified quantity.
	 * 
	 * @param Def The item definition data asset to use
	 * @param InQuantity The initial quantity for this item instance
	 * 
	 * @see GetItemDefinition
	 * @see ItemFragments
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	void Initialize(UDieg_ItemDefinitionDataAsset* Def, int32 InQuantity = 1);

	/**
	 * @brief Gets the item definition data asset for this instance.
	 * 
	 * @return The data asset containing the item definition, or nullptr if not set
	 * 
	 * @see ItemDefinitionDataAsset
	 * @see GetItemDefinition
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	UDieg_ItemDefinitionDataAsset* GetItemDefinitionDataAsset() const;

	/**
	 * @brief Gets the item definition struct for this instance.
	 * 
	 * @return The item definition struct containing static item data
	 * 
	 * @see FDieg_ItemDefinition
	 * @see GetItemDefinitionDataAsset
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	const FDieg_ItemDefinition& GetItemDefinition() const;
	
	/**
	 * @brief Gets the current quantity of this item instance.
	 * 
	 * @return The current quantity
	 * 
	 * @see Quantity
	 * @see SetQuantity
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	int32 GetQuantity() const { return Quantity; };
	
	/**
	 * @brief Sets the quantity of this item instance.
	 * 
	 * @param InQuantity The new quantity (must be >= 1)
	 * 
	 * @see Quantity
	 * @see GetQuantity
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	void SetQuantity(int32 InQuantity) {Quantity = InQuantity;};
	
	/**
	 * @brief Gets a specific fragment instance by type.
	 * 
	 * Template method that searches for a fragment of the specified type.
	 * This is useful for accessing specific functionality provided by fragments.
	 * 
	 * @tparam T The type of fragment to retrieve
	 * @return Pointer to the fragment of type T, or nullptr if not found
	 * 
	 * @see ItemFragments
	 * @see UDieg_ItemFragment
	 */
	template <typename T> T* GetFragment() const;
	
	/**
	 * @brief Gets the gameplay tags for this item instance.
	 * 
	 * @return The container of gameplay tags
	 * 
	 * @see ItemTags
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	const FGameplayTagContainer& GetTags() const { return ItemTags; }
	
	/**
	 * @brief Checks if this item instance is equal to another instance.
	 * 
	 * Performs deep comparison of the item instances, including fragment states.
	 * This is used to determine if two instances represent the same item.
	 * 
	 * @param ToCheck The item instance to compare against
	 * @return true if the instances are equal, false otherwise
	 * 
	 * @see CanStackWith
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	bool IsEqual(const UDieg_ItemInstance* ToCheck) const;
	
	/**
	 * @brief Checks if this item instance can be stacked with another instance.
	 * 
	 * Determines if two item instances can be combined into a single stack.
	 * This considers the item definition, fragment states, and other factors.
	 * 
	 * @param ToCheck The item instance to check stacking compatibility with
	 * @return true if the instances can be stacked, false otherwise
	 * 
	 * @see IsEqual
	 * @see FDieg_ItemDefinition::StackSizeMax
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Instance")
	bool CanStackWith(const UDieg_ItemInstance* ToCheck) const;
};

template <typename T>
T* UDieg_ItemInstance::GetFragment() const
{
	for (UDieg_ItemFragment* Fragment : ItemFragments)
	{
		if (T* CastedFragment = Cast<T>(Fragment))
		{
			return CastedFragment;
		}
	}
	return nullptr;
}
