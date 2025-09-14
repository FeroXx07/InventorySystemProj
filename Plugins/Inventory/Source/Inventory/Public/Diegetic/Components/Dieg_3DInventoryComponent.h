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
	/**
	 * @brief Default constructor for the 3D inventory component.
	 * 
	 * Initializes the component with default values and enables ticking
	 * for continuous updates of the 3D inventory representation.
	 */
	UDieg_3DInventoryComponent();

	/**
	 * @brief Delegate fired when requesting external inventory reference linking.
	 * 
	 * This delegate is triggered when the component needs to establish
	 * a connection to an external inventory component. Bind to this
	 * delegate to provide the inventory reference.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FLinkInventoryRefExternally OnInventoryExternalLinkRequest;

	/**
	 * @brief Delegate fired when hovering over a slot in the 3D widget.
	 * 
	 * This delegate is triggered when the mouse hovers over a slot
	 * in the 3D inventory widget. Provides geometry and mouse event information.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotHover On3DWidgetSlotHovered;

	/**
	 * @brief Delegate fired when unhovering a slot in the 3D widget.
	 * 
	 * This delegate is triggered when the mouse stops hovering over a slot
	 * in the 3D inventory widget. Provides mouse event information.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FWidgetSlotUnHover On3DWidgetSlotUnHovered;

	/**
	 * @brief Delegate fired when the input handler hovers over an item.
	 * 
	 * This delegate is triggered when the input handler detects hovering
	 * over an item in the 3D inventory. Provides input handler and item references.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerHoverItem OnHoverItem;

	/**
	 * @brief Delegate fired when the input handler unhovers an item.
	 * 
	 * This delegate is triggered when the input handler stops hovering
	 * over an item in the 3D inventory. Provides input handler and item references.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHandlerUnHoverItem OnUnHoverItem;
protected:
	/**
	 * @brief Reference to the core inventory component.
	 * 
	 * The inventory component that this 3D component represents.
	 * This reference is required for the 3D component to function properly.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryComponent> InventoryComponentRef;

	/**
	 * @brief Array of world item actors in this inventory.
	 * 
	 * Contains all the 3D item actors currently displayed in this
	 * 3D inventory component. Used for management and positioning.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<ADieg_WorldItemActor*> Items;

	/**
	 * @brief Class for creating new world item actors.
	 * 
	 * The class to instantiate when creating new 3D item actors
	 * for this inventory component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_WorldItemActor> ItemClass;

	/**
	 * @brief The grid widget for displaying the inventory.
	 * 
	 * The 3D widget component that displays the inventory grid
	 * and handles slot interactions in the 3D world.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_Grid> GridWidget;

	/**
	 * @brief Reference to the widget component.
	 * 
	 * Weak reference to the widget component that contains the
	 * 3D inventory grid widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UWidgetComponent> WidgetComponentRef;
	
	/**
	 * @brief Begin play for the component.
	 * 
	 * Called when the game starts. Initializes the 3D inventory
	 * component and prepares it for use.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Tick the component.
	 * 
	 * Called every frame. Updates the 3D inventory representation
	 * and handles any ongoing operations.
	 * 
	 * @param DeltaTime Time elapsed since last tick
	 * @param TickType Type of tick being performed
	 * @param ThisTickFunction The tick function for this component
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Whether the component has been initialized.
	 * 
	 * Tracks whether the 3D inventory component has been properly
	 * initialized and is ready for use.
	 */
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
