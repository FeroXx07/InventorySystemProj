// Fill out your copyright notice in the Description page of Project Settings.

#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "Inv_SpatialUtilityLibrary.h"
#include "Engine/Engine.h"

UInv_InventoryContainer::UInv_InventoryContainer()
{
	ContainerName = NAME_None;
	DisplayName = FText::GetEmpty();
	GridWidth = 10;
	GridHeight = 10;
	bIsInitialized = false;
	bIsLocked = false;
}

void UInv_InventoryContainer::Initialize(const FName& InContainerName, const FText& InDisplayName, int32 InWidth, int32 InHeight, const FGameplayTagContainer& InTags)
{
	ContainerName = InContainerName;
	DisplayName = InDisplayName;
	GridWidth = FMath::Max(1, InWidth);
	GridHeight = FMath::Max(1, InHeight);
	ContainerTags = InTags;
	
	InitializeGrid();
	bIsInitialized = true;
}

void UInv_InventoryContainer::InitializeGrid()
{
	GridOccupancy.Empty();
	GridOccupancy.SetNum(GridHeight);
	
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		GridOccupancy[Y].SetNum(GridWidth);
		for (int32 X = 0; X < GridWidth; ++X)
		{
			// Row : Y , Col: X
			GridOccupancy[Y][X] = false;
		}
	}
	
	ContainedItems.Empty();
	ItemPlacements.Empty();
}

void UInv_InventoryContainer::ClearGrid()
{
	InitializeGrid();
}

bool UInv_InventoryContainer::IsGridPositionValid(const FIntPoint& Position) const
{
	return Position.X >= 0 && Position.X < GridWidth && 
		   Position.Y >= 0 && Position.Y < GridHeight;
}

bool UInv_InventoryContainer::IsGridPositionOccupied(const FIntPoint& Position) const
{
	if (!IsGridPositionValid(Position))
	{
		return true; // Invalid positions are considered occupied
	}
	
	return GridOccupancy[Position.Y][Position.X];
}

bool UInv_InventoryContainer::CanPlaceItemAt(UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation) const
{
	if (!Item || !bIsInitialized || bIsLocked)
	{
		return false;
	}

	// Get the item's shape at the specified position and rotation
	TArray<FIntPoint> ItemShape = GetItemShapeAtPosition(Item, GridPosition, Rotation);
	
	// Check if all positions in the shape are valid and unoccupied
	for (const FIntPoint& ShapePosition : ItemShape)
	{
		if (!IsGridPositionValid(ShapePosition) || IsGridPositionOccupied(ShapePosition))
		{
			return false;
		}
	}
	
	return true;
}

bool UInv_InventoryContainer::TryPlaceItem(UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation)
{
	if (!CanPlaceItemAt(Item, GridPosition, Rotation))
	{
		return false;
	}

	// Create placement data
	FInv_ItemPlacement NewPlacement;
	NewPlacement.ItemInstance = Item;
	NewPlacement.GridPosition = GridPosition;
	NewPlacement.Rotation = Rotation;
	NewPlacement.OccupiedSlots = GetItemShapeAtPosition(Item, GridPosition, Rotation);

	// Add to containers
	ItemPlacements.Add(NewPlacement);
	ContainedItems.Add(Item);

	// Update grid occupancy
	UpdateGridOccupancy();

	return true;
}

bool UInv_InventoryContainer::TryRemoveItem(UInv_ItemInstance* Item)
{
	if (!Item || bIsLocked)
	{
		return false;
	}

	// Find and remove the item placement
	for (int32 i = ItemPlacements.Num() - 1; i >= 0; --i)
	{
		if (ItemPlacements[i].ItemInstance == Item)
		{
			ItemPlacements.RemoveAt(i);
			break;
		}
	}

	// Remove from contained items
	ContainedItems.Remove(Item);

	// Update grid occupancy
	UpdateGridOccupancy();

	return true;
}

bool UInv_InventoryContainer::TryMoveItem(UInv_ItemInstance* Item, const FIntPoint& NewPosition, float NewRotation)
{
	if (!Item || bIsLocked)
	{
		return false;
	}

	// ✅ FIX: Store original placement before removing
	FInv_ItemPlacement* OriginalPlacement = FindItemPlacement(Item);
	if (!OriginalPlacement)
	{
		return false; // Item doesn't exist in container
	}

	FIntPoint OriginalPosition = OriginalPlacement->GridPosition;
	float OriginalRotation = OriginalPlacement->Rotation;

	// Temporarily remove the item
	if (!TryRemoveItem(Item))
	{
		return false;
	}

	// Try to place it at the new position
	if (TryPlaceItem(Item, NewPosition, NewRotation))
	{
		return true;
	}
	else
	{
		// ✅ FIX: Restore item to original position on failure
		TryPlaceItem(Item, OriginalPosition, OriginalRotation);
		return false;
	}
}

