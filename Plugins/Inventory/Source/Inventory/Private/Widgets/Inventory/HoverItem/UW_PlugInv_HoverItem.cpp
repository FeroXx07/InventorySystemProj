// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/HoverItem/UW_PlugInv_HoverItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/O_PlugInv_InventoryItem.h"

void UPlugInv_HoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UPlugInv_HoverItem::UpdateStackCount(const int32 NewStackCount)
{
	StackCount = NewStackCount;
	if (StackCount > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(StackCount));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UPlugInv_HoverItem::GetItemType() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem.Get()->GetItemManifest().GetItemType();
	}
	return FGameplayTag::EmptyTag;
}

void UPlugInv_HoverItem::SetIsStackable(bool Stackable)
{
	bIsStackable = Stackable;
	if (!bIsStackable)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UPlugInv_InventoryItem* UPlugInv_HoverItem::GetInventoryItem() const
{
	return InventoryItem.Get();
}

void UPlugInv_HoverItem::SetInventoryItem(UPlugInv_InventoryItem* Value)
{
	InventoryItem = Value;
}
