// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPF_PlugInv_DataLibrary.h"
#include "Items/Fragments/BPF_PlugInv_ItemFragmentLibrary.h"
#include "Widgets/Inventory/GridSlots/UW_PlugInv_GridSlot.h"

#include "UW_PlugInv_InventoryGrid.generated.h"

struct FPlugInv_GridFragment;
class UPlugInv_SlottedItem;
struct FPlugInv_ItemManifest;
class UPlugInv_ItemComponent;
class UPlugInv_InventoryComponent;
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
	UFUNCTION()
	virtual void NativeOnInitialized() override;

	// Binded to the inventory component delegate OnItemAdded()
	UFUNCTION()
	void AddItem(UPlugInv_InventoryItem* Item);

	// Check if the category of the inventory and the item matches.
	bool MatchesCategory(const UPlugInv_InventoryItem* Item) const;

	// Returns availability result data for this grid.
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const UPlugInv_ItemComponent* ItemComponent);
	
	// Item category of the grid.
	EPlugInv_ItemCategory GetItemCategory() const
	{
		return this->ItemCategory;
	}
	
private:
	// Function to construct the actual grid.
	void ConstructGrid();
	
	// Overloads for HasRoomForItem
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const UPlugInv_InventoryItem* InventoryItem);
	FPlugInv_SlotAvailabilityResult HasRoomForItem(const FPlugInv_ItemManifest& ItemManifest);
	
	// Add Item to multiple indices if needed
	void AddItemToIndices(const FPlugInv_SlotAvailabilityResult& Result, UPlugInv_InventoryItem* NewItem);
	
	// Add Item to single index
	void AddItemToIndex(UPlugInv_InventoryItem* NewItem, const int32 Index, bool bStackable, int32 StackAmount);
	
	// Create a slotted item widget
	UPlugInv_SlottedItem* CreateSlottedItem(UPlugInv_InventoryItem* NewItem, int32 Index, bool bStackable, int32 StackAmount, const FPlugInv_GridFragment*
	                                        GridFragment, const FPlugInv_ImageFragment* ImageFragment) const;

	// Adds the slotted item widget to the canvas panel
	void AddSlottedItemToCanvas(const int32 Index, const FPlugInv_GridFragment* GridFragment, UPlugInv_SlottedItem* SlottedItem) const;

	// Update grid slots
	void UpdateGridSlots(UPlugInv_InventoryItem* NewItem, int32 Index, bool bStackableItem, int32 StackAmount);
	
	// Draw size helper function
	FVector2D GetDrawSize(const FPlugInv_GridFragment* GridFragment) const;
	
	// Slotted image helper function
	void SetSlottedImage(const FPlugInv_GridFragment* GridFragment, const FPlugInv_ImageFragment* ImageFragment,const UPlugInv_SlottedItem* SlottedItem) const;

	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const TObjectPtr<UPlugInv_GridSlot>& GridSlot);
	bool IsInGridBounds(int32 StartIndex, const FIntPoint& ItemDimensions) const;
	bool DoesItemTypeMatch(TWeakObjectPtr<UPlugInv_InventoryItem> SubItem, const FGameplayTag& ItemTypeTag) const;
	bool CheckSlotConstraints(const FPlugInv_ItemManifest& ItemManifest, const UPlugInv_GridSlot* GridSlot, const UPlugInv_GridSlot* SubGridSlot, const
	                          TSet<int32>& CheckedIndices, TSet<int32>
	                          & OutTentativelyClaimedIndices) const;
	int32 GetSlotStackAmount(const TObjectPtr<UPlugInv_GridSlot>& GridSlot);

	UFUNCTION()
	void AddStacks(const FPlugInv_SlotAvailabilityResult& Result);
	
	// Weak ref to the inventory component.
	TWeakObjectPtr<UPlugInv_InventoryComponent> InventoryComponent;

	// Type of the items that can be stored in this particular grid.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Inventory")
	EPlugInv_ItemCategory ItemCategory;

	// Container of grid slots.
	UPROPERTY()
	TArray<TObjectPtr<UPlugInv_GridSlot>> GridSlots;

	// Type of the class of grid slot to create.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_GridSlot> GridSlotClass;

	// Slott widget class type
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_SlottedItem> SlottedItemClass;

	// Dictionary container of index and slotted item widgets
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<int32, TObjectPtr<UPlugInv_SlottedItem>> SlottedItemMap;

	// Rather, to add all the gridslots to the viewport, store them in a canvas panel instead.
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
