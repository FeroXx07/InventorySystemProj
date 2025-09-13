// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dieg_Grid.generated.h"

enum class EDieg_SlotStatus : uint8;
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
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UGridPanel> GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 TotalSlots{35};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	int32 MaxColumns{10};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDieg_Slot> SlotClass;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FIntPoint, TObjectPtr<UDieg_Slot>> SlotMap;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SlotsHoveredSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Grid|Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bDebugDesignTimeEditor{false};

	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|Grid")
	void CreateEmptyGrid(int32 TotalSlots_, int32 MaxColumns_);
	
	// C++ only - returns reference to map for efficiency
	TMap<FIntPoint, UDieg_Slot*>& GetSlotMap();
	
	// Blueprint compatible - returns map by value
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	TMap<FIntPoint, UDieg_Slot*> GetSlotMapBP();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotHover OnGridSlotHover;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotUnHover OnGridSlotUnHover;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotPressed OnGridSlotPressed;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Grid")
	FGridSlotReleased OnGridSlotReleased;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	void UpdateHoveringSlots(const TArray<FIntPoint>& NewCoordinates, EDieg_SlotStatus NewStatus, EDieg_SlotStatus ResetStatus);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Grid")
	void ModifyAllSlotsAppearance(bool IsAppearanceLocked, bool Override, EDieg_SlotStatus OverrideStatus);

private:
	UFUNCTION(Category = "Game|Dieg|Grid")
	UDieg_Slot* CreateSlot(const FIntPoint& Point);

	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Game|Dieg|Grid")
	void ChildSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);
};
