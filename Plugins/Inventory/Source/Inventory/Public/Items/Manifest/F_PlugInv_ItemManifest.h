// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPF_PlugInv_DataLibrary.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "F_PlugInv_ItemManifest.generated.h"

/**
 * The Item Manifest contains all the necessary data for creating a new Inventory Item.
 */

struct FPlugInv_ItemFragment;

USTRUCT(BlueprintType)
struct INVENTORY_API FPlugInv_ItemManifest
{
	GENERATED_BODY()

	// Instance inventory item and instanced struct through manifest. 
	UPlugInv_InventoryItem* Manifest(UObject* Outer);

	// Item category getter.
	EPlugInv_ItemCategory GetItemCategory() const
	{
		return ItemCategory;
	}

	// Item Type getter.
	FGameplayTag GetItemType() const
	{
		return ItemTypesTags.First();
	}

	template<typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
	const T* GetFragmentOfTypeByTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
	T* GetFragmentOfTypeByTagMutable(const FGameplayTag& FragmentTag);
	
	template<typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

	void SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);
private:

	// Item category.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EPlugInv_ItemCategory ItemCategory{EPlugInv_ItemCategory::None};

	// Item tags.
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="GameItems"))
	FGameplayTagContainer ItemTypesTags;

	// TInstanced struct is a type safe wrapper of FInstancedStruct. This way ensures that it can be only of type FPlugInv_ItemFragment and children.
	// With ExcludeBaseStruct we also ensure that it can't be only FPlugInv_ItemFragment but child structs.
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FPlugInv_ItemFragment>> Fragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> PickupActorClass;
};

/*
*  Introduced in Unreal Engine 5.0 is the StructUtils plugin that brings with it the concept of Instanced Structs.
*  They do exactly what their name suggests; provides the same features as instanced objects but embodied as USTRUCTTs.
*  This comes with several advantages, the most prominent of which is their lightweight nature as compared to their counter-parts in instanced UObject*s.
*  Instanced Structs are also fully polymorphic and serialized across the network.
*  Meaning they support replicated, effectively arbitrary data. It is up to the receiver to interpret that data appropriately.
*  https://www.reddit.com/r/unrealengine/comments/1f7o1co/what_is_the_difference_between_a_struct_and_an/
 */

template <typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
const T* FPlugInv_ItemManifest::GetFragmentOfTypeByTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag))
				continue;
			
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
const T* FPlugInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
T* FPlugInv_ItemManifest::GetFragmentOfTypeByTagMutable(const FGameplayTag& FragmentTag)
{
	for (TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag))
				continue;
			
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FPlugInv_ItemFragment>
T* FPlugInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FPlugInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}
