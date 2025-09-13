// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Diegetic/Dieg_DataLibrary.h"
#include "Dieg_Slot.generated.h"

class UCanvasPanel;
enum class EDieg_SlotStatus : uint8;
class UDieg_Grid;
class UImage;
class USizeBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHoverSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnHoverSlot, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPressedSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReleasedSlot, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);

/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_Slot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Diegetic Inventory")
	void InitializeDefaults();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Diegetic Inventory")
	FIntPoint GetCoordinatesInGrid();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Diegetic Inventory")
	void SetStatusAndColor(EDieg_SlotStatus Status, bool bChangeAppearance = true);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory")
	bool IsNotLocked() const { return SlotStatus != EDieg_SlotStatus::Locked; }

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory")
	bool IsAppearanceLocked() const { return bIsLockedAppearance; }

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory")
	void SetAppearanceLocked(bool Value) { bIsLockedAppearance = Value; }

	void SetParentGrid(const TObjectPtr<UDieg_Grid>& Grid);

	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory")
	float GetInventoryScale3D() const { return InventoryScale3D; }
	
	UFUNCTION(BlueprintCallable, Category = "Diegetic Inventory")
	float GetGridSize3D() const { return GridSize3D; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHoverSlot OnHoverSlot;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUnHoverSlot OnUnHoverSlot;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPressedSlot OnPressedSlot;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FReleasedSlot OnReleasedSlot;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USizeBox> Overlay_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Fill;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Outline;

	UPROPERTY(VisibleAnywhere, Category = "Diegetic Inventory")
	TWeakObjectPtr<UDieg_Grid> ParentGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	EDieg_SlotStatus SlotStatus{EDieg_SlotStatus::None};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsLockedAppearance;

	// Scale in 2D space
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	FVector2D SlotSize{100.0f, 100.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<EDieg_SlotStatus, FLinearColor> SlotFillColor;

	// Scale of the items once parented
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	float InventoryScale3D{5.0f};

	// How big is the grid slot relative to the 2D, how big is in actual 3D space
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	float GridSize3D{20.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint LastUpdatedCoordinate;
};
