// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"
#include "BPF_PlugInv_InventoryStatics.generated.h"

class UPlugInv_InventoryBase;
class UPlugInv_HoverItem;
class UPlugInv_InventoryItem;
enum class EPlugInv_ItemCategory : uint8;
class UPlugInv_InventoryComponent;
class UPlugInv_ItemComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UPlugInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EPlugInv_ItemCategory GetItemCategoryFromItemComponent(const UPlugInv_ItemComponent* ItemComponent);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemHovered(APlayerController* PC, UPlugInv_InventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static void ItemUnhovered(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UPlugInv_HoverItem* GetHoverItem(APlayerController* PC);

	static UPlugInv_InventoryBase* GetInventoryWidget(const APlayerController* PC);
};

template <typename T, typename FuncT>
void UPlugInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns,
	const FuncT& Function)
{
	for (int32 j = 0; j < Range2D.Y; ++j)
	{
		for (int32 i = 0; i < Range2D.X; ++i)
		{
			const FIntPoint Coordinates = UPlugInv_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(i, j);
			const int32 TileIndexIteration = UPlugInv_WidgetUtils::GetIndexFromPosition(Coordinates, GridColumns);
			if (Array.IsValidIndex(TileIndexIteration))
			{
				Function(Array[TileIndexIteration]);
			}
		}
	}
}