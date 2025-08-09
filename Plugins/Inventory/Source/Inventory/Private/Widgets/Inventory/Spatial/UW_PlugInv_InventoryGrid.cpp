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
#include "Widgets/Inventory/HoverItem/UW_PlugInv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/UW_PlugInv_SlottedItem.h"
#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"
#include "Widgets/ItemPopUp/UW_PlugInv_ItemPopUp.h"

void UPlugInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

void UPlugInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Mouse pos relative to the canvas panel
	const FVector2D CanvasPosition = UPlugInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, UPlugInv_WidgetUtils::GetWidgetSize(CanvasPanel), MousePosition))
	{
		return;
	}
	
	UpdateTileParameters(CanvasPosition, MousePosition);
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
			GridSlot->OnGridSlotHovered.AddDynamic(this, &ThisClass::UPlugInv_InventoryGrid::OnGridSlotHovered);
			GridSlot->OnGridSlotUnhovered.AddDynamic(this, &ThisClass::UPlugInv_InventoryGrid::OnGridSlotUnhovered);
			GridSlot->OnGridSlotClicked.AddDynamic(this, &ThisClass::UPlugInv_InventoryGrid::UPlugInv_InventoryGrid::OnGridSlotClicked);
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
	LOG_DOUBLE_S(FColor::Orange,"InventoryGrid::AddItemToIndex : Adding Item with index: {0}, isStackable: {1}", Index, bStackable);
	
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
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::UPlugInv_InventoryGrid::OnSlottedItemClicked);
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

	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::AddSlottedItemToCanvas : DrawSize.X: {0}, DrawPos: {1}, DrawPosWithPadding: {2}, TileSize: {3}", DrawSize, DrawPos, DrawPosWithPadding, TileSize);

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

void UPlugInv_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UPlugInv_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());
}

void UPlugInv_InventoryGrid::SetOwningCanvas(UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
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
	
	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::HasRoomForItem: TotalRoomToFill: {0}, Remainder: {1}, bStackable: {2}", Result.TotalRoomToFill, Result.Remainder, Result.bStackable);
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

bool UPlugInv_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UPlugInv_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UPlugInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::OnSlottedItemClicked : Clicked on item at index {0}", GridIndex);
	check(GridSlots.IsValidIndex(GridIndex));
	UPlugInv_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (IsValid(HoverItem) == false && IsLeftClick(MouseEvent))
	{
		// Pick Up, Assign the hover item, and remove the slotted item from the grid
		PickUp(ClickedInventoryItem, GridIndex);
		return;
	}

	if (IsRightClick(MouseEvent))
	{
		CreateItemPopUp(GridIndex);
		return;
	}
	
	// Do the hovered item and the clicked inventory item share a type, and are they stackable?
	if (IsHoverAndClickedSameTypeAndStackable(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FPlugInv_StackableFragment* StackableFragment = ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FPlugInv_StackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 HoveredStackCount = HoverItem->GetStackCount();

		// Should we swap their stack counts? (Room in the clicked slot == 0 && HoveredStackCount < MaxStackSize)
		if (ShouldSwapStackCounts(RoomInClickedSlot, HoveredStackCount, MaxStackSize))
		{
			SwapStackCounts(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		
		// Should we consume the hover item's stacks? (Room in the clicked slot >= HoveredStackCount)
		if (ShouldConsumeHoverItemStacks(HoveredStackCount, RoomInClickedSlot))
		{
			ConsumeHoverItemStacks(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		
		// Should we fill in the stacks of the clicked item? (and not consume the hover item)
		if (ShouldFillInStack(RoomInClickedSlot, HoveredStackCount))
		{
			FillInStack(RoomInClickedSlot, HoveredStackCount - RoomInClickedSlot, GridIndex);
			return;
		}
		
		// Clicked slot Full.
		if (RoomInClickedSlot == 0)
		{
			return;
		}
	}
		
	// Swap with the hover item.
	SwapClickedWithHoverItem(ClickedInventoryItem, GridIndex);
}

void UPlugInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem)) return;
	if (!GridSlots.IsValidIndex(ItemDropIndex)) return;

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		// Clicked on a location that has a slotted event.
		OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	TObjectPtr<UPlugInv_GridSlot> GridSlot = GridSlots[GridIndex];
	if (!GridSlot->GetInventoryItem().IsValid())
	{
		// Clicked on an empty location
		PutDownOnIndex(ItemDropIndex);
	}
}

void UPlugInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	// Already doing a 2d range hover with the hover item, no need of this.
	if (IsValid(HoverItem)) return;

	UPlugInv_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Occupied);
	}
}

