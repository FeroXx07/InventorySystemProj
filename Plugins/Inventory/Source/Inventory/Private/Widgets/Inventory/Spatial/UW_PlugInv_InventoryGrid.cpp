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
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
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

bool UPlugInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const TObjectPtr<UPlugInv_GridSlot>& GridSlot)
{
	return CheckedIndices.Contains(GridSlot->GetTileIndex());
}

FPlugInv_SlotAvailabilityResult UPlugInv_InventoryGrid::HasRoomForItem(const FPlugInv_ItemManifest& ItemManifest)
{
	FPlugInv_SlotAvailabilityResult Result;

	// Determine if the item is stackable.
	// Determine how many stacks to add.
	// For each Grid Slot:
		// If we don't have anymore to fill, break out of the loop early.
		// Is this index claimed yet?
		// Can the item fit here? (i.e. is it out of grid bounds?)
		// Is there room at this index? (i.e. are there other items in the way?)
		// Check any other important conditions - ForEach2D over a 2D range
			// Index claimed?
			// Has valid item?
			// Is this item the same type as the item we're trying to add?
			// If so, is this a stackable item?
			// If stackable, is this slot at the max stack size already?
		// How much to fill?
		// Update the amount left to fill
	// How much is the remainder

	// Determine if the item is stackable.
	const FPlugInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfType<FPlugInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;
	
	// Determine how many stacks to add.
	const int32 MaxStackSize = Result.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable ? StackableFragment->GetStackCount() : 1;

	// Container of unique values unlike arrays.
	TSet<int32> CheckedIndices;
	// For each Grid Slot:
	for (const TObjectPtr<UPlugInv_GridSlot>& GridSlot : GridSlots)
	{
		// If we don't have anymore to fill, break out of the loop early.
		if (AmountToFill == 0)
		{
			break;
		}
		
		// Is this index claimed yet?
		if (IsIndexClaimed(CheckedIndices, GridSlot))
		{
			continue;
		}
		
		
		// Can the item fit here? (i.e. is it out of grid bounds?)
		const FPlugInv_GridFragment* GridFragment = ItemManifest.GetFragmentOfType<FPlugInv_GridFragment>();
		const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
		if (!IsInGridBounds(GridSlot->GetTileIndex(), Dimensions))
		{
			continue;
		}
		
		TSet<int32> TentativelyClaimedIndices;
		bool bHasRoomAtIndex = true;

		// From the current upper left index check through its item grid size all potential indices
		// Is there room at this index? (i.e. are there other items in the way?
		UPlugInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetTileIndex(), Dimensions, Columns,
			[&](const UPlugInv_GridSlot* SubGridSlot)
		{
				if (CheckSlotConstraints(ItemManifest, GridSlot, SubGridSlot, CheckedIndices, TentativelyClaimedIndices))
				{
					TentativelyClaimedIndices.Add(SubGridSlot->GetTileIndex());
				}
				else
				{
					bHasRoomAtIndex = false;
				}
		});

		// Single invalid index issues early continue, thus not appending to checked indices
		if (bHasRoomAtIndex == false)
		{
			continue;
		}
		
		// How much to fill in slot?
		const int32 CurrentSlotStackCount = GetSlotStackAmount(GridSlot);
		const int32 RoomInSlot = MaxStackSize - CurrentSlotStackCount;
		const int32 AmountToFillInSlot = Result.bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;

		if (AmountToFillInSlot == 0)
		{
			continue;
		}

		// Only append to checked indices if all valid
		CheckedIndices.Append(TentativelyClaimedIndices);
		
		// Update the amount left to fill
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FPlugInv_SlotAvailability{
				GridSlot->GetInventoryItem().IsValid() ? GridSlot->GetUpperLeftIndex() : GridSlot->GetTileIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				GridSlot->GetInventoryItem().IsValid()
			}
		);

		// Update the filling left
		AmountToFill -= AmountToFillInSlot;

		// How much is the remainder
		Result.Remainder = AmountToFill;

		// Exit if no more to fill left
		if (AmountToFill == 0)
		{
			return Result;
		}
	}
	
	LOG_DOUBLE_S("InventoryGrid::HasRoomForItem: TotalRoomToFill: {0}, Remainder: {1}, bStackable: {2}", Result.TotalRoomToFill, Result.Remainder, Result.bStackable);
	return Result;	
}

bool UPlugInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num())
	{
		return false;
	}

	// The column where the rightmost square in this item is going to be.
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

bool UPlugInv_InventoryGrid::CheckSlotConstraints(const FPlugInv_ItemManifest& ItemManifest, const UPlugInv_GridSlot* GridSlot, const UPlugInv_GridSlot* SubGridSlot, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimedIndices) const
{
	// If valid index adds to potential index
	// Check any other important conditions - ForEach2D over a 2D range
	// Index claimed?
	if (CheckedIndices.Contains(SubGridSlot->GetTileIndex()))
	{
		return false;
	}
					
	// Has valid item?
	if (SubGridSlot->GetInventoryItem().IsValid() == false)
	{
		OutTentativelyClaimedIndices.Add(SubGridSlot->GetTileIndex());
		return true;
	}

	// Is this GridSlot the anchor (Upper Left Slot)
	if (SubGridSlot->GetUpperLeftIndex() != GridSlot->GetTileIndex())
	{
		return false;
	}

	// Is this a stackable item?
	const TWeakObjectPtr<UPlugInv_InventoryItem> SubItem = SubGridSlot->GetInventoryItem().Get();
	const FPlugInv_StackableFragment* StackableFragment = SubItem->GetItemManifestMutable().GetFragmentOfType<FPlugInv_StackableFragment>();
	if (StackableFragment == nullptr)
	{
		return false;
	}
	
	// Is this item the same type as the item we're trying to add?
	const FGameplayTag& ItemTypeTag = ItemManifest.GetItemType();
	if (DoesItemTypeMatch(SubItem, ItemTypeTag) == false)
	{
		return false;
	}
	
	// If stackable, is this slot at the max stack size already?
	if (GridSlot->GetStackCount() >= StackableFragment->GetMaxStackSize())
	{
		return false;
	}
	
	return true;
}

bool UPlugInv_InventoryGrid::DoesItemTypeMatch(const TWeakObjectPtr<UPlugInv_InventoryItem> SubItem,
	const FGameplayTag& ItemTypeTag) const
{
	return SubItem->GetItemManifestMutable().GetItemType().MatchesTagExact(ItemTypeTag);
}

int32 UPlugInv_InventoryGrid::GetSlotStackAmount(const TObjectPtr<UPlugInv_GridSlot>& GridSlot)
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	// If it is a slot that doesn't hold stack count (i.e. not being the upper left, then get the upper left index)
	const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex();
	if (UpperLeftIndex != INDEX_NONE)
	{
		const UPlugInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}
	return CurrentSlotStackCount;
}

void UPlugInv_InventoryGrid::AddStacks(const FPlugInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get()))
	{
		return;
	}
	
	for (const FPlugInv_SlotAvailability& SlotAvailability : Result.SlotAvailabilities)
	{
		if (SlotAvailability.bItemAtIndex)
		{
			UPlugInv_GridSlot* GridSlot = GridSlots[SlotAvailability.Index];
			const UPlugInv_SlottedItem* SlottedItem = SlottedItemMap.FindChecked(SlotAvailability.Index);
			SlottedItem->UpdateStackAmount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + SlotAvailability.AmountToFill);
		}
		else
		{
			AddItemToIndex(Result.Item.Get(), SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), SlotAvailability.Index, Result.bStackable, SlotAvailability.AmountToFill);
		}
	}
}



