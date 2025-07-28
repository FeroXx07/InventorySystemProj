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

private:

	// Item category.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EPlugInv_ItemCategory ItemCategory{EPlugInv_ItemCategory::None};

	// Item tags.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTagContainer ItemTypesTags;
};

/*
*  Introduced in Unreal Engine 5.0 is the StructUtils plugin that brings with it the concept of Instanced Structs.
*  They do exactly what their name suggests; provides the same features as instanced objects but embodied as USTRUCTs.
*  This comes with several advantages, the most prominent of which is their light weight nature as compared to their counter parts in instanced UObject*s.
*  Instanced Structs are also fully polymorphic and serialized across the network.
*  Meaning they support replicated, effectively arbitrary data. It is up to the receiver to interpret that data appropriately.
*  https://www.reddit.com/r/unrealengine/comments/1f7o1co/what_is_the_difference_between_a_struct_and_an/
 */