void UPlugInv_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	// Already doing a 2d range hover with the hover item, no need of this.
	if (IsValid(HoverItem)) return;
	UPlugInv_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState:: Unoccupied);
	}
}

void UPlugInv_InventoryGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	UPlugInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;
	if (!RightClickedItem->IsStackable()) return;
	
	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UPlugInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;
	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackAmount(NewStackCount);
	
	AssignHoverItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void UPlugInv_InventoryGrid::OnPopUpMenuDrop(int32 Index)
{
	UPlugInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;
	
	PickUp(RightClickedItem, Index);
	DropItem();
}

void UPlugInv_InventoryGrid::OnPopUpMenuConsume(int32 Index)
{
	UPlugInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;
	
	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UPlugInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	
	const int32 NewStackCount = UpperLeftGridSlot->GetStackCount() - 1;
	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItemMap.FindChecked(UpperLeftIndex)->UpdateStackAmount(NewStackCount);

	InventoryComponent->Server_ConsumeItem(RightClickedItem);

	if (NewStackCount <= 0)
	{
		RemoveItemFromGrid(RightClickedItem, Index);
	}
}

void UPlugInv_InventoryGrid::PickUp(UPlugInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	// Remove clicked item from the grid
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UPlugInv_InventoryGrid::AssignHoverItem(UPlugInv_InventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UPlugInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}
	
	const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	const FPlugInv_ImageFragment* ImageFragment = GetFragment<FPlugInv_ImageFragment>(InventoryItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;
	

	const FVector2D DrawSize = GetDrawSize(GridFragment);
	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	// Instead of adding it to the viewport, set it as the mouse cursor widget
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}

void UPlugInv_InventoryGrid::AssignHoverItem(UPlugInv_InventoryItem* InventoryItem, const int32 GridIndex,
	const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
	HoverItem->UpdateStackCount(InventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UPlugInv_InventoryGrid::RemoveItemFromGrid(UPlugInv_InventoryItem* InventoryItem, const int32 GridIndex)
{
	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::RemoveItemFromGrid : Removed item at index {0}", GridIndex);

	const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UPlugInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UPlugInv_GridSlot* GridSlot)
	{
		GridSlot->SetInventoryItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Unoccupied);
		GridSlot->SetAvailable(true);
		GridSlot->SetStackCount(0);
	});

	if (SlottedItemMap.Contains(GridIndex))
	{
		TObjectPtr<UPlugInv_SlottedItem> FoundSlottedItem;
		SlottedItemMap.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

void UPlugInv_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	// if mouse not in canvas panel, return.
	if (!bCurrentMouseWithinCanvas)
	{
		return;
	}
	
	// Calculate the tile quadrant
	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);
	LastTileParameters = TileParameters;

	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = UPlugInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPosition, MousePosition);

	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::UpdateTileParameters : Parameters.TileIndex: {0}, Parameters.TileQuadrant: {1}, Parameters.TileCoordinates: {2}, ", TileParameters.TileIndex, UEnum::GetDisplayValueAsText(TileParameters.TileQuadrant).ToString(), TileParameters.TileCoordinates);
	
	// Handle highlight/unhighlight of the grid slots
	OnTileParametersUpdated(TileParameters);
}

