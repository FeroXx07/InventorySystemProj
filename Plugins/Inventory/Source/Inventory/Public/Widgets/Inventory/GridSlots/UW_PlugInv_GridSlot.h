// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlugInv_GridSlot.generated.h"

class UPlugInv_InventoryItem;

UENUM(BlueprintType)
enum class EPlugInv_GridSlotState : uint8
{
	Unoccupied = 0 UMETA(DisplayName = "Unoccupied"),
	Occupied UMETA(DisplayName = "Occupied"),
	Selected UMETA(DisplayName = "Selected"),
	GrayedOut UMETA(DisplayName = "GrayedOut")
};

class UImage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void SetTileIndex(const int32 Index) {TileIndex = Index;}
	int32 GetTileIndex() const {return TileIndex;}

	// Sets the state and changes brush texture
	void SetStateAndBrushTexture(const EPlugInv_GridSlotState SlotState);
	int32 GetStackCount() const;
	void SetStackCount(const int32 StackCount);
	int32 GetUpperLeftIndex() const;
	void SetUpperLeftIndex(const int32 UpperLeftIndex);
	TWeakObjectPtr<UPlugInv_InventoryItem> GetInventoryItem() const;
	void SetInventoryItem(UPlugInv_InventoryItem* InventoryItem);
	bool IsAvailable() const;
	void SetAvailable(const bool bAvailable);

	FGridSlotEvent OnGridSlotClicked;
	FGridSlotEvent OnGridSlotHovered;
	FGridSlotEvent OnGridSlotUnhovered;
private:

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 StackCount{0};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 UpperLeftIndex{INDEX_NONE};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	bool bAvailable{true};

	TWeakObjectPtr<UPlugInv_InventoryItem> InventoryItem;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TMap<EPlugInv_GridSlotState, FSlateBrush> Brushes;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	EPlugInv_GridSlotState GridSlotState;
	
};
