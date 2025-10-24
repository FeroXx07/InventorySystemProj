// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_DataLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Inv_Slot.generated.h"

class UImage;
class UCanvasPanel;
class USizeBox;
class UInv_Grid;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlotUnHover, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSlotPressed, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSlotReleased, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_Slot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> Overlay_Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Fill;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Outline;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UInv_Grid> ParentGrid;

	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	EInv_SlotState State{EInv_SlotState::None};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	FVector2D Size2D{100.0f, 100.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	TMap<EInv_SlotState, FLinearColor> FillColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	FIntPoint GridCoordinates{-1, -1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Game|Inv|Slot", meta = (AllowPrivateAccess = "true"))
	bool bIsStateMutable{true};
public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Slot")
	FSlotHover OnHoverSlot;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Slot")
	FSlotUnHover OnUnHoverSlot;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Slot")
	FSlotPressed OnPressedSlot;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Slot")
	FSlotReleased OnReleasedSlot;

	UFUNCTION(BlueprintNativeEvent, Category = "Game|Inv|Slot")
	void InitializeDefaults();

	UFUNCTION(BlueprintNativeEvent, Category = "Game|Inv|Slot")
	void InitializeWithGrid(UInv_Grid* Grid);

	EInv_SlotState GetState() const { return State; }
	void SetState(const EInv_SlotState InState, bool UpdateColor = true);
	
	const FVector2D& GetSize2D() const { return Size2D; }
	void SetSize2D(const FVector2D& InSize2D) {this->Size2D = InSize2D; }

	const TMap<EInv_SlotState, FLinearColor>& GetFillColor() const { return FillColor; }
	void SetFillColor(const TMap<EInv_SlotState, FLinearColor>& InFillColor) {this->FillColor = InFillColor; }

	const FIntPoint& GetGridCoordinates() const { return GridCoordinates; }
	void SetGridCoordinates(const FIntPoint& InGridCoordinates) { this->GridCoordinates = InGridCoordinates;}

	bool IsStateMutable() const { return bIsStateMutable; }
	void SetStateMutable(const bool bInIsStateMutable) { this->bIsStateMutable = bInIsStateMutable; }
};