FIntPoint UPlugInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition,
	const FVector2D& MousePosition) const
{
	// Calculate relative position inside canvas and floor it
	return FIntPoint{
		static_cast<int32>(FMath::FloorToInt((MousePosition.X - CanvasPosition.X) / TileSize)),
		static_cast<int32>(FMath::FloorToInt((MousePosition.Y - CanvasPosition.Y) / TileSize))
	};
}

EPlugInv_TileQuadrant UPlugInv_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPosition,
	const FVector2D& MousePosition) const
{
	// Calculate relative position within the current tile, with the remainder -> Fmod()
	const float TileLocalX = FMath::Fmod(MousePosition.X - CanvasPosition.X, TileSize);
	const float TileLocalY = FMath::Fmod(MousePosition.Y - CanvasPosition.Y, TileSize);

	// Determine which quadrant the mouse is in
	const bool bIsTop = TileLocalY < TileSize / 2.f; // Top if Y is in the upper half
	const bool bIsLeft = TileLocalX < TileSize / 2.f; // Left if X is in the left half

	EPlugInv_TileQuadrant HoveredTileQuadrant{EPlugInv_TileQuadrant::None};
	if (bIsTop && bIsLeft) HoveredTileQuadrant = EPlugInv_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = EPlugInv_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = EPlugInv_TileQuadrant::BottomLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = EPlugInv_TileQuadrant::BottomRight;
	
	return HoveredTileQuadrant;
}

void UPlugInv_InventoryGrid::OnTileParametersUpdated(const FPlugInv_TileParameters& Parameters)
{
	if (!IsValid(HoverItem)) return;


	// Get Hover Item's dimensions
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();

	// calculate the starting coordinate for highlighting and ItemDropIndex setting
	const FIntPoint StartingCoordinate = CalculateStartingCoordinate(Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = UPlugInv_WidgetUtils::GetIndexFromPosition(StartingCoordinate, Columns);
	
	// check hover position
	CurrentQueryResult = CheckHoverPosition(StartingCoordinate, Dimensions);

	LOG_DOUBLE_S(FColor::Orange, "InventoryGrid::OnTileParametersUpdated : ItemDropIndex = {0}, QuerybHasSpace? = {1}, QueryHasItem = {2}, QueryUpperLeftIndex = {3}"
		, ItemDropIndex, CurrentQueryResult.bHasSpace, CurrentQueryResult.ValidItem.IsValid(), CurrentQueryResult.UpperLeftIndex);

	// in the grid bounds?
	if (CurrentQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}
	
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		// TODO: There's a single item in this space. We can swap or add stacks.
		const FPlugInv_GridFragment* GridFragment = GetFragment<FPlugInv_GridFragment>(CurrentQueryResult.ValidItem.Get(), FragmentTags::GridFragment);
		if (!GridFragment) return;
		ChangeHoverType(CurrentQueryResult.UpperLeftIndex, GridFragment->GetGridSize(), EPlugInv_GridSlotState::GrayedOut);
	}

	// any items in the way?


	// if so, is there only one item in the way? (can we swap?)
}

FIntPoint UPlugInv_InventoryGrid::CalculateStartingCoordinate(const FIntPoint& Coordinates, const FIntPoint& Dimensions,
	const EPlugInv_TileQuadrant Quadrant) const
{
	// Are dimensions properties width, height even or odd
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0;

	// If even just change highlighting based on the quadrant in
	// if odd just change highlighting based on the tile in
	FIntPoint StartingCoord;
	switch (Quadrant)
	{
	case EPlugInv_TileQuadrant::TopLeft:
		StartingCoord.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
	case EPlugInv_TileQuadrant::TopRight:
		StartingCoord.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
	case EPlugInv_TileQuadrant::BottomLeft:
		StartingCoord.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartingCoord.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	case EPlugInv_TileQuadrant::BottomRight:
		StartingCoord.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartingCoord.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	default:
		LOG_DOUBLE_ERROR_S("Invalid quadrant");
		return FIntPoint(-1, -1);
	}
	return StartingCoord;
}

