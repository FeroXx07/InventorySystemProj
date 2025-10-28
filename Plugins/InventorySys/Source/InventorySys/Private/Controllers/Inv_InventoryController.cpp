// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/Inv_InventoryController.h"
#include "Components/Inv_InventoryComponent.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"

UInv_InventoryController::UInv_InventoryController()
{
	bIsDragging = false;
	DraggedItem = nullptr;
	SelectedItem = nullptr;
	DragStartPosition = FIntPoint::ZeroValue;
	CurrentRotation = 0.0f;
	bAllowRotation = true;
	RotationStep = 90.0f;
	bAutoStack = true;
}

// ============================================
// LIFECYCLE
// ============================================

void UInv_InventoryController::Initialize(UInv_InventoryComponent* InInventoryComponent)
{
	InventoryComponent = InInventoryComponent;
	
	if (InventoryComponent && InventoryComponent->GetContainerCount() > 0)
	{
		// Set first container as current by default
		CurrentContainer = InventoryComponent->GetContainerAtIndex(0);
	}
}

void UInv_InventoryController::Shutdown()
{
	// Clear all state
	CancelDrag();
	DeselectItem();
	
	InventoryComponent = nullptr;
	CurrentContainer = nullptr;
}

// ============================================
// CONTAINER MANAGEMENT
// ============================================

void UInv_InventoryController::SetCurrentContainer(UInv_InventoryContainer* Container)
{
	if (CurrentContainer == Container)
	{
		return;
	}

	UInv_InventoryContainer* OldContainer = CurrentContainer;
	CurrentContainer = Container;

	// Cancel any ongoing operations when switching containers
	if (bIsDragging)
	{
		CancelDrag();
	}

	OnContainerChanged.Broadcast(OldContainer, CurrentContainer);
}

void UInv_InventoryController::SetCurrentContainerByName(FName ContainerName)
{
	if (!InventoryComponent)
	{
		return;
	}

	UInv_InventoryContainer* Container = InventoryComponent->GetContainer(ContainerName);
	if (Container)
	{
		SetCurrentContainer(Container);
	}
}

// ============================================
// INPUT HANDLING
// ============================================

void UInv_InventoryController::HandleCellClicked(int32 X, int32 Y, bool bLeftButton, bool bRightButton)
{
	if (!CurrentContainer)
	{
		return;
	}

	FIntPoint ClickPosition(X, Y);

	if (bIsDragging)
	{
		// Try to place dragged item
		if (bLeftButton)
		{
			EndDrag(ClickPosition);
		}
		else if (bRightButton)
		{
			// Rotate during drag
			HandleRotationRequested(RotationStep);
		}
	}
	else
	{
		// Try to pick up item at position
		UInv_ItemInstance* ItemAtPosition = GetItemAtPosition(ClickPosition);
		
		if (ItemAtPosition)
		{
			if (bLeftButton)
			{
				// Start dragging the item
				StartDrag(ItemAtPosition, ClickPosition);
			}
			else if (bRightButton)
			{
				// Right click - select or use item
				SelectItem(ItemAtPosition, ClickPosition);
			}
		}
		else
		{
			// Clicked empty space - deselect
			DeselectItem();
		}
	}
}

void UInv_InventoryController::HandleCellRightClicked(int32 X, int32 Y)
{
	HandleCellClicked(X, Y, false, true);
}

void UInv_InventoryController::HandleMouseMoved(FIntPoint GridPosition)
{
	if (bIsDragging && CurrentContainer)
	{
		// Update drag validity for visual feedback
		bool bValid = CanPlaceDraggedItemAt(GridPosition);
		NotifyDragValidityChanged(GridPosition, bValid);
	}
}

void UInv_InventoryController::HandleRotationRequested(float RotationDelta)
{
	if (!bAllowRotation)
	{
		return;
	}

	if (bIsDragging && DraggedItem)
	{
		RotateItem(RotationDelta);
	}
	else if (SelectedItem)
	{
		// Rotate selected item in place
		const UInv_ItemDefinition* ItemDef = SelectedItem->GetItemDefinition();
		if (ItemDef && ItemDef->bCanRotate)
		{
			FInv_ItemPlacement* Placement = CurrentContainer->FindItemPlacement(SelectedItem);
			if (Placement)
			{
				float NewRotation = Placement->Rotation + RotationDelta;
				TryMoveItem(SelectedItem, Placement->GridPosition, NewRotation);
			}
		}
	}
}

