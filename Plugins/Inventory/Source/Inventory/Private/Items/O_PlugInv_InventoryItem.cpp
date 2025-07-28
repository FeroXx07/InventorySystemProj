// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/O_PlugInv_InventoryItem.h"

#include "Net/UnrealNetwork.h"

void UPlugInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UPlugInv_InventoryItem::SetItemManifest(const FPlugInv_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FPlugInv_ItemManifest>(Manifest);
}
