// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItems/UW_PlugInv_SlottedItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/O_PlugInv_InventoryItem.h"

FReply UPlugInv_SlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex, InMouseEvent);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPlugInv_SlottedItem::SetInventoryItem(UPlugInv_InventoryItem* Item)
{
	InventoryItem = Item; 
}

void UPlugInv_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UPlugInv_SlottedItem::UpdateStackAmount(const int32 NewStackCount) const
{
	if (NewStackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(NewStackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