FInv_SpaceQueryResult UPlugInv_InventoryGrid::CheckHoverPosition(const FIntPoint& Position,
	const FIntPoint& Dimensions)
{
	FInv_SpaceQueryResult Result; // Default false constructor
	
	// In the grid bounds?
	int32 StartingIndex = UPlugInv_WidgetUtils::GetIndexFromPosition(Position, Columns);
	if (IsInGridBounds(StartingIndex, Dimensions) == false)
	{
		return Result; 
	}
	
	// If more than one of the indices is occupied with the same item, we need to se if they all have the same upper left index.
	// Container of unique values unlike arrays.
	TSet<int32> OccupiedUpperLeftIndices;

	UPlugInv_InventoryStatics::ForEach2D(GridSlots, UPlugInv_WidgetUtils::GetIndexFromPosition(Position, Columns), Dimensions, Columns, [&](const UPlugInv_GridSlot* GridSlot)
	{
		if (GridSlot->GetInventoryItem().IsValid())
		{
			OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftIndex());
			Result.bHasSpace = false;
		}
	});

	if (OccupiedUpperLeftIndices.IsEmpty())
	{
		Result.bHasSpace = true;
	}
	
	// if so, is there only one item in the way? (can we swap?)

	if (OccupiedUpperLeftIndices.Num() == 1) // single item at position - it's valid for swapping/combining
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlots[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	}
	return Result;
}

bool UPlugInv_InventoryGrid::CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize,
	const FVector2D& Location)
{
	bLastMouseWithinCanvas = bCurrentMouseWithinCanvas;
	bCurrentMouseWithinCanvas = UPlugInv_WidgetUtils::IsWithinBounds(BoundaryPos, BoundarySize, Location);
	if (bCurrentMouseWithinCanvas == false && bLastMouseWithinCanvas)
	{
		UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true;
		
	}
	return false;
}

void UPlugInv_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bCurrentMouseWithinCanvas) return;
	
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UPlugInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UPlugInv_GridSlot* GridSlot)
	{
		GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Occupied);
	});
	
	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;
}

void UPlugInv_InventoryGrid::UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UPlugInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UPlugInv_GridSlot* GridSlot)
	{
		if (GridSlot->IsAvailable())
		{
			GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Unoccupied);
		}
		else
		{
			GridSlot->SetStateAndBrushTexture(EPlugInv_GridSlotState::Occupied);
		}
	});
}

void UPlugInv_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions,
	EPlugInv_GridSlotState GridSlotState)
{
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UPlugInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [State = GridSlotState](UPlugInv_GridSlot* GridSlot)
	{
		GridSlot->SetStateAndBrushTexture(State);
	});
	
	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

void UPlugInv_InventoryGrid::PutDownOnIndex(const int32 Index)
{
	check(HoverItem);
	
	AddItemToIndex(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	UpdateGridSlots(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	ClearHoverItem();
}

void UPlugInv_InventoryGrid::ClearHoverItem()
{
	if (!IsValid(HoverItem)) return;

	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());
	HoverItem->RemoveFromParent();
	HoverItem = nullptr;

	ShowCursor();
}

UUserWidget* UPlugInv_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}
	return VisibleCursorWidget;
}

UUserWidget* UPlugInv_InventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;
	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}
	return HiddenCursorWidget;
}

bool UPlugInv_InventoryGrid::IsHoverAndClickedSameTypeAndStackable(
	const UPlugInv_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();
	return bIsSameItem && bIsStackable && HoverItem->GetItemType().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemType());
}

