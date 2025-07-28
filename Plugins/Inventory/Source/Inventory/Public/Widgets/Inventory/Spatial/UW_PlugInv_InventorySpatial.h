// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/UW_PlugInv_InventoryBase.h"
#include "UW_PlugInv_InventorySpatial.generated.h"

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

	virtual FPlugInv_SlotAvailabilityResult HasRoomForItem(TObjectPtr<UPlugInv_ItemComponent> ItemComponent) const override;
	
private:
	// By marking a pointer to a widget as BindWidget, you can create an identically-named widget in a Blueprint subclass of your C++ class, and at run-time access it from the C++.
	// https://unreal-garden.com/tutorials/ui-bindwidget/
	// These can be a List<UPlugInv_InventoryGrid> but for simplicity for now lets hold vars like this.

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

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	// Button widgets to set active inventory grids.
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	// Function to bind to buttons.
	UFUNCTION()
	void ShowEquippables();

	// Function to bind to buttons.
	UFUNCTION()
	void ShowConsumables();

	// Function to bind to buttons.
	UFUNCTION()
	void ShowCraftables();

	// Disable active inventory grid button.
	void DisableButton(const TObjectPtr<UButton>& Button) const;

	// Function to set the active inventory grid.
	void SetActiveGrid(const TObjectPtr<UPlugInv_InventoryGrid>& Grid, const TObjectPtr<UButton>& Button) const;
};
