// Fill out your copyright notice in the Description page of Project Settings.


#include "Inv_UtilityLibrary.h"

#include "Kismet/GameplayStatics.h"

APlayerController* UInv_UtilityLibrary::GetOwningPlayerController(const UObject* Object)
{
	return UGameplayStatics::GetPlayerController(Object->GetWorld(), 0);
}

FIntPoint UInv_UtilityLibrary::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

int32 UInv_UtilityLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

TArray<FIntPoint> UInv_UtilityLibrary::GetSlotPointsFromTotalColumns(int32 TotalSlots, int32 Columns)
{
	TArray<FIntPoint> Coordinates;
	Coordinates.Reserve(TotalSlots); // Preallocate memory

	for (int32 Index = 0; Index < TotalSlots; ++Index)
	{
		const int32 InX = Index % Columns;       // Column index
		const int32 InY = Index / Columns;       // Row index
		Coordinates.Add(FIntPoint(InX, InY));
	}

	return Coordinates;
}

FIntPoint UInv_UtilityLibrary::GetMaxSpan(const TArray<FIntPoint>& Shape)
{
	TArray<int32> X, Y;
	int32 SpanX = 0, SpanY = 0;
	int32 IndexX = -1; int32 IndexY = -1;
	
	for (const FIntPoint& Element : Shape)
	{
		X.Add(Element.X);
		Y.Add(Element.Y);
	}
	
	SpanX = FindMaxOfArray(X, IndexX);
	SpanY = FindMaxOfArray(Y, IndexY);

	return FIntPoint(SpanX, SpanY);
}

FIntPoint UInv_UtilityLibrary::GetMinSpan(const TArray<FIntPoint>& Shape)
{
	TArray<int32> X, Y;
	int32 SpanX = 0, SpanY = 0;
	int32 IndexX = -1; int32 IndexY = -1;
	
	for (const FIntPoint& Element : Shape)
    {
    	X.Add(Element.X);
    	Y.Add(Element.Y);
    }
	
    SpanX = FindMinOfArray(X, IndexX);
    SpanY = FindMinOfArray(Y, IndexY);
	
    return FIntPoint(SpanX, SpanY);
}

FIntPoint UInv_UtilityLibrary::GetShapeSpan(const TArray<FIntPoint>& Shape)
{
	// 2x2 Size block: Coordinates span from 0..1, its max value would say 1 instead of 2, so sum 1 unit on both axis.
	return GetMaxSpan(Shape) + FIntPoint(1, 1);
}

