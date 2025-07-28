// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"

#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"

UPlugInv_InventoryComponent* UPlugInv_InventoryStatics::GetInventoryComponent(
	const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UPlugInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UPlugInv_InventoryComponent>();
	
	if (IsValid(InventoryComponent))
		return InventoryComponent;
	else
		return nullptr;
}