void UInv_InventoryController::HandleCancelRequested()
{
	if (bIsDragging)
	{
		CancelDrag();
	}
	else if (SelectedItem)
	{
		DeselectItem();
	}
}

// ============================================
// ITEM OPERATIONS
// ============================================

bool UInv_InventoryController::TryPickupItem(UInv_ItemInstance* Item)
{
	if (!Item || !InventoryComponent)
	{
		return false;
	}

	// Try to add to any container
	FName OutContainerName;
	FIntPoint OutPosition;
	return InventoryComponent->TryAddItemToAnyContainer(Item, OutContainerName, OutPosition, 0.0f);
}

bool UInv_InventoryController::TryPlaceItem(UInv_ItemInstance* Item, FIntPoint Position, float Rotation)
{
	if (!Item || !CurrentContainer)
	{
		return false;
	}

	bool bSuccess = CurrentContainer->TryPlaceItem(Item, Position, Rotation);
	
	if (bSuccess)
	{
		OnItemPlaced.Broadcast(Item, Position, Rotation);
	}

	return bSuccess;
}

bool UInv_InventoryController::TryPlaceItemAtBestPosition(UInv_ItemInstance* Item)
{
	if (!Item || !CurrentContainer)
	{
		return false;
	}

	// Try different rotations
	TArray<float> RotationsToTry = {0.0f, 90.0f, 180.0f, 270.0f};
	
	for (float Rotation : RotationsToTry)
	{
		// Try each position in the grid
		for (int32 Y = 0; Y < CurrentContainer->GridHeight; ++Y)
		{
			for (int32 X = 0; X < CurrentContainer->GridWidth; ++X)
			{
				FIntPoint Position(X, Y);
				if (CurrentContainer->CanPlaceItemAt(Item, Position, Rotation))
				{
					return TryPlaceItem(Item, Position, Rotation);
				}
			}
		}
	}

	return false;
}

bool UInv_InventoryController::TryRemoveItem(UInv_ItemInstance* Item)
{
	if (!Item || !CurrentContainer)
	{
		return false;
	}

	bool bSuccess = CurrentContainer->TryRemoveItem(Item);
	
	if (bSuccess)
	{
		OnItemRemoved.Broadcast(Item);
	}

	return bSuccess;
}

bool UInv_InventoryController::TryMoveItem(UInv_ItemInstance* Item, FIntPoint NewPosition, float NewRotation)
{
	if (!Item || !CurrentContainer)
	{
		return false;
	}

	bool bSuccess = CurrentContainer->TryMoveItem(Item, NewPosition, NewRotation);
	
	if (bSuccess)
	{
		OnItemMoved.Broadcast(Item, NewPosition, NewRotation);
	}

	return bSuccess;
}

// ============================================
// DRAG OPERATIONS
// ============================================

void UInv_InventoryController::StartDrag(UInv_ItemInstance* Item, FIntPoint StartPosition)
{
	if (!Item || !CurrentContainer || bIsDragging)
	{
		return;
	}

	// Verify item exists in container
	if (!CurrentContainer->ContainsItem(Item))
	{
		return;
	}

	// Store drag state
	DraggedItem = Item;
	DragStartPosition = StartPosition;
	bIsDragging = true;

	// Get current rotation from placement
	FInv_ItemPlacement* Placement = CurrentContainer->FindItemPlacement(Item);
	if (Placement)
	{
		CurrentRotation = Placement->Rotation;
	}
	else
	{
		CurrentRotation = 0.0f;
	}

	// Remove item from grid temporarily
	CurrentContainer->TryRemoveItem(Item);

	OnDragStarted.Broadcast(Item, StartPosition);
}

void UInv_InventoryController::UpdateDrag(FIntPoint NewPosition)
{
	if (!bIsDragging)
	{
		return;
	}

	HandleMouseMoved(NewPosition);
}

bool UInv_InventoryController::EndDrag(FIntPoint Position)
{
	if (!bIsDragging || !DraggedItem || !CurrentContainer)
	{
		return false;
	}

	// Try to place at target position
	bool bSuccess = CurrentContainer->TryPlaceItem(DraggedItem, Position, CurrentRotation);

	if (!bSuccess)
	{
		// Failed - try to return to start position
		bSuccess = CurrentContainer->TryPlaceItem(DraggedItem, DragStartPosition, CurrentRotation);
		
		if (!bSuccess)
		{
			// Still failed - try any valid position (shouldn't happen)
			bSuccess = TryPlaceItemAtBestPosition(DraggedItem);
		}
	}

	UInv_ItemInstance* PlacedItem = DraggedItem;
	FIntPoint FinalPosition = bSuccess ? Position : DragStartPosition;
	
	ClearDragState();

	OnDragEnded.Broadcast(PlacedItem, bSuccess);
	
	if (bSuccess)
	{
		OnItemPlaced.Broadcast(PlacedItem, FinalPosition, CurrentRotation);
	}

	return bSuccess;
}

