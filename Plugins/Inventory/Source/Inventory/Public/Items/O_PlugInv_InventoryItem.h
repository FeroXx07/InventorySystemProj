// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/F_PlugInv_ItemManifest.h"

#include "O_PlugInv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// So that it can be a registered sub object lists.
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	// Item Manifest Setter.
	void SetItemManifest(const FPlugInv_ItemManifest& Manifest);

	// Item Manifest Getter const.
	const FPlugInv_ItemManifest& GetItemManifest() const
	{
		return ItemManifest.Get<FPlugInv_ItemManifest>();
	}

	// Item Manifest Getter mutable.
	FPlugInv_ItemManifest& GetItemManifest()
	{
		return ItemManifest.GetMutable<FPlugInv_ItemManifest>();
	}
private:

	// Constraint its inheritance capabilities to only FPlugInv_ItemManifest.
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/Inventory.FPlugInv_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
};
