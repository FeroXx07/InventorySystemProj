// Fill out your copyright notice in the Description page of Project Settings.

#include "Inv_SpatialUtilityLibrary.h"
#include "UObjects/Inv_InventoryContainer.h"
#include "UObjects/Inv_ItemInstance.h"
#include "UObjects/Inv_ItemDefinition.h"
#include "Engine/Engine.h"

UInv_SpatialUtilityLibrary::FPlacementResult UInv_SpatialUtilityLibrary::FindBestPlacement(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FPlacementOptions& Options)
{
	FPlacementResult Result;
	
	if (!Container || !Item)
	{
		Result.FailureReason = TEXT("Invalid container or item");
		return Result;
	}

	// Get all possible rotations
	TArray<float> Rotations;
	if (Options.bAllowRotation)
	{
		for (int32 i = 0; i < Options.MaxRotationSteps; ++i)
		{
			Rotations.Add(i * Options.RotationStep);
		}
	}
	else
	{
		Rotations.Add(0.0f);
	}

	// Get all available positions
	TArray<FIntPoint> AvailablePositions = GetAvailablePositions(Container);
	
	int32 BestScore = -1;
	FIntPoint BestPosition = FIntPoint::ZeroValue;
	float BestRotation = 0.0f;
	TArray<FIntPoint> BestShape;

	// Try each rotation
	for (float Rotation : Rotations)
	{
		// Get the shape at this rotation
		TArray<FIntPoint> Shape = GetItemShapeAtPosition(Item, FIntPoint::ZeroValue, Rotation);
		
		// Try each available position
		for (const FIntPoint& Position : AvailablePositions)
		{
			// Check if the shape fits at this position
			TArray<FIntPoint> ShapeAtPosition = GetItemShapeAtPosition(Item, Position, Rotation);
			
			if (IsShapeWithinBounds(ShapeAtPosition, Container->GridWidth, Container->GridHeight))
			{
				// Check for collisions
				bool bHasCollision = false;
				for (const FIntPoint& ShapePoint : ShapeAtPosition)
				{
					if (IsPositionOccupied(Container, ShapePoint))
					{
						bHasCollision = true;
						break;
					}
				}
				
				if (!bHasCollision)
				{
					// Calculate score for this placement
					int32 Score = CalculateShapeScore(ShapeAtPosition, Position, Options.bPreferTopLeft);
					
					if (Score > BestScore)
					{
						BestScore = Score;
						BestPosition = Position;
						BestRotation = Rotation;
						BestShape = ShapeAtPosition;
					}
				}
			}
		}
	}

	// Set result
	if (BestScore >= 0)
	{
		Result.bCanPlace = true;
		Result.BestPosition = BestPosition;
		Result.BestRotation = BestRotation;
		Result.OccupiedSlots = BestShape;
	}
	else
	{
		Result.FailureReason = TEXT("No valid placement found");
	}

	return Result;
}

