// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "UStructs/Inv_ItemPlacement.h"
#include "Inv_InventoryContainer.generated.h"

class UInv_ItemInstance;

/**
 * Represents a single inventory container with spatial grid-based item placement
 * Supports Tetris-style item shapes and placement validation
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYS_API UInv_InventoryContainer : public UObject
{
	GENERATED_BODY()

public:
	UInv_InventoryContainer();

	// Container identification and metadata
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Container")
	FName ContainerName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Container")
	FText DisplayName = FText::GetEmpty();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Container")
	FGameplayTagContainer ContainerTags;

	// Grid dimensions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Container|Grid", meta = (ClampMin = "1"))
	int32 GridWidth = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Container|Grid", meta = (ClampMin = "1"))
	int32 GridHeight = 10;

	// Container state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Container")
	bool bIsInitialized = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Container")
	bool bIsLocked = false;

	// Grid data - 2D array representing occupied slots
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Container", meta = (AllowPrivateAccess = "true"))
	TArray<TArray<bool>> GridOccupancy;

	// Items currently in this container
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Container", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInv_ItemInstance>> ContainedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Container", meta = (AllowPrivateAccess = "true"))
	TArray<FInv_ItemPlacement> ItemPlacements;

	// Initialization
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	void Initialize(const FName& InContainerName, const FText& InDisplayName, int32 InWidth, int32 InHeight, const FGameplayTagContainer& InTags = FGameplayTagContainer());

	// Grid management
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	void InitializeGrid();

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	void ClearGrid();

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool IsGridPositionValid(const FIntPoint& Position) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool IsGridPositionOccupied(const FIntPoint& Position) const;

	// Item placement and management
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool CanPlaceItemAt(UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool TryPlaceItem(UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool TryRemoveItem(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool TryMoveItem(UInv_ItemInstance* Item, const FIntPoint& NewPosition, float NewRotation = 0.0f);

	// Spatial calculations
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	TArray<FIntPoint> GetItemShapeAtPosition(const UInv_ItemInstance* Item, const FIntPoint& GridPosition, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool DoShapesOverlap(const TArray<FIntPoint>& Shape1, const TArray<FIntPoint>& Shape2) const;

	// Container queries
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool ContainsItem(const UInv_ItemInstance* Item) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	FInv_ItemPlacement* FindItemPlacement(const UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	TArray<FIntPoint> GetAvailablePositions() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	int32 GetAvailableSlotCount() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	int32 GetTotalSlotCount() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	float GetFillPercentage() const;

	// Container management
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	void SetLocked(bool bLocked);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	bool IsLocked() const { return bIsLocked; }

	// Debug and visualization
	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	void DebugPrintGrid() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Container")
	TArray<FIntPoint> GetOccupiedPositions() const;

protected:
	// Internal helper functions
	void UpdateGridOccupancy();
	void MarkSlotsOccupied(const TArray<FIntPoint>& Slots, bool bOccupied);
	bool ValidateItemShape(const UInv_ItemInstance* Item, const FIntPoint& Position, float Rotation) const;
};
