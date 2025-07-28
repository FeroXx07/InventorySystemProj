// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/UW_PlugInv_HUDWidget.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Widgets/HUD/UW_PlugInv_InfoMessage.h"

void UPlugInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	TObjectPtr<UPlugInv_InventoryComponent> InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnNoRoomInInventory.AddDynamic(this, &UPlugInv_HUDWidget::OnNoRoom);
	}
}

void UPlugInv_HUDWidget::OnNoRoom()
{
	if (!IsValid(InfoMessage))
		return;

	UPlugInv_DoubleLogger::Log("On No Room for UPlugInv_HUDWidget");
	
	InfoMessage->SetMessage(FString("No room in inventory."));
}
