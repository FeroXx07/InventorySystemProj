// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dieg_3DInventoryComponent.generated.h"


class ADieg_WorldItemActor;
class UDieg_Slot;
class UWidgetComponent;
class UDieg_Grid;
class UDieg_InventoryComponent;

/**
 * @brief Delegate fired when linking an inventory reference externally.
 * 
 * @param NewInventoryRef Reference to the new inventory component
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLinkInventoryRefExternally, UDieg_InventoryComponent*&, NewInventoryRef);
/**
 * @brief Delegate fired when hovering over a slot in the 3D inventory widget.
 * 
 * @param InGeometry The geometry of the slot widget
 * @param InMouseEvent The mouse event that triggered the hover
 * @param InventoryComponent3D The 3D inventory component containing the slot
 * @param Slot The slot that was hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FWidgetSlotHover, const FGeometry&, InGeometry, const FPointerEvent&, InMouseEvent, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
/**
 * @brief Delegate fired when unhovering a slot in the 3D inventory widget.
 * 
 * @param InMouseEvent The mouse event that triggered the unhover
 * @param InventoryComponent3D The 3D inventory component containing the slot
 * @param Slot The slot that was unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWidgetSlotUnHover, const FPointerEvent&, InMouseEvent, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
/**
 * @brief Delegate fired when the input handler hovers over an item.
 * 
 * @param InventoryInputHandler The input handler managing the hover
 * @param HoveredItem The item being hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandlerHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, HoveredItem);

/**
 * @brief Delegate fired when the input handler unhovers an item.
 * 
 * @param InventoryInputHandler The input handler managing the unhover
 * @param UnHoveredItem The item being unhovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandlerUnHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, HoveredItem);

/**
 * @brief 3D inventory management component for the diegetic inventory system.
 * 
 * UDieg_3DInventoryComponent manages the 3D representation of inventories in the game world.
 * It handles the visual display of inventory grids, item placement, and interaction with
 * the 3D world. This component bridges the gap between the 2D inventory system and the
 * 3D world representation.
 * 
 * The component provides:
 * - 3D widget display for inventory grids
 * - Item actor management and positioning
 * - Input handling for 3D interactions
 * - Visual feedback for hover and selection states
 * - Integration with the core inventory system
 * 
 * @note This component requires a UDieg_InventoryComponent reference to function.
 * @note This component is designed to work with UDieg_InventoryInputHandler for input.
 * 
 * @see UDieg_InventoryComponent
 * @see UDieg_InventoryInputHandler
 * @see ADieg_WorldItemActor
 * @see UDieg_Grid
 * 
 * @since 1.0
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_3DInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_3DInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FLinkInventoryRefExternally OnInventoryExternalLinkRequest;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotHover On3DWidgetSlotHovered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotUnHover On3DWidgetSlotUnHovered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerHoverItem OnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerUnHoverItem OnUnHoverItem;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryComponent> InventoryComponentRef;

	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<ADieg_WorldItemActor*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_WorldItemActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_Grid> GridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UWidgetComponent> WidgetComponentRef;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	bool bIsInitialized{false};

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void PreInitialize();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void Initialize();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void Populate3D();

	UFUNCTION(Category = "Constructor", BlueprintCallable)
	void DeInitializeInventory();
	
	UFUNCTION(Category = "Event Handler", BlueprintCallable)
	void WidgetSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION(Category = "Event Handler", BlueprintCallable)
	void WidgetSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

public:
	UFUNCTION(Category = "Direct", BlueprintCallable)
	void AddItemToInventory(ADieg_WorldItemActor* ItemActor);

	UFUNCTION(Category = "Direct", BlueprintCallable)
	void AddItemToInventorySlot(ADieg_WorldItemActor* ItemActor, const FIntPoint& SlotCoordinates, int32 RotationUsed);

	UFUNCTION(Category = "Direct", BlueprintCallable)
	void RemoveItemFromInventory(ADieg_WorldItemActor* ItemActor);

	
	UFUNCTION(BlueprintCallable)
	UDieg_Grid* GetGridWidget() {return GridWidget;}

	TWeakObjectPtr<UWidgetComponent> GetWidgetComponent() {return WidgetComponentRef;}
	TObjectPtr<UDieg_InventoryComponent> GetInventoryComponent() {return InventoryComponentRef;}

	TArray<ADieg_WorldItemActor*>& GetItems() {return Items;}
};
