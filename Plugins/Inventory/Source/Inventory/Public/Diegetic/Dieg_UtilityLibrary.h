// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPF_PlugInv_DoubleLogger.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dieg_UtilityLibrary.generated.h"

class ADieg_PlayerController;

/**
 * @brief Utility function library for the diegetic inventory system.
 * 
 * UDieg_UtilityLibrary provides static utility functions for common operations
 * used throughout the diegetic inventory system. This includes mathematical
 * operations, coordinate transformations, component caching, and array utilities.
 * 
 * @note This is a Blueprint function library and most functions are accessible
 * from Blueprint graphs. Template functions are C++ only.
 * 
 * @see UBlueprintFunctionLibrary
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_UtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Gets the owning player controller for the given object.
	 * 
	 * @param Object The object to get the player controller for
	 * @return The owning player controller, or nullptr if not found
	 * 
	 * @see ADieg_PlayerController
	 */
	static ADieg_PlayerController* GetOwningPlayerController(const UObject* Object);

	/**
	 * @brief Caches a component reference for performance optimization.
	 * 
	 * This template function finds and caches a component of the specified type
	 * on the given actor. The cached reference is stored in the provided pointer.
	 * 
	 * @tparam T The type of component to find
	 * @param Owner The actor to search for the component
	 * @param CachedPtr Reference to store the cached component pointer
	 * @return The found component, or nullptr if not found
	 * 
	 * @see CacheComponentChecked
	 */
	template <typename T>
	static T* CacheComponent(AActor* Owner, TObjectPtr<T>& CachedPtr);

	/**
	 * @brief Caches a component reference with error checking.
	 * 
	 * Similar to CacheComponent but logs a warning if the component is not found.
	 * 
	 * @tparam T The type of component to find
	 * @param Owner The actor to search for the component
	 * @param CachedPtr Reference to store the cached component pointer
	 * @return The found component, or nullptr if not found
	 * 
	 * @see CacheComponent
	 */
	template <typename T>
	static T* CacheComponentChecked(AActor* Owner, TObjectPtr<T>& CachedPtr);

	/**
	 * @brief Converts a 1D index to 2D grid coordinates.
	 * 
	 * @param Index The 1D index to convert
	 * @param Columns The number of columns in the grid
	 * @return The 2D coordinates (X, Y)
	 * 
	 * @see GetIndexFromPosition
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetPositionFromIndex(const int32 Index, const int32 Columns);
	
	/**
	 * @brief Converts 2D grid coordinates to a 1D index.
	 * 
	 * @param Position The 2D coordinates to convert
	 * @param Columns The number of columns in the grid
	 * @return The 1D index
	 * 
	 * @see GetPositionFromIndex
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);

	/**
	 * @brief Generates an array of grid coordinates for the specified grid.
	 * 
	 * @param TotalSlots Total number of slots in the grid
	 * @param Columns Number of columns in the grid
	 * @return Array of FIntPoint coordinates for all slots
	 * 
	 * @see FIntPoint
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> GetSlotPoints(int32 TotalSlots, int32 Columns);

	/**
	 * @brief Calculates the span (width and height) of a shape.
	 * 
	 * @param Shape Array of coordinates defining the shape
	 * @return The span as FIntPoint (width, height)
	 * 
	 * @see GetMinSpan
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetShapeSpan(const TArray<FIntPoint>& Shape);

	/**
	 * @brief Calculates the minimum span of a shape.
	 * 
	 * @param Shape Array of coordinates defining the shape
	 * @return The minimum span as FIntPoint
	 * 
	 * @see GetShapeSpan
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetMinSpan(const TArray<FIntPoint>& Shape);

	/**
	 * @brief Finds the maximum value in an array.
	 * 
	 * @tparam T Arithmetic type (int32, float, double, etc.)
	 * @param Array The array to search
	 * @param OutIndex [Out] Index of the maximum value
	 * @return The maximum value found
	 * 
	 * @see FindMinOfArray
	 */
	template<typename T>
	static T FindMaxOfArray(const TArray<T>& Array, int32& OutIndex);

	/**
	 * @brief Finds the minimum value in an array.
	 * 
	 * @tparam T Arithmetic type (int32, float, double, etc.)
	 * @param Array The array to search
	 * @param OutIndex [Out] Index of the minimum value
	 * @return The minimum value found
	 * 
	 * @see FindMaxOfArray
	 */
	template<typename T>
	static T FindMinOfArray(const TArray<T>& Array, int32& OutIndex);
	
	/**
	 * @brief Applies a function to each slot in a shape.
	 * 
	 * @tparam TKey Key type for the slot map
	 * @tparam TValue Value type for the slot map
	 * @tparam FuncT Function type to apply
	 * @param SlotMap The map of slots to iterate over
	 * @param Shape Array of coordinates defining the shape
	 * @param Function The function to apply to each slot
	 * 
	 * @see TMap
	 */
	template <typename TKey, typename TValue, typename FuncT>
	static void ForEachSlotInShape(TMap<TKey, TValue>& SlotMap, const TArray<TKey>& Shape, const FuncT& Function);

	/**
	 * @brief Rotates 2D coordinates by the specified angle.
	 * 
	 * @param Coordinates The coordinates to rotate
	 * @param AngleDegrees The rotation angle in degrees
	 * @param bRelativeToOrigin Whether to rotate relative to origin (true) or center (false)
	 * @return The rotated coordinates
	 * 
	 * @see Rotate2DArray
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FVector2D Rotate2D(const FVector2D& Coordinates, const float AngleDegrees, bool bRelativeToOrigin = true);
	
	/**
	 * @brief Rotates an array of 2D coordinates by the specified angle.
	 * 
	 * @param Shape Array of coordinates to rotate
	 * @param AngleDegrees The rotation angle in degrees
	 * @return Array of rotated coordinates
	 * 
	 * @see Rotate2D
	 * @see Rotate2DArrayWithRoot
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> Rotate2DArray(const TArray<FIntPoint>& Shape, const float AngleDegrees);

	/**
	 * @brief Rotates an array of 2D coordinates with root point handling.
	 * 
	 * @param Shape Array of coordinates to rotate
	 * @param AngleDegrees The rotation angle in degrees
	 * @param RootIn Input root point
	 * @param RootOut [Out] Output root point after rotation
	 * @return Array of rotated coordinates
	 * 
	 * @see Rotate2DArray
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static TArray<FIntPoint> Rotate2DArrayWithRoot(const TArray<FIntPoint>& Shape, const float AngleDegrees,  const FIntPoint& RootIn, FIntPoint& RootOut);

	/**
	 * @brief Calculates offset based on rotation angle.
	 * 
	 * @param AngleDegrees The rotation angle in degrees
	 * @return The calculated offset
	 * 
	 * @see Rotate2D
	 */
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory Utility Statics")
	static FIntPoint GetOffsetBasedOnRotation(float AngleDegrees);
};


template <typename T>
T* UDieg_UtilityLibrary::CacheComponent(AActor* Owner, TObjectPtr<T>& CachedPtr)
{
	if (!CachedPtr && Owner)
	{
		CachedPtr = Owner->FindComponentByClass<T>();
	}
	return CachedPtr.Get();
}

template <typename T>
T* UDieg_UtilityLibrary::CacheComponentChecked(AActor* Owner, TObjectPtr<T>& CachedPtr)
{
	if (!CachedPtr && Owner)
	{
		CachedPtr = Owner->FindComponentByClass<T>();
		if (!CachedPtr)
		{
			UPlugInv_DoubleLogger::LogWarning(10.0f, TEXT("CacheComponentChecked: Could not find component on {0}"), Owner->GetName());
		}
	}
	return CachedPtr.Get();
}

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


