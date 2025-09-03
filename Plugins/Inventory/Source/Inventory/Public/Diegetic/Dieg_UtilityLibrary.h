// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dieg_UtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template<typename T> requires std::derived_from<T, UObject>
	static T* GetClassDefaults()
	{
		TSubclassOf<T> Subclass;
		T* DefaultObject = Subclass.GetDefaultObject();
		return DefaultObject;
	}

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetPositionFromIndex(const int32 Index, const int32 Columns);
	
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> GetSlotPoints(int32 TotalSlots, int32 Columns);

	// Also previously known as GetMaxValues
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetShapeSpan(const TArray<FIntPoint>& Shape);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetMinSpan(const TArray<FIntPoint>& Shape);

	template<typename T>
	static T FindMaxOfArray(const TArray<T>& Array, int32& OutIndex);

	template<typename T>
	static T FindMinOfArray(const TArray<T>& Array, int32& OutIndex);
	
	template <typename TKey, typename TValue, typename FuncT>
	static void ForEachSlotInShape(TMap<TKey, TValue>& SlotMap, const TArray<TKey>& Shape, const FuncT& Function);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FVector2D Rotate2D(const FVector2D& Coordinates, const float AngleDegrees, bool bRelativeToOrigin = true);
	
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> Rotate2DArray(const TArray<FIntPoint>& Shape, const float AngleDegrees);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> Rotate2DArrayWithRoot(const TArray<FIntPoint>& Shape, const float AngleDegrees,  const FIntPoint& RootIn, FIntPoint& RootOut);
};


template <typename T>
T UDieg_UtilityLibrary::FindMaxOfArray(const TArray<T>& Array, int32& OutIndex)
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
T UDieg_UtilityLibrary::FindMinOfArray(const TArray<T>& Array, int32& OutIndex)
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
void UDieg_UtilityLibrary::ForEachSlotInShape(TMap<TKey, TValue>& SlotMap, const TArray<TKey>& Shape,
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