void UPlugInv_InventoryGrid::SwapClickedWithHoverItem(UPlugInv_InventoryItem* ClickedInventoryItem,
	const int32 GridIndex)
{
	if (!IsValid(HoverItem)) return;
	
	UPlugInv_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();
	
	// Keep the same previous grid index
	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemToIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

bool UPlugInv_InventoryGrid::ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount,
	const int32 MaxStackSize) const
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void UPlugInv_InventoryGrid::SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	UPlugInv_GridSlot* GridSlot = GridSlots[Index];
	GridSlot->SetStackCount(HoveredStackCount);
	const UPlugInv_SlottedItem* ClickedSlottedItem = SlottedItemMap.FindChecked(Index);
	ClickedSlottedItem->UpdateStackAmount(HoveredStackCount);
	HoverItem->UpdateStackCount(ClickedStackCount);
}

bool UPlugInv_InventoryGrid::ShouldConsumeHoverItemStacks(const int32 HoveredStackCount,
	const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void UPlugInv_InventoryGrid::ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;
	
	GridSlots[Index]->SetStackCount(NewClickedStackCount);
	SlottedItemMap.FindChecked(Index)->UpdateStackAmount(NewClickedStackCount);
	
	ClearHoverItem();
	ShowCursor();
	
	const FPlugInv_GridFragment* GridFragment = GridSlots[Index]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FPlugInv_GridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	HighlightSlots(Index, Dimensions);
}

bool UPlugInv_InventoryGrid::ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const
{
	return RoomInClickedSlot < HoveredStackCount;
}

void UPlugInv_InventoryGrid::FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	UPlugInv_GridSlot* GridSlot = GridSlots[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;
	GridSlot->SetStackCount(NewStackCount);
	UPlugInv_SlottedItem* ClickedSlottedItem = SlottedItemMap.FindChecked(Index);
	ClickedSlottedItem->UpdateStackAmount(NewStackCount);
	HoverItem->UpdateStackCount(Remainder);
}

void UPlugInv_InventoryGrid::CreateItemPopUp(const int32 GridIndex)
{
	UPlugInv_InventoryItem* RightClickedItem = GridSlots[GridIndex]->GetInventoryItem().Get();
	
	if (!IsValid(RightClickedItem)) return;
	if (GridSlots[GridIndex]->GetItemPopUp().IsValid()) return;
	
	ItemPopUp = CreateWidget<UPlugInv_ItemPopUp>(this, ItemPopUpClass);
	GridSlots[GridIndex]->SetItemPopUp(ItemPopUp);
	OwningCanvasPanel->AddChild(ItemPopUp);
	// Same alternative UCanvasPanelSlot* CanvasSlot = OwningCanvasPanel->AddChildToCanvas(ItemPopUp);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemPopUp);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	CanvasSlot->SetPosition(MousePosition - ItemPopUpOffset);
	CanvasSlot->SetSize(ItemPopUp->GetBoxSize());

	const int32 SliderMax = GridSlots[GridIndex]->GetStackCount() - 1;
	if (RightClickedItem->IsStackable() && SliderMax > 0)
	{
		ItemPopUp->OnSplit.BindDynamic(this, &ThisClass::OnPopUpMenuSplit);
		ItemPopUp->SetSliderParams(SliderMax, FMath::Max(1, GridSlots[GridIndex]->GetStackCount() / 2));
	}
	else
	{
		ItemPopUp->CollapseSplitButton();
	}
	
	ItemPopUp->OnDrop.BindDynamic(this, &ThisClass::OnPopUpMenuDrop);
	
	if (RightClickedItem->IsConsumable())
	{
		ItemPopUp->OnConsume.BindDynamic(this, &ThisClass::OnPopUpMenuConsume);
	}
	else
	{
		ItemPopUp->CollapseConsumeButton();
	}
}

void UPlugInv_InventoryGrid::DropItem()
{
	if (!IsValid(HoverItem)) return;
	if (!IsValid(HoverItem->GetInventoryItem())) return;

	InventoryComponent->Server_DropItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount());
	
	ClearHoverItem();
	ShowCursor();
}







