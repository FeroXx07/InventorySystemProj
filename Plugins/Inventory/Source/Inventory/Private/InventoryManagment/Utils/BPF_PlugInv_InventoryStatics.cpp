// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"

#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"

UPlugInv_InventoryComponent* UPlugInv_InventoryStatics::GetInventoryComponent(
	const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	
	UPlugInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UPlugInv_InventoryComponent>();
	
	if (IsValid(InventoryComponent))
	{
		return InventoryComponent;
	}
	else
	{
		return nullptr;
	}
}

EPlugInv_ItemCategory UPlugInv_InventoryStatics::GetItemCategoryFromItemComponent(
	const UPlugInv_ItemComponent* ItemComponent)
{
	if (!IsValid(ItemComponent))
	{
		return EPlugInv_ItemCategory::None;
	}

	return ItemComponent->GetItemManifest().GetItemCategory();
}
