// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_UtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static APlayerController* GetOwningPlayerController(const UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Utility")
	static FIntPoint GetPositionFromIndex(const int32 Index, const int32 Columns);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Utility")
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);

	UFUNCTION(BlueprintCallable, Category = "Game|Inv|Utility")
	static TArray<FIntPoint> GetSlotPointsFromTotalColumns(int32 TotalSlots, int32 Columns);
	
	static FIntPoint GetMaxSpan(const TArray<FIntPoint>& Shape);
	static FIntPoint GetMinSpan(const TArray<FIntPoint>& Shape);
	static FIntPoint GetShapeSpan(const TArray<FIntPoint>& Shape); // GetMaxSpan() + {1,1}
	
	template<typename T>
	static T FindMaxOfArray(const TArray<T>& Array, int32& OutIndex);

	template<typename T>
	static T FindMinOfArray(const TArray<T>& Array, int32& OutIndex);
	
	template <typename TKey, typename TValue, typename FuncT>
	static void ForEachSlotInShape(TMap<TKey, TValue>& SlotMap, const TArray<TKey>& Shape, const FuncT& Function);
};

template <typename T>
T UInv_UtilityLibrary::FindMaxOfArray(const TArray<T>& Array, int32& OutIndex)
{
	static_assert(std::is_arithmetic<T>::value, "FindMaxOfArray only supports arithmetic types (int32, float, double, etc.)");

	if (Array.Num() == 0)
	{
		OutIndex = -1;
		return T(0);
	}

	const T* MaxElementIter = std::max_element(Array.GetData(), Array.GetData() + Array.Num());
	OutIndex = MaxElementIter - Array.GetData();
	return *MaxElementIter;
}

template <typename T>
T UInv_UtilityLibrary::FindMinOfArray(const TArray<T>& Array, int32& OutIndex)
{
	static_assert(std::is_arithmetic<T>::value, "FindMinOfArray only supports arithmetic types (int32, float, double, etc.)");

	if (Array.Num() == 0)
	{
		OutIndex = -1;
		return T(0);
	}

	const T* MaxElementIter = std::min_element(Array.GetData(), Array.GetData() + Array.Num());
	OutIndex = MaxElementIter - Array.GetData();
	return *MaxElementIter;
}

template <typename TKey, typename TValue, typename FuncT>
void UInv_UtilityLibrary::ForEachSlotInShape(TMap<TKey, TValue>& SlotMap, const TArray<TKey>& Shape,
	const FuncT& Function)
{
	for (const TKey& SlotPoint : Shape)
	{
		if (TValue* Slot = SlotMap.Find(SlotPoint))
		{
			Function(*Slot);
		}
	}
}
