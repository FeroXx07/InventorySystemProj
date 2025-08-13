// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventorySpatial.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/UW_PlugInv_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/UW_PlugInv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/UW_PlugInv_EquippedSlottedItem.h"
#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"
#include "Widgets/ItemDescription/UW_PlugInv_ItemDescription.h"

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

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UPlugInv_EquippedGridSlot* EquippedGridSlot = Cast<UPlugInv_EquippedGridSlot>(Widget); IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});
}

FReply UPlugInv_InventorySpatial::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//ActiveGrid->DropItem();
	return Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
}

void UPlugInv_InventorySpatial::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;
	SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel_Root);
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

void UPlugInv_InventorySpatial::OnItemHovered(UPlugInv_InventoryItem* Item)
{
	const FPlugInv_ItemManifest& Manifest = Item->GetItemManifest();
	//Super::OnItemHovered(Item);
	UPlugInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
	
	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest, DescriptionWidget]()
	{
		Manifest.AssimilateInventoryFragments(DescriptionWidget);
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
	});
	
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UPlugInv_InventorySpatial::OnItemUnHovered()
{
	//Super::OnItemUnHovered();
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UPlugInv_InventorySpatial::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	return false;
}

UPlugInv_HoverItem* UPlugInv_InventorySpatial::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

float UPlugInv_InventorySpatial::GetEquippablesTileSize() const
{
	return Grid_Equippables->GetTileSize();
}

UPlugInv_ItemDescription* UPlugInv_InventorySpatial::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UPlugInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel_Root->AddChild(ItemDescription);
	}
	
	return ItemDescription;
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

void UPlugInv_InventorySpatial::EquippedGridSlotClicked(UPlugInv_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag)
{
	// Check to see if we can equip the Hover Item
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	UPlugInv_HoverItem* HoverItem = GetHoverItem();
	// Create an Equipped Slotted Item and add it to the Equipped Grid Slot (call EquippedGridSlot->OnItemEquipped())
	const float TileSize = UPlugInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetEquippablesTileSize();
	
	UPlugInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag,
		TileSize
	);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	
	// Inform the server that we've equipped an item (potentially unequipping an item as well)
	UPlugInv_InventoryComponent* InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent)); 
	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);
	
	if (GetOwningPlayer()->GetNetMode() == ENetMode::NM_ListenServer || GetOwningPlayer()->GetNetMode() == ENetMode::NM_Standalone)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}

	// Clear the Hover Item
	Grid_Equippables->ClearHoverItem();
}

void UPlugInv_InventorySpatial::EquippedSlottedItemClicked(UPlugInv_EquippedSlottedItem* EquippedSlottedItem)
{
	// Remove the Item Description
	UPlugInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());

	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;

	// Get Item to Equip
	UPlugInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;

	// Get Item to Unequip
	UPlugInv_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();

	// Get the Equipped Grid Slot holding this item
	UPlugInv_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);
	
	// Clear the equipped grid slot of this item (set its inventory item to nullptr
	ClearSlotOfItem(EquippedGridSlot);

	// Assign previously equipped item as the hover item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);

	// Remove of the equipped slotted item from the equipped grid slot
	RemoveEquippedSlottedItem(EquippedSlottedItem);
	
	// Make a new equipped slotted item (for the item we held in HoverItem)
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);
	
	// Broadcast delegates for OnItemEquipped/OnItemUnequipped (from the IC)
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
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
		ActiveGrid->OnHide();
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

void UPlugInv_InventorySpatial::SetItemDescriptionSizeAndPosition(UPlugInv_ItemDescription* Description,
	UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	const FVector2D CanvasSize = UPlugInv_WidgetUtils::GetWidgetSize(Canvas);
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	const FVector2D ClampedPosition = UPlugInv_WidgetUtils::GetClampedWidgetPosition(CanvasSize, ItemDescriptionSize, MousePos);
	
	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

bool UPlugInv_InventorySpatial::CanEquipHoverItem(const UPlugInv_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag) const
{
	bool CanEquipHoverItem = false;
	
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid())
	{
		return CanEquipHoverItem;
	}

	const UPlugInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem))
	{
		return CanEquipHoverItem;
	}

	const UPlugInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();
	if (HasHoverItem() && IsValid(HeldItem)
		&& !HoverItem->IsStackable() && HeldItem->GetItemManifest().GetItemCategory() == EPlugInv_ItemCategory::Equippable &&
		HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag))
	{
		CanEquipHoverItem = true;
	}

	return CanEquipHoverItem;
}

UPlugInv_EquippedGridSlot* UPlugInv_InventorySpatial::FindSlotWithEquippedItem(
	UPlugInv_InventoryItem* EquippedItem) const
{
	const TObjectPtr<UPlugInv_EquippedGridSlot>* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate(
		[EquippedItem](const UPlugInv_EquippedGridSlot* GridSlot)
		{
			return GridSlot->GetInventoryItem() == EquippedItem;
		});

	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}

void UPlugInv_InventorySpatial::ClearSlotOfItem(UPlugInv_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

void UPlugInv_InventorySpatial::RemoveEquippedSlottedItem(UPlugInv_EquippedSlottedItem* EquippedSlottedItem)
{
	// (unbind from the OnEquippedSlottedItemClicked )
	// Removing the Equipped Slotted Item from Parent
	if (!IsValid(EquippedSlottedItem)) return;
	
	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	EquippedSlottedItem->RemoveFromParent();
}

void UPlugInv_InventorySpatial::MakeEquippedSlottedItem(const UPlugInv_EquippedSlottedItem* EquippedSlottedItem,
	UPlugInv_EquippedGridSlot* EquippedGridSlot, UPlugInv_InventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	UPlugInv_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(
		ItemToEquip,
		EquippedSlottedItem->GetEquipmentTypeTag(),
		UPlugInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetEquippablesTileSize());
	
	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UPlugInv_InventorySpatial::BroadcastSlotClickedDelegates(UPlugInv_InventoryItem* ItemToEquip,
	UPlugInv_InventoryItem* ItemToUnequip) const
{
	UPlugInv_InventoryComponent* InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if (GetOwningPlayer()->GetNetMode() == ENetMode::NM_ListenServer || GetOwningPlayer()->GetNetMode() == ENetMode::NM_Standalone)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemUnequipped.Broadcast(ItemToUnequip);
	}
}