bool UInv_SpatialUtilityLibrary::CanPlaceItemAt(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation)
{
	if (!Container || !Item)
	{
		return false;
	}

	TArray<FIntPoint> Shape = GetItemShapeAtPosition(Item, Position, Rotation);
	
	// Check if shape is within bounds
	if (!IsShapeWithinBounds(Shape, Container->GridWidth, Container->GridHeight))
	{
		return false;
	}

	// Check for collisions
	for (const FIntPoint& ShapePoint : Shape)
	{
		if (IsPositionOccupied(Container, ShapePoint))
		{
			return false;
		}
	}

	return true;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetValidPlacementPositions(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FPlacementOptions& Options)
{
	TArray<FIntPoint> ValidPositions;
	
	if (!Container || !Item)
	{
		return ValidPositions;
	}

	// Get all possible rotations
	TArray<float> Rotations;
	if (Options.bAllowRotation)
	{
		for (int32 i = 0; i < Options.MaxRotationSteps; ++i)
		{
			Rotations.Add(i * Options.RotationStep);
		}
	}
	else
	{
		Rotations.Add(0.0f);
	}

	// Get all available positions
	TArray<FIntPoint> AvailablePositions = GetAvailablePositions(Container);

	// Try each rotation and position combination
	for (float Rotation : Rotations)
	{
		for (const FIntPoint& Position : AvailablePositions)
		{
			if (CanPlaceItemAt(Container, Item, Position, Rotation))
			{
				ValidPositions.AddUnique(Position);
			}
		}
	}

	return ValidPositions;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetItemShapeAtPosition(UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation)
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

	// Apply rotation if needed
	if (!FMath::IsNearlyZero(Rotation))
	{
		Shape = RotateShape(Shape, Rotation);
	}

	// Offset by the position
	for (FIntPoint& ShapePoint : Shape)
	{
		ShapePoint += Position;
	}

	return Shape;
}

bool UInv_SpatialUtilityLibrary::DoShapesOverlap(const TArray<FIntPoint>& Shape1, const TArray<FIntPoint>& Shape2)
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

bool UInv_SpatialUtilityLibrary::IsShapeWithinBounds(const TArray<FIntPoint>& Shape, int32 GridWidth, int32 GridHeight)
{
	for (const FIntPoint& Point : Shape)
	{
		if (Point.X < 0 || Point.X >= GridWidth || Point.Y < 0 || Point.Y >= GridHeight)
		{
			return false;
		}
	}
	return true;
}

FIntPoint UInv_SpatialUtilityLibrary::FindTopLeftPosition(const TArray<FIntPoint>& Shape)
{
	if (Shape.Num() == 0)
	{
		return FIntPoint::ZeroValue;
	}

	FIntPoint TopLeft = Shape[0];
	for (const FIntPoint& Point : Shape)
	{
		if (Point.Y < TopLeft.Y || (Point.Y == TopLeft.Y && Point.X < TopLeft.X))
		{
			TopLeft = Point;
		}
	}
	return TopLeft;
}

int32 UInv_SpatialUtilityLibrary::CalculateShapeScore(const TArray<FIntPoint>& Shape, const FIntPoint& Position, bool bPreferTopLeft)
{
	int32 Score = 0;
	
	// Base score from position (prefer top-left)
	if (bPreferTopLeft)
	{
		Score += (1000 - Position.Y * 100 - Position.X);
	}
	else
	{
		Score += (1000 - Position.Y * 10 - Position.X);
	}
	
	// Bonus for compact shapes
	Score += Shape.Num() * 10;
	
	return Score;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetShapeRotations(UInv_ItemInstance* Item, float RotationStep, int32 MaxSteps)
{
	TArray<FIntPoint> Rotations;
	
	if (!Item)
	{
		return Rotations;
	}

	for (int32 i = 0; i < MaxSteps; ++i)
	{
		float Rotation = i * RotationStep;
		TArray<FIntPoint> Shape = GetItemShapeAtPosition(Item, FIntPoint::ZeroValue, Rotation);
		Rotations.Append(Shape);
	}
	
	return Rotations;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetAvailablePositions(UInv_InventoryContainer* Container)
{
	TArray<FIntPoint> AvailablePositions;
	
	if (!Container)
	{
		return AvailablePositions;
	}

	for (int32 Y = 0; Y < Container->GridHeight; ++Y)
	{
		for (int32 X = 0; X < Container->GridWidth; ++X)
		{
			FIntPoint Position(X, Y);
			if (!IsPositionOccupied(Container, Position))
			{
				AvailablePositions.Add(Position);
			}
		}
	}
	
	return AvailablePositions;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetOccupiedPositions(UInv_InventoryContainer* Container)
{
	TArray<FIntPoint> OccupiedPositions;
	
	if (!Container)
	{
		return OccupiedPositions;
	}

	for (int32 Y = 0; Y < Container->GridHeight; ++Y)
	{
		for (int32 X = 0; X < Container->GridWidth; ++X)
		{
			FIntPoint Position(X, Y);
			if (IsPositionOccupied(Container, Position))
			{
				OccupiedPositions.Add(Position);
			}
		}
	}
	
	return OccupiedPositions;
}

bool UInv_SpatialUtilityLibrary::IsPositionValid(UInv_InventoryContainer* Container, const FIntPoint& Position)
{
	if (!Container)
	{
		return false;
	}
	
	return Position.X >= 0 && Position.X < Container->GridWidth && 
		   Position.Y >= 0 && Position.Y < Container->GridHeight;
}

bool UInv_SpatialUtilityLibrary::IsPositionOccupied(UInv_InventoryContainer* Container, const FIntPoint& Position)
{
	if (!Container || !IsPositionValid(Container, Position))
	{
		return true; // Invalid positions are considered occupied
	}
	
	return Container->IsGridPositionOccupied(Position);
}

int32 UInv_SpatialUtilityLibrary::GetContainerFillPercentage(UInv_InventoryContainer* Container)
{
	if (!Container)
	{
		return 0;
	}

	int32 TotalSlots = Container->GetTotalSlotCount();
	int32 AvailableSlots = Container->GetAvailableSlotCount();
	int32 OccupiedSlots = TotalSlots - AvailableSlots;
	
	return TotalSlots > 0 ? (OccupiedSlots * 100) / TotalSlots : 0;
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::GetEmptyRegions(UInv_InventoryContainer* Container, int32 MinSize)
{
	TArray<FIntPoint> EmptyRegions;
	
	if (!Container)
	{
		return EmptyRegions;
	}

	// Find contiguous empty regions of at least MinSize
	for (int32 Y = 0; Y < Container->GridHeight; ++Y)
	{
		for (int32 X = 0; X < Container->GridWidth; ++X)
		{
			FIntPoint StartPos(X, Y);
			if (!IsPositionOccupied(Container, StartPos))
			{
				// Check if this position starts a region of at least MinSize
				int32 RegionSize = 0;
				TArray<FIntPoint> CurrentRegion;
				
				// Simple flood fill to find connected empty area
				TArray<FIntPoint> ToCheck;
				ToCheck.Add(StartPos);
				
				while (ToCheck.Num() > 0 && RegionSize < MinSize * 2) // Limit search for performance
				{
					FIntPoint Current = ToCheck.Pop();
					
					if (!IsPositionOccupied(Container, Current) && !CurrentRegion.Contains(Current))
					{
						CurrentRegion.Add(Current);
						RegionSize++;
						
						// Add adjacent positions to check
						TArray<FIntPoint> Adjacent = {
							FIntPoint(Current.X + 1, Current.Y),
							FIntPoint(Current.X - 1, Current.Y),
							FIntPoint(Current.X, Current.Y + 1),
							FIntPoint(Current.X, Current.Y - 1)
						};
						
						for (const FIntPoint& Adj : Adjacent)
						{
							if (IsPositionValid(Container, Adj) && !CurrentRegion.Contains(Adj))
							{
								ToCheck.Add(Adj);
							}
						}
					}
				}
				
				if (RegionSize >= MinSize)
				{
					EmptyRegions.Add(StartPos);
				}
			}
		}
	}
	
	return EmptyRegions;
}

void UInv_SpatialUtilityLibrary::DebugPrintGrid(UInv_InventoryContainer* Container)
{
	if (!Container || !GEngine)
	{
		return;
	}

	FString GridString = FString::Printf(TEXT("Container: %s\n"), *Container->ContainerName.ToString());
	
	for (int32 Y = 0; Y < Container->GridHeight; ++Y)
	{
		FString RowString;
		for (int32 X = 0; X < Container->GridWidth; ++X)
		{
			RowString += IsPositionOccupied(Container, FIntPoint(X, Y)) ? TEXT("X") : TEXT(".");
		}
		GridString += RowString + TEXT("\n");
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, GridString);
}

void UInv_SpatialUtilityLibrary::DebugPrintShape(const TArray<FIntPoint>& Shape, const FString& Label)
{
	if (!GEngine)
	{
		return;
	}

	FString ShapeString = Label + TEXT(" Shape: ");
	for (const FIntPoint& Point : Shape)
	{
		ShapeString += FString::Printf(TEXT("(%d,%d) "), Point.X, Point.Y);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, ShapeString);
}

TArray<FIntPoint> UInv_SpatialUtilityLibrary::RotateShape(const TArray<FIntPoint>& OriginalShape, float Rotation)
{
	TArray<FIntPoint> RotatedShape = OriginalShape;
	
	// Normalize rotation to 0, 90, 180, 270 degrees
	int32 RotationSteps = FMath::RoundToInt(Rotation / 90.0f) % 4;
	if (RotationSteps < 0) RotationSteps += 4;
	
	// Apply rotation steps
	for (int32 Step = 0; Step < RotationSteps; ++Step)
	{
		for (FIntPoint& Point : RotatedShape)
		{
			// 90-degree clockwise rotation: (x, y) -> (-y, x)
			int32 NewX = -Point.Y;
			int32 NewY = Point.X;
			Point.X = NewX;
			Point.Y = NewY;
		}
	}
	
	// Normalize shape to start from (0, 0) - no negative coordinates
	if (RotatedShape.Num() > 0)
	{
		int32 MinX = RotatedShape[0].X;
		int32 MinY = RotatedShape[0].Y;
		
		for (const FIntPoint& Point : RotatedShape)
		{
			MinX = FMath::Min(MinX, Point.X);
			MinY = FMath::Min(MinY, Point.Y);
		}
		
		// Offset all points to make minimum (0, 0)
		for (FIntPoint& Point : RotatedShape)
		{
			Point.X -= MinX;
			Point.Y -= MinY;
		}
	}
	
	return RotatedShape;
}

bool UInv_SpatialUtilityLibrary::ValidateShape(const TArray<FIntPoint>& Shape)
{
	// Check if shape has at least one point
	if (Shape.Num() == 0)
	{
		return false;
	}
	
	// Check if all points are valid (non-negative coordinates)
	for (const FIntPoint& Point : Shape)
	{
		if (Point.X < 0 || Point.Y < 0)
		{
			return false;
		}
	}
	
	return true;
}

FIntPoint UInv_SpatialUtilityLibrary::CalculateShapeCenter(const TArray<FIntPoint>& Shape)
{
	if (Shape.Num() == 0)
	{
		return FIntPoint::ZeroValue;
	}
	
	int32 SumX = 0, SumY = 0;
	for (const FIntPoint& Point : Shape)
	{
		SumX += Point.X;
		SumY += Point.Y;
	}
	
	return FIntPoint(SumX / Shape.Num(), SumY / Shape.Num());
}

bool UInv_SpatialUtilityLibrary::IsShapeConnected(const TArray<FIntPoint>& Shape)
{
	if (Shape.Num() <= 1)
	{
		return true;
	}
	
	// Simple connectivity check - all points should be adjacent to at least one other point
	for (const FIntPoint& Point : Shape)
	{
		bool bHasNeighbor = false;
		for (const FIntPoint& OtherPoint : Shape)
		{
			if (Point != OtherPoint)
			{
				int32 DeltaX = FMath::Abs(Point.X - OtherPoint.X);
				int32 DeltaY = FMath::Abs(Point.Y - OtherPoint.Y);
				if ((DeltaX == 1 && DeltaY == 0) || (DeltaX == 0 && DeltaY == 1))
				{
					bHasNeighbor = true;
					break;
				}
			}
		}
		if (!bHasNeighbor)
		{
			return false;
		}
	}
	
	return true;
}
