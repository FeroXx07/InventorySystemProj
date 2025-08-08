// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

#include "UW_PlugInv_HoverItem.generated.h"

class UTextBlock;
class UPlugInv_InventoryItem;
class UImage;
/**
 *  The HoverItem is the item that will appear and follow the mouse
 *  when an inventory item on the grid has been clicked.
 */
UCLASS()
class INVENTORY_API UPlugInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(int32 NewStackCount);

	FGameplayTag GetItemType() const;
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool Stackable);

	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(const int32 Value) { this->PreviousGridIndex = Value; }
	const FIntPoint& GetGridDimensions() const { return GridDimensions; }
	void SetGridDimensions(const FIntPoint& Value) { this->GridDimensions = Value; }
	UPlugInv_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UPlugInv_InventoryItem* Value);
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(const int32 Value) { this->StackCount = Value; }
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	int32 PreviousGridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<UPlugInv_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
};
