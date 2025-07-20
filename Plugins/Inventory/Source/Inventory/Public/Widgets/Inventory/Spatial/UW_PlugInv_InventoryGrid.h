// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPF_PlugInv_DataLibrary.h"
#include "Widgets/Inventory/GridSlots/UW_PlugInv_GridSlot.h"

#include "UW_PlugInv_InventoryGrid.generated.h"

class UCanvasPanel;
class UPlugInv_GridSlot;

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	EPlugInv_ItemCategory GetItemCategory() const {return this->ItemCategory;}

private:

	// Function to construct the actual grid.
	void ConstructGrid();

	// Type of the items that can be stored in this particular grid.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlugInv_ItemCategory ItemCategory;

	// Container of grid slots.
	UPROPERTY()
	TArray<TObjectPtr<UPlugInv_GridSlot>> GridSlots;

	// Type of the class of grid slot to create.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_GridSlot> GridSlotClass;

	// Rather to add all the gridslots to the viewport, store them in a canvas panel instead.
	// Panel size changes also affect the grid slots.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	// Rows of the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	// Columns of the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;
	
	// Size of the tiles in the grid.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 TileSize;
};
