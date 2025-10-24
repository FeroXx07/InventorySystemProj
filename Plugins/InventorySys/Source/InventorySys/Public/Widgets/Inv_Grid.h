// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_Grid.generated.h"

enum class EInv_SlotState : uint8;
class UInv_Slot;
class UGridPanel;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotUnHover, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotPressed, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotReleased, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UInv_Slot*, Slot);

/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_Grid : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Grid", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UGridPanel> GridPanel;

	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Grid")
	FGridSlotHover OnGridSlotHover;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Grid")
	FGridSlotUnHover OnGridSlotUnHover;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Grid")
	FGridSlotPressed OnGridSlotPressed;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Inv|Grid")
	FGridSlotReleased OnGridSlotReleased;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	int32 TotalSlots{35};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	int32 MaxColumns{10};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInv_Slot> SlotClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, TObjectPtr<UInv_Slot>> SlotMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SlotsHoveredSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Inv|Grid", meta = (AllowPrivateAccess = "true"))
	bool bDebugDesignTimeEditor{false};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void CreateGrid(int32 InTotalSlots, int32 InMaxColumns);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	UInv_Slot* CreateSlot(const FIntPoint& InGridCoordinates);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void UpdateHoveringSlots(const TArray<FIntPoint>& SlotsCoordinatesToUpdate, EInv_SlotState NewStatus, EInv_SlotState ResetStatus);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void ChildSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void ChildSlotUnHovered(const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void ChildSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Inv|Grid")
	void ChildSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);
};
