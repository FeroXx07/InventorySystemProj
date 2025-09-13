// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Dieg_InventorySlot.generated.h"

class UDieg_ItemInstance;
/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FDieg_InventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	TObjectPtr<UDieg_ItemInstance> ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	FGameplayTagContainer SlotTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	FIntPoint Coordinates{-1, -1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|Inventory Slot|Slot")
	FIntPoint RootSlot{-1, -1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Inventory Slot|Slot")
	float Rotation{0.0f};

	void Initialize(const FIntPoint& SlotPoint, const FGameplayTagContainer& Tags);

	bool IsOccupied() const;
	bool IsRootSlot() const { return RootSlot == Coordinates; };
	
	bool CanAcceptItem(const UDieg_ItemInstance* ItemToTest) const;
	void OnItemPlacedInSlot(UDieg_ItemInstance* PlacedItem); // May call PlacedItem->GetFragment<X>()->OnItemActivated() if slot is active
	void OnItemRemovedFromSlot(); // May call ItemInstance->GetFragment<X>()->OnItemDeactivated() if slot was active
};
