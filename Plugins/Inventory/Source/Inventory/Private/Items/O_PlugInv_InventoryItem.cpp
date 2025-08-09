// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/O_PlugInv_InventoryItem.h"

#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Items/Fragments/PlugInv_FragmentTags.h"
#include "Net/UnrealNetwork.h"

void UPlugInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}

void UPlugInv_InventoryItem::SetItemManifest(const FPlugInv_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FPlugInv_ItemManifest>(Manifest);
}

bool UPlugInv_InventoryItem::IsStackable() const
{
	const FPlugInv_StackableFragment* Stackable = GetFragment<FPlugInv_StackableFragment>(this, FragmentTags::StackableFragment);
	return Stackable != nullptr;
}

bool UPlugInv_InventoryItem::IsConsumable() const
{
	return GetItemManifest().GetItemCategory() == EPlugInv_ItemCategory::Consumable;
}
