// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Items/Fragments/PlugInv_FragmentTags.h"
#include "Widgets/Inventory/SlottedItems/UW_PlugInv_SlottedItem.h"
#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"

void UPlugInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

void UPlugInv_InventoryGrid::ConstructGrid()
{
	UPlugInv_DoubleLogger::Log("InventoryGrid::ConstructGrid()");
	
	GridSlots.Reserve(Rows*Columns);

	for (int j = 0; j < Rows; ++j)
	{
		for (int i = 0; i < Columns; ++i)
		{
			// Unique temp name
			FString StrName = TEXT("Row ") + FString::FromInt(j) + TEXT(" Col ") + FString::FromInt(i);

			// Creating and adding grid slot to the canvas
			UPlugInv_GridSlot* GridSlot = CreateWidget<UPlugInv_GridSlot>(this, GridSlotClass, FName(*StrName));
			CanvasPanel->AddChild(GridSlot);
			
			const FIntPoint TilePos = FIntPoint(i, j);
			
			// Setting index
			GridSlot->SetTileIndex(UPlugInv_WidgetUtils::GetIndexFromPosition(TilePos, Columns));

			// Grid slot widget pos and size
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePos * TileSize);

			// Adding grid slot to the container array
			GridSlots.Add(GridSlot);
		}
	}
}



void UPlugInv_InventoryGrid::AddItem(UPlugInv_InventoryItem* Item)
{
	// Early return if category doesn't match
	if (!MatchesCategory(Item))
	{
		return;
	}

	UPlugInv_DoubleLogger::Log("InventoryGrid::AddItem()");

	// Slot availability result data + Check room
	FPlugInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	// Create a widget to show the item icon and add it to the correct spot on the grid.
	AddItemToIndices(Result, Item);
}

void UPlugInv_InventoryGrid::AddItemToIndices(const FPlugInv_SlotAvailabilityResult& Result, UPlugInv_InventoryItem* NewItem)
{
	UPlugInv_DoubleLogger::Log("InventoryGrid::AddItemToIndices()");

	// Check multiple slot availabilities
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemToIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

void UPlugInv_InventoryGrid::AddItemToIndex(UPlugInv_InventoryItem* NewItem, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	LOG_DOUBLE_S("Adding Item with index: {0}, isStackable: {1}", Index, bStackable);
	
	// Get Grid Fragment
	const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	
	// Get Icon Fragment
	const FPlugInv_ImageFragment* ImageFragment = GetFragment<FPlugInv_ImageFragment>(NewItem, FragmentTags::IconFragment);

	// Early return
	if (!GridFragment || !ImageFragment)
	{
		return;
	}

	// Create a slotted item widget
	UPlugInv_SlottedItem* SlottedItem = CreateSlottedItem(NewItem, Index, bStackable, StackAmount, GridFragment, ImageFragment);

	// Add the widget to the canvas panel
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);
	
	// Store the widget in a container for later CRUD
	SlottedItemMap.Add(Index, SlottedItem);
}


UPlugInv_SlottedItem* UPlugInv_InventoryGrid::CreateSlottedItem(UPlugInv_InventoryItem* NewItem, const int32 Index,const bool bStackable, const int32 StackAmount,
																const FPlugInv_GridFragment* GridFragment,
																const FPlugInv_ImageFragment* ImageFragment) const
{
	// Create the actual widget and set its properties
	UPlugInv_SlottedItem* SlottedItem = CreateWidget<UPlugInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(NewItem);
	SetSlottedImage(GridFragment, ImageFragment, SlottedItem);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackAmount(StackUpdateAmount);
	
	return SlottedItem;
}

void UPlugInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FPlugInv_GridFragment* GridFragment,
	UPlugInv_SlottedItem* SlottedItem) const
{
	// Add the previously created widget to the canvas panel through utility functions
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	const FVector2D DrawSize = GetDrawSize(GridFragment);
	CanvasPanelSlot->SetSize(DrawSize);
	
	const FVector2D DrawPos = UPlugInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasPanelSlot->SetPosition(DrawPosWithPadding);

	LOG_DOUBLE_S("InventoryGrid::AddSlottedItemToCanvas : DrawSize.X: {0}, DrawPos: {1}, DrawPosWithPadding: {2}, TileSize: {3}", DrawSize, DrawPos, DrawPosWithPadding, TileSize);

}

void UPlugInv_InventoryGrid::UpdateGridSlots(UPlugInv_InventoryItem* NewItem, const int32 Index, const bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem)
	{
		// Upper left index holds the stack amount
		GridSlots[Index]->SetStackCount(StackAmount);
	}
	
	const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);

	UPlugInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UPlugInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Occupied);
		GridSlot->SetAvailable(false);
	});
}

void UPlugInv_InventoryGrid::SetSlottedImage(const FPlugInv_GridFragment* GridFragment, const FPlugInv_ImageFragment* ImageFragment, const UPlugInv_SlottedItem* SlottedItem) const
{
	// Set the brush properties
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

FVector2D UPlugInv_InventoryGrid::GetDrawSize(const FPlugInv_GridFragment* GridFragment) const
{
	// Returns the draw size taking into account multiple properties
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	return GridFragment->GetGridSize() * IconTileWidth;
}

bool UPlugInv_InventoryGrid::MatchesCategory(const UPlugInv_InventoryItem* Item) const
{
	return this->ItemCategory == Item->GetItemManifest().GetItemCategory();
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventoryGrid::HasRoomForItem(const UPlugInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventoryGrid::HasRoomForItem(const UPlugInv_InventoryItem* InventoryItem)
{
	return HasRoomForItem(InventoryItem->GetItemManifest());
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventoryGrid::HasRoomForItem(const FPlugInv_ItemManifest& ItemManifest)
{
	// Just for testing purposes for now
	FPlugInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 7;
	Result.bStackable = true;
	
	FPlugInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 2;
	SlotAvailability.Index = 0;
	Result.SlotAvailabilities.Add(SlotAvailability);

	FPlugInv_SlotAvailability SlotAvailability2;
	SlotAvailability2.AmountToFill = 5;
	SlotAvailability2.Index = 1;
	Result.SlotAvailabilities.Add(SlotAvailability2);

	LOG_DOUBLE_S("InventoryGrid::HasRoomForItem: TotalRoomToFill: {0}, Remainder: {1}, bStackable: {2}", Result.TotalRoomToFill, Result.Remainder, Result.bStackable);

	return Result;	
}