TArray<FIntPoint> UInv_InventoryContainer::GetItemShapeAtPosition(const UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation) const
{
	TArray<FIntPoint> Shape;
	
	if (!Item)
	{
		return Shape;
	}

	// Get the item's default shape from its definition
	const UInv_ItemDefinition* ItemDef = Item->GetItemDefinition();
	if (!ItemDef)
	{
		// If no definition, create a 1x1 shape
		Shape.Add(FIntPoint::ZeroValue);
	}
	else
	{
		// Use the item's default shape
		Shape = ItemDef->DefaultShape;
		
		// If no shape defined, create a 1x1 shape
		if (Shape.Num() == 0)
		{
			Shape.Add(FIntPoint::ZeroValue);
		}
	}

	// Apply rotation if needed (use utility library to avoid duplication)
	if (!FMath::IsNearlyZero(Rotation))
	{
		Shape = UInv_SpatialUtilityLibrary::RotateShape(Shape, Rotation);
	}

	// Offset by the grid position
	for (FIntPoint& ShapePoint : Shape)
	{
		ShapePoint += GridPosition;
	}

	return Shape;
}

bool UInv_InventoryContainer::DoShapesOverlap(const TArray<FIntPoint>& Shape1, const TArray<FIntPoint>& Shape2) const
{
	for (const FIntPoint& Point1 : Shape1)
	{
		for (const FIntPoint& Point2 : Shape2)
		{
			if (Point1 == Point2)
			{
				return true;
			}
		}
	}
	return false;
}

bool UInv_InventoryContainer::ContainsItem(const UInv_ItemInstance* Item) const
{
	return ContainedItems.Contains(Item);
}

FInv_ItemPlacement* UInv_InventoryContainer::FindItemPlacement(const UInv_ItemInstance* Item)
{
	for (FInv_ItemPlacement& Placement : ItemPlacements)
	{
		if (Placement.ItemInstance == Item)
		{
			return &Placement;
		}
	}
	return nullptr;
}

TArray<FIntPoint> UInv_InventoryContainer::GetAvailablePositions() const
{
	TArray<FIntPoint> AvailablePositions;
	
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			FIntPoint Position(X, Y);
			if (!IsGridPositionOccupied(Position))
			{
				AvailablePositions.Add(Position);
			}
		}
	}
	
	return AvailablePositions;
}

int32 UInv_InventoryContainer::GetAvailableSlotCount() const
{
	int32 AvailableCount = 0;
	
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			if (!GridOccupancy[Y][X])
			{
				AvailableCount++;
			}
		}
	}
	
	return AvailableCount;
}

int32 UInv_InventoryContainer::GetTotalSlotCount() const
{
	return GridWidth * GridHeight;
}

float UInv_InventoryContainer::GetFillPercentage() const
{
	int32 TotalSlots = GetTotalSlotCount();
	if (TotalSlots == 0)
	{
		return 0.0f;
	}
	
	int32 OccupiedSlots = TotalSlots - GetAvailableSlotCount();
	return static_cast<float>(OccupiedSlots) / static_cast<float>(TotalSlots);
}

void UInv_InventoryContainer::SetLocked(bool bLocked)
{
	this->bIsLocked = bLocked;
}

void UInv_InventoryContainer::DebugPrintGrid() const
{
	if (!GEngine)
	{
		return;
	}

	FString GridString = FString::Printf(TEXT("Container: %s\n"), *ContainerName.ToString());
	
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		FString RowString;
		for (int32 X = 0; X < GridWidth; ++X)
		{
			RowString += GridOccupancy[Y][X] ? TEXT("X") : TEXT(".");
		}
		GridString += RowString + TEXT("\n");
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, GridString);
}

TArray<FIntPoint> UInv_InventoryContainer::GetOccupiedPositions() const
{
	TArray<FIntPoint> OccupiedPositions;
	
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			if (GridOccupancy[Y][X])
			{
				OccupiedPositions.Add(FIntPoint(X, Y));
			}
		}
	}
	
	return OccupiedPositions;
}

void UInv_InventoryContainer::UpdateGridOccupancy()
{
	// Clear the grid
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			GridOccupancy[Y][X] = false;
		}
	}

	// Mark occupied slots based on item placements
	for (const FInv_ItemPlacement& Placement : ItemPlacements)
	{
		MarkSlotsOccupied(Placement.OccupiedSlots, true);
	}
}

void UInv_InventoryContainer::MarkSlotsOccupied(const TArray<FIntPoint>& Slots, bool bOccupied)
{
	for (const FIntPoint& Slot : Slots)
	{
		if (IsGridPositionValid(Slot))
		{
			GridOccupancy[Slot.Y][Slot.X] = bOccupied;
		}
	}
}

bool UInv_InventoryContainer::ValidateItemShape(const UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation) const
{
	TArray<FIntPoint> Shape = GetItemShapeAtPosition(Item, Position, Rotation);
	
	for (const FIntPoint& ShapePoint : Shape)
	{
		if (!IsGridPositionValid(ShapePoint) || IsGridPositionOccupied(ShapePoint))
		{
			return false;
		}
	}
	
	return true;
}
