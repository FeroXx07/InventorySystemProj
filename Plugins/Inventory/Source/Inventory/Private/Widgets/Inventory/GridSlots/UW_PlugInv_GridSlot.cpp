// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/UW_PlugInv_GridSlot.h"

#include "Components/Image.h"
#include "Items/O_PlugInv_InventoryItem.h"

void UPlugInv_GridSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlugInv_GridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnGridSlotHovered.Broadcast(TileIndex, InMouseEvent);
}

void UPlugInv_GridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnGridSlotUnhovered.Broadcast(TileIndex, InMouseEvent);
}

FReply UPlugInv_GridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnGridSlotClicked.Broadcast(TileIndex, InMouseEvent);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPlugInv_GridSlot::SetStateAndBrushTexture(const EPlugInv_GridSlotState SlotState)
{
	GridSlotState = SlotState;
	Image_GridSlot->SetBrush(Brushes[GridSlotState]);
}

int32 UPlugInv_GridSlot::GetStackCount() const
{
	return StackCount;
}

void UPlugInv_GridSlot::SetStackCount(const int32 Value)
{
	this->StackCount = Value;
}

int32 UPlugInv_GridSlot::GetUpperLeftIndex() const
{
	return UpperLeftIndex;
}

void UPlugInv_GridSlot::SetUpperLeftIndex(const int32 Value)
{
	this->UpperLeftIndex = Value;
}

TWeakObjectPtr<UPlugInv_InventoryItem> UPlugInv_GridSlot::GetInventoryItem() const
{
	return InventoryItem;
}

void UPlugInv_GridSlot::SetInventoryItem(UPlugInv_InventoryItem* Value)
{
	InventoryItem = Value;
}

bool UPlugInv_GridSlot::IsAvailable() const
{
	return bAvailable;
}

void UPlugInv_GridSlot::SetAvailable(const bool bValue)
{
	this->bAvailable = bValue;
}
