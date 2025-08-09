// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventorySpatial.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"

void UPlugInv_InventorySpatial::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel_Root);
	Grid_Consumables->SetOwningCanvas(CanvasPanel_Root);
	Grid_Craftables->SetOwningCanvas(CanvasPanel_Root);
	
	ShowEquippables();
}

FReply UPlugInv_InventorySpatial::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//ActiveGrid->DropItem();
	return Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventorySpatial::HasRoomForItem(TObjectPtr<UPlugInv_ItemComponent> ItemComponent) const
{
	EPlugInv_ItemCategory ItemCategory = UPlugInv_InventoryStatics::GetItemCategoryFromItemComponent(ItemComponent);

	switch (ItemCategory)
	{
	case EPlugInv_ItemCategory::None:
		break;
	case EPlugInv_ItemCategory::Equippable:
		{
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		}
		break;
	case EPlugInv_ItemCategory::Consumable:
		{
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		}
		break;
	case EPlugInv_ItemCategory::Craftable:
		{
			return Grid_Craftables->HasRoomForItem(ItemComponent);
		}
		break;
	default:
		{
			UPlugInv_DoubleLogger::LogError("UPlugInv_InventorySpatial::HasRoomForItem : ItemComponent doesn't have a valid item Category");
			checkNoEntry();
			return FPlugInv_SlotAvailabilityResult();
		}
	}

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

void UPlugInv_InventorySpatial::SetActiveGrid(const TObjectPtr<UPlugInv_InventoryGrid>& Grid, const TObjectPtr<UButton>& Button)
{
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
	}
	
	ActiveGrid = Grid;
	
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->ShowCursor();
	}
	
	if (IsValid(Button) == false || IsValid(Grid) == false)
		return;
	
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}
