// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventorySpatial.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"

void UPlugInv_InventorySpatial::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	ShowEquippables();
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventorySpatial::HasRoomForItem(TObjectPtr<UPlugInv_ItemComponent> ItemComponent) const
{
	FPlugInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;
	return Result;
}

void UPlugInv_InventorySpatial::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UPlugInv_InventorySpatial::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UPlugInv_InventorySpatial::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UPlugInv_InventorySpatial::DisableButton(const TObjectPtr<UButton>& Button) const
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UPlugInv_InventorySpatial::SetActiveGrid(const TObjectPtr<UPlugInv_InventoryGrid>& Grid, const TObjectPtr<UButton>& Button) const
{
	if (IsValid(Button) == false || IsValid(Grid) == false)
		return;
	
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}
