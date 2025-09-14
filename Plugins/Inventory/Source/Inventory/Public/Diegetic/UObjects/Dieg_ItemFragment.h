// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_ItemFragment.generated.h"

class UDieg_ItemInstance;

/**
 * @brief Base class for item functionality fragments in the diegetic inventory system.
 * 
 * UDieg_ItemFragment is an abstract base class that provides a modular way to add
 * functionality to items. Each fragment represents a specific aspect of an item's
 * behavior, such as consumable effects, equipment bonuses, or special abilities.
 * 
 * The fragment system allows for flexible item design by combining different fragments
 * to create complex item behaviors. Fragments are instantiated when item instances
 * are created and can respond to various lifecycle events.
 * 
 * @note This class is abstract and should be inherited to create specific fragment types.
 * @note This class is Blueprint-compatible and can be implemented in Blueprint.
 * 
 * @see UDieg_ItemInstance
 * @see FDieg_ItemDefinition
 * 
 * @since 1.0
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class INVENTORY_API UDieg_ItemFragment : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief The gameplay tag that identifies this fragment type.
	 * 
	 * Used to identify and categorize fragments. This tag can be used to
	 * search for specific fragment types or apply fragment-specific behavior.
	 * 
	 * @see FGameplayTag
	 * @see GetFragmentTag
	 * @see SetFragmentTag
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Game|Dieg|Item Fragment|Item", meta = (AllowPrivateAccess = "true")) 
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;

	/**
	 * @brief Called when the fragment is instantiated for an item instance.
	 * 
	 * This method is invoked when the fragment is created and attached to an
	 * item instance. Override this to perform any initialization specific to
	 * the fragment type.
	 * 
	 * @param OwningInstance The item instance that owns this fragment
	 * 
	 * @note The default implementation does nothing.
	 * 
	 * @see OnItemActivated
	 * @see OnItemDeactivated
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnInstanced(UDieg_ItemInstance* OwningInstance) {}
	
	/**
	 * @brief Called when the item is activated or equipped.
	 * 
	 * This method is invoked when the item becomes active, such as when it's
	 * equipped by a player or placed in an active slot. Override this to
	 * implement activation behavior.
	 * 
	 * @param OwningInstance The item instance that owns this fragment
	 * 
	 * @note The default implementation does nothing.
	 * 
	 * @see OnItemDeactivated
	 * @see OnInstanced
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnItemActivated(UDieg_ItemInstance* OwningInstance) {}
	
	/**
	 * @brief Called when the item is deactivated or unequipped.
	 * 
	 * This method is invoked when the item becomes inactive, such as when it's
	 * unequipped by a player or removed from an active slot. Override this to
	 * implement deactivation behavior.
	 * 
	 * @param OwningInstance The item instance that owns this fragment
	 * 
	 * @note The default implementation does nothing.
	 * 
	 * @see OnItemActivated
	 * @see OnInstanced
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual void OnItemDeactivated(UDieg_ItemInstance* OwningInstance) {}

	/**
	 * @brief Gets the gameplay tag for this fragment.
	 * 
	 * @return The fragment's gameplay tag
	 * 
	 * @see FragmentTag
	 * @see SetFragmentTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	const FGameplayTag& GetFragmentTag() const;
	
	/**
	 * @brief Sets the gameplay tag for this fragment.
	 * 
	 * @param Tag The new gameplay tag to set
	 * 
	 * @see FragmentTag
	 * @see GetFragmentTag
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	void SetFragmentTag(const FGameplayTag& Tag);

	/**
	 * @brief Virtual method for comparing fragments for equality.
	 * 
	 * This method is used to determine if two fragments are equivalent.
	 * The base implementation compares fragment tags, but derived classes
	 * should override this to include fragment-specific comparison logic.
	 * 
	 * @param Other The fragment to compare against
	 * @return true if the fragments are equal, false otherwise
	 * 
	 * @note The default implementation only compares fragment tags.
	 * 
	 * @see UDieg_ItemInstance::IsEqual
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Item Fragment")
	virtual bool IsEqual(const UDieg_ItemFragment* Other) const
	{
		if (!Other) return false;
		return FragmentTag == Other->FragmentTag;
	}
};
