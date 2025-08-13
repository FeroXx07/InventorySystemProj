// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/UW_PlugInv_InventoryBase.h"
#include "UW_PlugInv_InventorySpatial.generated.h"

class UPlugInv_EquippedSlottedItem;
struct FGameplayTag;
class UPlugInv_EquippedGridSlot;
class UPlugInv_ItemDescription;
class UCanvasPanel;
class UWidgetSwitcher;
class UPlugInv_InventoryGrid;
class UButton;

/**
 *  The base widget class that manages all possible grid widgets.
 */
UCLASS()
class INVENTORY_API UPlugInv_InventorySpatial : public UPlugInv_InventoryBase
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FPlugInv_SlotAvailabilityResult HasRoomForItem(TObjectPtr<UPlugInv_ItemComponent> ItemComponent) const override;
	virtual void OnItemHovered(UPlugInv_InventoryItem* Item) override;
	virtual void OnItemUnHovered() override;
	virtual bool HasHoverItem() const override;
	virtual UPlugInv_HoverItem* GetHoverItem() const override;
	virtual float GetEquippablesTileSize() const override;
private:
	// By marking a pointer to a widget as BindWidget, you can create an identically-named widget in a Blueprint subclass of your C++ class, and at run-time access it from the C++.
	// https://unreal-garden.com/tutorials/ui-bindwidget/
	// These can be a List<UPlugInv_InventoryGrid> but for simplicity for now lets hold vars like this.

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;

	UPROPERTY(meta=(BindWidget))
	TArray<TObjectPtr<UPlugInv_EquippedGridSlot>> EquippedGridSlots;
	
	// Reference to an inventory grid widget.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlugInv_InventoryGrid> Grid_Equippables;

	// Reference to an inventory grid widget.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlugInv_InventoryGrid> Grid_Consumables;

	// Reference to an inventory grid widget.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlugInv_InventoryGrid> Grid_Craftables;

	// Widget to switch to active inventory grid.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<UPlugInv_InventoryGrid> ActiveGrid;

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_ItemDescription> ItemDescriptionClass;
	
	UPROPERTY()
	TObjectPtr<UPlugInv_ItemDescription> ItemDescription;
	
	FTimerHandle DescriptionTimer;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DescriptionTimerDelay = 0.5f;
	
	UPlugInv_ItemDescription* GetItemDescription();

	// Function to bind to buttons.
	UFUNCTION()
	void ShowEquippables();

	// Function to bind to buttons.
	UFUNCTION()
	void ShowConsumables();

	// Function to bind to buttons.
	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UPlugInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void EquippedSlottedItemClicked(UPlugInv_EquippedSlottedItem* EquippedSlottedItem);

	// Disable active inventory grid button.
	void DisableButton(const TObjectPtr<UButton>& Button) const;

	// Function to set the active inventory grid.
	void SetActiveGrid(const TObjectPtr<UPlugInv_InventoryGrid>& Grid, const TObjectPtr<UButton>& Button);

	void SetItemDescriptionSizeAndPosition(UPlugInv_ItemDescription* Description, UCanvasPanel* Canvas) const;

	bool CanEquipHoverItem(const UPlugInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag) const;

	UPlugInv_EquippedGridSlot* FindSlotWithEquippedItem(UPlugInv_InventoryItem* EquippedItem) const;

	void ClearSlotOfItem(UPlugInv_EquippedGridSlot* EquippedGridSlot);
	
	void RemoveEquippedSlottedItem(UPlugInv_EquippedSlottedItem* EquippedSlottedItem);
	
	void MakeEquippedSlottedItem(const UPlugInv_EquippedSlottedItem* EquippedSlottedItem, UPlugInv_EquippedGridSlot* EquippedGridSlot, UPlugInv_InventoryItem* ItemToEquip);

	void BroadcastSlotClickedDelegates(UPlugInv_InventoryItem* ItemToEquip, UPlugInv_InventoryItem* ItemToUnequip) const;
};
