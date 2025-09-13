// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/Dieg_DataLibrary.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"
#include "Diegetic/Interfaces/Dieg_Interactable.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldItemActor.generated.h"

class UDieg_InventoryInputHandler;
class UTextRenderComponent;
class UDieg_ItemInstance;
class UDieg_ItemDefinitionDataAsset;

/**
 * @brief World actor that represents an item in the 3D world for the diegetic inventory system.
 * 
 * ADieg_WorldItemActor is a 3D world actor that represents an item that can be picked up,
 * dragged, and placed in inventories. It serves as the bridge between the 3D world and the
 * inventory system, handling visual representation, interaction, and drag-and-drop operations.
 * 
 * The actor can exist in two states:
 * - World state: Free-floating in the 3D world, can be picked up by players
 * - Inventory state: Contained within an inventory, can be dragged and placed
 * 
 * The actor automatically adjusts its visual appearance, position, and rotation based on
 * its current state and the item data it represents.
 * 
 * @note This actor implements IDieg_Interactable for player interaction.
 * @note This actor is designed to work with the diegetic inventory system and requires
 * proper setup with item instances and input handlers.
 * 
 * @see UDieg_ItemInstance
 * @see UDieg_3DInventoryComponent
 * @see UDieg_InventoryInputHandler
 * @see IDieg_Interactable
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API ADieg_WorldItemActor : public AActor, public IDieg_Interactable
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Root component for the actor hierarchy.
	 * 
	 * The root scene component that all other components are attached to.
	 * This is the base of the actor's transform hierarchy.
	 * 
	 * @see USceneComponent
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	/**
	 * @brief Static mesh component that displays the item's 3D model.
	 * 
	 * This component renders the visual representation of the item in the 3D world.
	 * The mesh is automatically set based on the item's definition data.
	 * 
	 * @see UStaticMeshComponent
	 * @see SetMesh
	 * @see UDieg_ItemDefinition::WorldMesh
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/**
	 * @brief Text render component for displaying item information.
	 * 
	 * Optional component that can display text information about the item,
	 * such as quantity or item name. Can be used for debugging or UI purposes.
	 * 
	 * @see UTextRenderComponent
	 * @see AdjustText
	 * @see ModifyTextQuantity
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> TextRendererComponent;
	
	/**
	 * @brief Prepopulate data for initializing the item.
	 * 
	 * Contains the data needed to create and initialize the item instance
	 * when the actor is spawned. This is used for items placed in the world
	 * during level design.
	 * 
	 * @see FDieg_PrePopulate
	 * @see UDieg_ItemInstance
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Items" , meta = (AllowPrivateAccess = "true")) 
	FDieg_PrePopulate PrePopulateData;
	
	/**
	 * @brief The item instance that this actor represents.
	 * 
	 * Contains all the data and functionality for the item, including
	 * its definition, quantity, and fragments. This is the core data
	 * that drives the actor's behavior.
	 * 
	 * @see UDieg_ItemInstance
	 * @see GetItemInstance
	 * @see SetFromItemInstance
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Items", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_ItemInstance> ItemInstance;

	/**
	 * @brief Z-axis offset for positioning the item in the world.
	 * 
	 * Additional offset applied to the item's position along the Z-axis.
	 * This can be used to adjust the item's height relative to the ground
	 * or other reference points.
	 * 
	 * @see AdjustActorLocation
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	float OffsetZ{0.0f};

	/**
	 * @brief Last known inventory slot data for this item.
	 * 
	 * Stores the inventory slot information from when the item was last
	 * in an inventory. This is used for restoring the item's state
	 * when it's removed from an inventory.
	 * 
	 * @see FDieg_InventorySlot
	 * @see SetLastSlotData
	 * @see GetLastSlotData
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	FDieg_InventorySlot LastInventorySlotData;

	/**
	 * @brief Current rotation of the item in degrees.
	 * 
	 * The rotation angle applied to the item when it's placed in an inventory
	 * or positioned in the world. This affects both visual representation
	 * and collision detection.
	 * 
	 * @see SetCurrentRotation
	 * @see GetCurrentRotation
	 * @see AdjustMeshRelativeRotation
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	float CurrentRotation{0.0f};

	/**
	 * @brief Current quantity of the item.
	 * 
	 * The number of units of this item that this actor represents.
	 * For stackable items, this can be greater than 1.
	 * 
	 * @see GetQuantity
	 * @see SetQuantity
	 * @see UDieg_ItemInstance::Quantity
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	int32 Quantity {1};

	/**
	 * @brief Default constructor for the world item actor.
	 * 
	 * Initializes the actor with default values and sets up the component hierarchy.
	 */
	ADieg_WorldItemActor();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Text
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustText() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void ModifyTextQuantity();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	FIntPoint GetTextCoordinates(const TArray<FIntPoint>& Shape, EDieg_TextLocation TextLocation) const;

	// Transform
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustActorLocation();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustMeshRelativeRotation() const; // Previously named adjust rotation
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustForGrabPoint(const FVector2D& GrabPoint) const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustRootTransformForGrid() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustRootTransformForWorld();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	float GetUnitScaled() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	FVector GetLocationMultiplier() const;

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleDragHoverEnterInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleDragHoverLeaveInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStartDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FIntPoint Coordinates);
	
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStartDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FVector WorldLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStopDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                             DroppedItem, FIntPoint DroppedCoordinates, float DroppedRotation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStopDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, FVector DroppedLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleResetDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor* RestItem, FIntPoint
	                              ResetCoordinates, float ResetRotation);
	
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleResetDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* ResetItem, FVector ResetLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleRotateItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* RotatedItem, float NewRotation, FIntPoint GripPoint);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleMergeItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, TArray<AActor*> MergedActors, int32
	                              OldQuantity, int32 NewQuantity);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleConsumedItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                                 ConsumedActor);

