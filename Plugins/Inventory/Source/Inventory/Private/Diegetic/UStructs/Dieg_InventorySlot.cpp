// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UObjects/Dieg_ItemInstance.h"

void FDieg_InventorySlot::Initialize(const FIntPoint& SlotPoint, const FGameplayTagContainer& Tags)
{
	Coordinates = SlotPoint;
	SlotTags.AppendTags(Tags);
}

bool FDieg_InventorySlot::IsOccupied() const
{
	if (!IsValid(ItemInstance))
	{
		return false;
	}
	return true;
}

bool FDieg_InventorySlot::CanAcceptItem(const UDieg_ItemInstance* ItemToTest) const
{
	return SlotTags.HasAny(ItemToTest->GetTags());
}

void FDieg_InventorySlot::OnItemPlacedInSlot(UDieg_ItemInstance* PlacedItem)
{
	
}

void FDieg_InventorySlot::OnItemRemovedFromSlot()
{
}
