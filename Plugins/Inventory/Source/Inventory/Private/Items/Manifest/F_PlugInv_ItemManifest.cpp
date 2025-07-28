// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/F_PlugInv_ItemManifest.h"

#include "Items/O_PlugInv_InventoryItem.h"

UPlugInv_InventoryItem* FPlugInv_ItemManifest::Manifest(UObject* Outer)
{
	UPlugInv_InventoryItem* Item = NewObject<UPlugInv_InventoryItem>(Outer, UPlugInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);
	return Item;
}