void UInv_InventoryController::CancelDrag()
{
	if (!bIsDragging || !DraggedItem || !CurrentContainer)
	{
		return;
	}

	// Return item to start position
	CurrentContainer->TryPlaceItem(DraggedItem, DragStartPosition, CurrentRotation);

	UInv_ItemInstance* CancelledItem = DraggedItem;
	
	ClearDragState();

	OnDragEnded.Broadcast(CancelledItem, false);
}

void UInv_InventoryController::ClearDragState()
{
	bIsDragging = false;
	DraggedItem = nullptr;
	DragStartPosition = FIntPoint::ZeroValue;
	CurrentRotation = 0.0f;
}

// ============================================
// SELECTION
// ============================================

void UInv_InventoryController::SelectItem(UInv_ItemInstance* Item, FIntPoint Position)
{
	if (SelectedItem == Item)
	{
		return;
	}

	DeselectItem();

	SelectedItem = Item;
	OnItemSelected.Broadcast(Item, Position);
}

void UInv_InventoryController::DeselectItem()
{
	if (!SelectedItem)
	{
		return;
	}

	UInv_ItemInstance* PreviousSelection = SelectedItem;
	SelectedItem = nullptr;
	OnItemDeselected.Broadcast(PreviousSelection);
}

bool UInv_InventoryController::IsItemSelected(UInv_ItemInstance* Item) const
{
	return SelectedItem == Item;
}

// ============================================
// ROTATION
// ============================================

void UInv_InventoryController::RotateItem(float RotationDelta)
{
	CurrentRotation = FMath::Fmod(CurrentRotation + RotationDelta, 360.0f);
	if (CurrentRotation < 0.0f)
	{
		CurrentRotation += 360.0f;
	}

	if (DraggedItem)
	{
		OnRotationChanged.Broadcast(DraggedItem, CurrentRotation);
	}
}

void UInv_InventoryController::SetRotation(float NewRotation)
{
	CurrentRotation = FMath::Fmod(NewRotation, 360.0f);
	if (CurrentRotation < 0.0f)
	{
		CurrentRotation += 360.0f;
	}

	if (DraggedItem)
	{
		OnRotationChanged.Broadcast(DraggedItem, CurrentRotation);
	}
}

// ============================================
// VALIDATION
// ============================================

bool UInv_InventoryController::CanPlaceItemAt(UInv_ItemInstance* Item, FIntPoint Position, float Rotation) const
{
	if (!Item || !CurrentContainer)
	{
		return false;
	}

	return CurrentContainer->CanPlaceItemAt(Item, Position, Rotation);
}

bool UInv_InventoryController::CanPlaceDraggedItemAt(FIntPoint Position) const
{
	if (!bIsDragging || !DraggedItem)
	{
		return false;
	}

	return CanPlaceItemAt(DraggedItem, Position, CurrentRotation);
}

TArray<FIntPoint> UInv_InventoryController::GetDraggedItemShapeAtPosition(FIntPoint Position) const
{
	if (!bIsDragging || !DraggedItem || !CurrentContainer)
	{
		return TArray<FIntPoint>();
	}

	return CurrentContainer->GetItemShapeAtPosition(DraggedItem, Position, CurrentRotation);
}

// ============================================
// INTERNAL HELPERS
// ============================================

void UInv_InventoryController::NotifyDragValidityChanged(FIntPoint Position, bool bValid)
{
	OnDragValidityChanged.Broadcast(Position, bValid);
}

UInv_ItemInstance* UInv_InventoryController::GetItemAtPosition(FIntPoint Position) const
{
	if (!CurrentContainer)
	{
		return nullptr;
	}

	// Find item that occupies this position
	for (const FInv_ItemPlacement& Placement : CurrentContainer->ItemPlacements)
	{
		if (Placement.ContainsPosition(Position))
		{
			return Placement.ItemInstance;
		}
	}

	return nullptr;
}


