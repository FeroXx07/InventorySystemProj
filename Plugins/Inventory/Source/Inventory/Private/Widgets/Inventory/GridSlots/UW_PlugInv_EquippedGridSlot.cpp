// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/UW_PlugInv_EquippedGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Items/Fragments/PlugInv_FragmentTags.h"
#include "Widgets/Inventory/HoverItem/UW_PlugInv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/UW_PlugInv_EquippedSlottedItem.h"

void UPlugInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (!IsAvailable()) return;
	const UPlugInv_HoverItem* HoverItem = UPlugInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetStateAndBrushTexture(EPlugInv_GridSlotState::Occupied);
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlugInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (!IsAvailable()) return;
	const UPlugInv_HoverItem* HoverItem = UPlugInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetStateAndBrushTexture(EPlugInv_GridSlotState::Unoccupied);
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply UPlugInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

UPlugInv_EquippedSlottedItem* UPlugInv_EquippedGridSlot::OnItemEquipped(UPlugInv_InventoryItem* Item,
	const FGameplayTag& EquipmentTag, float TileSize)
{
	// Check the Equipment Type Tag
	if (!EquipmentTag.MatchesTagExact(EquipmentTypeTag)) return nullptr;

	// Get Grid Dimensions
	const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridDimensions = GridFragment->GetGridSize();

	// Calculate the Draw Size for the Equipped Slotted Item
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;

	// Create the Equipped Slotted Item widget
	EquippedSlottedItem = CreateWidget<UPlugInv_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);

	// Set the Slotted Item's Inventory Item
	EquippedSlottedItem->SetInventoryItem(Item);

	// Set the Slotted Item's Equipment Type Tag
	EquippedSlottedItem->SetEquipmentTypeTag(EquipmentTag);

	// Hide the Stack Count widget on the Slotted Item
	EquippedSlottedItem->UpdateStackAmount(0);

	// Set Inventory Item on this class (the Equipped Grid Slot)
	SetInventoryItem(Item);

	// Set the Image Brush on the Equipped Slotted Item
	const FPlugInv_ImageFragment* ImageFragment = GetFragment<FPlugInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!ImageFragment) return nullptr;
	
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;
	EquippedSlottedItem->SetImageBrush(Brush);

	// Add the Slotted Item as a child to this widget's Overlay
	Overlay_Root->AddChildToOverlay(EquippedSlottedItem);
	const FGeometry OverlayGeometry = Overlay_Root->GetCachedGeometry();
	FVector2f OverlayPos = OverlayGeometry.Position;
	const FDeprecateSlateVector2D OverlaySize = OverlayGeometry.Size;
	
	const float LeftPadding = OverlaySize.X / 2.f - DrawSize.X / 2.f;
	const float TopPadding = OverlaySize.Y / 2.f - DrawSize.Y / 2.f;
	
	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));

	// Return the Equipped Slotted Item widget
	return EquippedSlottedItem;
}
