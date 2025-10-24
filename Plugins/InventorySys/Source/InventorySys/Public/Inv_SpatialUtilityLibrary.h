// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inv_SpatialUtilityLibrary.generated.h"

class UInv_ItemInstance;
class UInv_InventoryContainer;

// Placement result structure
USTRUCT(BlueprintType)
struct INVENTORYSYS_API FPlacementResult
{
	GENERATED_BODY()

	FPlacementResult()
	{
		bCanPlace = false;
		BestPosition = FIntPoint::ZeroValue;
		BestRotation = 0.0f;
	}

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	bool bCanPlace = false;

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	FIntPoint BestPosition = FIntPoint::ZeroValue;

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	float BestRotation = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	TArray<FIntPoint> OccupiedSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	FString FailureReason = TEXT("");
};

// Placement options
USTRUCT(BlueprintType)
struct INVENTORYSYS_API FPlacementOptions
{
	GENERATED_BODY()

	FPlacementOptions()
	{
		bAllowRotation = true;
		bFindBestFit = true;
		bPreferTopLeft = true;
		RotationStep = 90.0f;
		MaxRotationSteps = 4;
	}
	
	UPROPERTY(BlueprintReadWrite, Category = "Options")
	bool bAllowRotation = true;

	UPROPERTY(BlueprintReadWrite, Category = "Options")
	bool bFindBestFit = true;

	UPROPERTY(BlueprintReadWrite, Category = "Options")
	bool bPreferTopLeft = true;

	UPROPERTY(BlueprintReadWrite, Category = "Options")
	float RotationStep = 90.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Options")
	int32 MaxRotationSteps = 4;
};

/**
 * Utility class for spatial placement operations in Tetris-style inventory systems
 * Handles collision detection, shape fitting, and placement optimization
 */
UCLASS(BlueprintType)
class INVENTORYSYS_API UInv_SpatialUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Main placement functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static FPlacementResult FindBestPlacement(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FPlacementOptions& Options = FPlacementOptions());

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static bool CanPlaceItemAt(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetValidPlacementPositions(UInv_InventoryContainer* Container, UInv_ItemInstance* Item, const FPlacementOptions& Options = FPlacementOptions());

	// Shape analysis functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetItemShapeAtPosition(UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static bool DoShapesOverlap(const TArray<FIntPoint>& Shape1, const TArray<FIntPoint>& Shape2);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static bool IsShapeWithinBounds(const TArray<FIntPoint>& Shape, int32 GridWidth, int32 GridHeight);

	// Optimization functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static FIntPoint FindTopLeftPosition(const TArray<FIntPoint>& Shape);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static int32 CalculateShapeScore(const TArray<FIntPoint>& Shape, const FIntPoint& Position, bool bPreferTopLeft = true);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetShapeRotations(UInv_ItemInstance* Item, float RotationStep = 90.0f, int32 MaxSteps = 4);

	// Grid analysis functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetAvailablePositions(UInv_InventoryContainer* Container);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetOccupiedPositions(UInv_InventoryContainer* Container);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static bool IsPositionValid(UInv_InventoryContainer* Container, const FIntPoint& Position);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static bool IsPositionOccupied(UInv_InventoryContainer* Container, const FIntPoint& Position);

	// Container analysis functions
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static int32 GetContainerFillPercentage(UInv_InventoryContainer* Container);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static TArray<FIntPoint> GetEmptyRegions(UInv_InventoryContainer* Container, int32 MinSize = 1);

	// Debug and visualization
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static void DebugPrintGrid(UInv_InventoryContainer* Container);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|SpatialPlacement")
	static void DebugPrintShape(const TArray<FIntPoint>& Shape, const FString& Label = TEXT(""));

protected:
	// Internal helper functions
	static TArray<FIntPoint> RotateShape(const TArray<FIntPoint>& OriginalShape, float Rotation);
	static bool ValidateShape(const TArray<FIntPoint>& Shape);
	static FIntPoint CalculateShapeCenter(const TArray<FIntPoint>& Shape);
	static bool IsShapeConnected(const TArray<FIntPoint>& Shape);
};

