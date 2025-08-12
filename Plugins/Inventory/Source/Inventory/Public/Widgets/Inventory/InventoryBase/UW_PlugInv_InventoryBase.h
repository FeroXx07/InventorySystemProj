// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPF_PlugInv_DataLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlugInv_InventoryBase.generated.h"

class UPlugInv_ItemComponent;
class UPlugInv_InventoryItem;

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// Virtual base function to get slots availability data.
	virtual FPlugInv_SlotAvailabilityResult HasRoomForItem(TObjectPtr<UPlugInv_ItemComponent> ItemComponent) const
	{
		return FPlugInv_SlotAvailabilityResult();
	}
	virtual void OnItemHovered(UPlugInv_InventoryItem* Item) {}
	virtual void OnItemUnHovered() {}
	virtual bool HasHoverItem() const { return false; }
};
