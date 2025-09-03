// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dieg_Grid.generated.h"

class UCanvasPanel;
class UDieg_Slot;
class UGridPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotUnHover, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotPressed, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridSlotReleased, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_Slot*, Slot);
/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_Grid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Diegetic Inventory")
	void CreateEmptyGrid(int32 TotalSlots_, int32 MaxColumns_);
	
	TMap<FIntPoint, TObjectPtr<UDieg_Slot>>& GetSlotMap();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridSlotHover OnGridSlotHover;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridSlotUnHover OnGridSlotUnHover;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridSlotPressed OnGridSlotPressed;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGridSlotReleased OnGridSlotReleased;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 TotalSlots{35};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxColumns{10};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDieg_Slot> SlotClass;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, TObjectPtr<UDieg_Slot>> SlotMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SlotsHoveredSet;

	UDieg_Slot* CreateSlot(const FIntPoint& Point);

	UFUNCTION()
	void ChildSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void ChildSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void ChildSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void ChildSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bDebugDesignTimeEditor{false};
};