public:
	/**
	 * @brief Called every frame.
	 * 
	 * @param DeltaTime Time elapsed since last frame
	 */
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief Implementation of the interactable interface.
	 * 
	 * Called when a player interacts with this item actor.
	 * This is where pickup logic would typically be implemented.
	 * 
	 * @param Interactor The object that is interacting with this item
	 * 
	 * @see IDieg_Interactable
	 */
	virtual void OnInteract_Implementation(UObject* Interactor) override;
	
	/**
	 * @brief Binds this actor to an inventory input handler.
	 * 
	 * This Blueprint-native event connects the actor to the input handler
	 * for drag-and-drop operations. Override this in Blueprint to customize
	 * the binding process.
	 * 
	 * @param Handler The input handler to bind to
	 * 
	 * @see UDieg_InventoryInputHandler
	 * @see UnBindEventsFromHandler
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|World Item Actor")
	void BindEventsToHandler(UDieg_InventoryInputHandler* Handler);

	/**
	 * @brief Unbinds this actor from an inventory input handler.
	 * 
	 * This Blueprint-native event disconnects the actor from the input handler.
	 * Override this in Blueprint to customize the unbinding process.
	 * 
	 * @param Handler The input handler to unbind from
	 * 
	 * @see UDieg_InventoryInputHandler
	 * @see BindEventsToHandler
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|World Item Actor")
	void UnBindEventsFromHandler(UDieg_InventoryInputHandler* Handler);
	
	/**
	 * @brief Checks if this actor is owned by an inventory component.
	 * 
	 * @return true if the actor is owned by an inventory, false otherwise
	 * 
	 * @see IsInInventory
	 * @see GetOwnerComponent
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	bool IsOwnedByInventory() const;
	
	/**
	 * @brief Checks if this actor is currently in an inventory.
	 * 
	 * @return true if the actor is in an inventory, false otherwise
	 * 
	 * @see IsOwnedByInventory
	 * @see GetOwnerComponent
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	bool IsInInventory() const;
	
	/**
	 * @brief Gets the inventory component that owns this actor.
	 * 
	 * @return The inventory component that owns this actor, or nullptr if not owned
	 * 
	 * @see UDieg_3DInventoryComponent
	 * @see IsOwnedByInventory
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	const UDieg_3DInventoryComponent* GetOwnerComponent() const;
	
	/**
	 * @brief Gets the item instance for this actor (const version).
	 * 
	 * @return The item instance, or nullptr if not set
	 * 
	 * @see ItemInstance
	 * @see GetItemInstanceMutable
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	UDieg_ItemInstance* GetItemInstance() const;
	
	/**
	 * @brief Gets the item instance for this actor (mutable version).
	 * 
	 * @return The item instance, or nullptr if not set
	 * 
	 * @see ItemInstance
	 * @see GetItemInstance
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	UDieg_ItemInstance* GetItemInstanceMutable();

	/**
	 * @brief Gets the current quantity of the item.
	 * 
	 * @return The current quantity
	 * 
	 * @see Quantity
	 * @see SetQuantity
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	int32 GetQuantity() const;
	
	/**
	 * @brief Sets the quantity of the item.
	 * 
	 * @param Value The new quantity value
	 * 
	 * @see Quantity
	 * @see GetQuantity
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetQuantity(const int32 Value);
	
	/**
	 * @brief Sets the current rotation of the item.
	 * 
	 * @param Value The new rotation angle in degrees
	 * 
	 * @see CurrentRotation
	 * @see GetCurrentRotation
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetCurrentRotation(float Value) { CurrentRotation = Value; }
	
	/**
	 * @brief Gets the current rotation of the item.
	 * 
	 * @return The current rotation angle in degrees
	 * 
	 * @see CurrentRotation
	 * @see SetCurrentRotation
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	float GetCurrentRotation() const { return CurrentRotation; }
	
	/**
	 * @brief Sets the coordinates of the item in the inventory grid.
	 * 
	 * @param Value The new grid coordinates
	 * 
	 * @see LastInventorySlotData
	 * @see GetCoordinates
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetCoordinates(const FIntPoint& Value) { LastInventorySlotData.Coordinates = Value; }
	
	/**
	 * @brief Gets the coordinates of the item in the inventory grid.
	 * 
	 * @return The current grid coordinates
	 * 
	 * @see LastInventorySlotData
	 * @see SetCoordinates
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	FIntPoint GetCoordinates() const { return LastInventorySlotData.Coordinates; }
	
	/**
	 * @brief Sets the last known inventory slot data.
	 * 
	 * @param Slot The inventory slot data to store
	 * 
	 * @see LastInventorySlotData
	 * @see GetLastSlotData
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetLastSlotData(const FDieg_InventorySlot& Slot) { LastInventorySlotData = Slot; }
	
	/**
	 * @brief Gets the last known inventory slot data.
	 * 
	 * @return The stored inventory slot data
	 * 
	 * @see LastInventorySlotData
	 * @see SetLastSlotData
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	const FDieg_InventorySlot& GetLastSlotData() const { return LastInventorySlotData; }

	/**
	 * @brief Updates the actor from an item definition data asset.
	 * 
	 * This Blueprint-native event updates the actor's appearance and properties
	 * based on the provided data asset. Override this in Blueprint to customize
	 * the update process.
	 * 
	 * @param InItemDataAsset The data asset to update from
	 * 
	 * @see UDieg_ItemDefinitionDataAsset
	 * @see SetFromItemInstance
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void UpdateFromDataAsset(UDieg_ItemDefinitionDataAsset* InItemDataAsset);

	/**
	 * @brief Sets the actor from an item instance.
	 * 
	 * This Blueprint-native event configures the actor based on the provided
	 * item instance. Override this in Blueprint to customize the configuration.
	 * 
	 * @param Instance The item instance to configure from
	 * 
	 * @see UDieg_ItemInstance
	 * @see SetFromInventorySlot
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void SetFromItemInstance(UDieg_ItemInstance* Instance);

	/**
	 * @brief Sets the actor from an inventory slot.
	 * 
	 * This Blueprint-native event configures the actor based on the provided
	 * inventory slot data. Override this in Blueprint to customize the configuration.
	 * 
	 * @param InventorySlot The inventory slot data to configure from
	 * 
	 * @see FDieg_InventorySlot
	 * @see SetFromItemInstance
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void SetFromInventorySlot(const FDieg_InventorySlot& InventorySlot);

private:
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	bool SetMesh(const UDieg_ItemDefinitionDataAsset* InItemDataAsset) const;
};
