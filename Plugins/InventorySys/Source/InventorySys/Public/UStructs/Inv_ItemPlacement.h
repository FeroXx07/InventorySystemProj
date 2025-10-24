// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Inv_ItemPlacement.generated.h"

class UInv_ItemInstance;

/**
 * Represents the spatial placement data for an item in an inventory container
 * Contains position, rotation, and occupied slots information
 */
USTRUCT(BlueprintType)
struct INVENTORYSYS_API FInv_ItemPlacement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	TObjectPtr<UInv_ItemInstance> ItemInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	FIntPoint GridPosition = FIntPoint::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float Rotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	TArray<FIntPoint> OccupiedSlots;

	FInv_ItemPlacement()
	{
		ItemInstance = nullptr;
		GridPosition = FIntPoint::ZeroValue;
		Rotation = 0.0f;
	}

	FInv_ItemPlacement(UInv_ItemInstance* InItemInstance, const FIntPoint& InGridPosition, float InRotation, const TArray<FIntPoint>& InOccupiedSlots)
		: ItemInstance(InItemInstance)
		, GridPosition(InGridPosition)
		, Rotation(InRotation)
		, OccupiedSlots(InOccupiedSlots)
	{
	}

	// Utility functions
	bool IsValid() const
	{
		return ItemInstance != nullptr && OccupiedSlots.Num() > 0;
	}

	void Clear()
	{
		ItemInstance = nullptr;
		GridPosition = FIntPoint::ZeroValue;
		Rotation = 0.0f;
		OccupiedSlots.Empty();
	}

	bool ContainsPosition(const FIntPoint& Position) const
	{
		return OccupiedSlots.Contains(Position);
	}

	FIntPoint GetCenterPosition() const
	{
		if (OccupiedSlots.Num() == 0)
		{
			return GridPosition;
		}

		FIntPoint Sum = FIntPoint::ZeroValue;
		for (const FIntPoint& Slot : OccupiedSlots)
		{
			Sum += Slot;
		}
		return FIntPoint(Sum.X / OccupiedSlots.Num(), Sum.Y / OccupiedSlots.Num());
	}
};