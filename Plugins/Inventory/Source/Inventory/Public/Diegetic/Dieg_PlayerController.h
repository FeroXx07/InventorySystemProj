// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Dieg_Interactor.h"
#include "Dieg_PlayerController.generated.h"

class UDieg_InventoryInputHandler;
class UDieg_ItemInstance;
class ADieg_WorldItemActor;
class UDieg_InteractWidget;
class UDieg_InventoryComponent;
class UInputAction;
class UDieg_TracerComponent;
class UInputMappingContext;

/**
 * @brief Delegate fired when an actor is traced in.
 * 
 * @param CurrentActor The actor that was traced
 * @param PreviousActor The previously traced actor
 * @param TraceChannel The collision channel used for tracing
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHandleTraceActorIn, AActor*, CurrentActor, AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

/**
 * @brief Delegate fired when an actor is traced out.
 * 
 * @param PreviousActor The actor that was traced out
 * @param TraceChannel The collision channel used for tracing
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleTraceActorOut,  AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

/**
 * @brief Player controller for the diegetic inventory system.
 * 
 * ADieg_PlayerController extends the base player controller with diegetic inventory
 * functionality. It manages input handling, interaction with inventory items,
 * tracing for interactable objects, and coordination between various inventory components.
 * 
 * The controller provides:
 * - Input mapping context management for inventory interactions
 * - Tracing system for detecting interactable objects
 * - Integration with inventory input handlers
 * - Interaction widget management
 * - Support for multiple input mapping contexts (UE 5.6+)
 * 
 * @note This controller implements IDieg_Interactor for interaction functionality.
 * @note This controller is designed to work with the diegetic inventory system.
 * 
 * @see IDieg_Interactor
 * @see UDieg_InventoryInputHandler
 * @see UDieg_TracerComponent
 * @see UDieg_InteractWidget
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API ADieg_PlayerController : public APlayerController, public IDieg_Interactor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Array of input mapping contexts for inventory interactions.
	 * 
	 * Multiple Input Mapping Contexts are required from UE 5.6 onwards.
	 * These contexts define how input actions are mapped to player input.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|PlayerController|InputMappingContexts")
	TArray<UInputMappingContext*> DefaultIMCs;

	/**
	 * @brief Component responsible for tracing interactable objects.
	 * 
	 * The tracer component performs line traces to detect objects that
	 * the player can interact with, such as inventory items and containers.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Game|Dieg|PlayerController|StrongReferences")
	UDieg_TracerComponent* TracerComponent;

	/**
	 * @brief Component managing the player's inventory.
	 * 
	 * The inventory component handles item storage, retrieval, and
	 * inventory state management for the player.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Game|Dieg|PlayerController|StrongReferences")
	UDieg_InventoryComponent* InventoryComponent;

	/**
	 * @brief Component handling input for inventory interactions.
	 * 
	 * The inventory input handler manages all input-related functionality
	 * for the diegetic inventory system, including drag-and-drop operations.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Game|Dieg|PlayerController|StrongReferences")
	UDieg_InventoryInputHandler* InventoryInputHandlerComponent;

	/**
	 * @brief Currently hovered actor reference.
	 * 
	 * Reference to the actor currently being hovered over by the player.
	 * Used for interaction feedback and UI display.
	 */
	UPROPERTY(VisibleAnywhere, Category= "Game|Dieg|PlayerController|WeakReferences")
	AActor* HoveredActor;
	

	/**
	 * @brief Class for the interaction widget.
	 * 
	 * The widget class to instantiate for displaying interaction prompts
	 * and feedback to the player.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Game|Dieg|PlayerController|StrongReferences")
	TSubclassOf<UDieg_InteractWidget> InteractWidgetClass;

	/**
	 * @brief Instance of the interaction widget.
	 * 
	 * The actual widget instance displayed to the player for interaction
	 * feedback and prompts.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|PlayerController|StrongReferences")
	UDieg_InteractWidget* InteractWidget;

	/**
	 * @brief Default constructor for the player controller.
	 * 
	 * Initializes the controller with default component references
	 * and sets up the basic configuration.
	 */
	ADieg_PlayerController();
	
	/**
	 * @brief Primary input action for interaction.
	 * 
	 * The input action that triggers primary interaction attempts,
	 * such as picking up items or opening containers.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|PlayerController|InputActions")
	UInputAction* PrimaryInteractIA;
	
	/**
	 * @brief Delegate fired when an actor enters the trace.
	 * 
	 * This delegate is triggered when the tracer component detects
	 * a new actor that can be interacted with.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|PlayerController")
	FHandleTraceActorIn OnHandleTraceActorIn;
	
	/**
	 * @brief Delegate fired when an actor exits the trace.
	 * 
	 * This delegate is triggered when the tracer component no longer
	 * detects an actor that was previously being traced.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|PlayerController")
	FHandleTraceActorOut OnHandleTraceActorOut;

	/**
	 * @brief Handle when an actor enters the trace.
	 * 
	 * Called when the tracer component detects a new actor that can be interacted with.
	 * Updates the hovered actor reference and triggers appropriate delegates.
	 * 
	 * @param CurrentActor The actor that was just traced
	 * @param PreviousActor The previously traced actor (if any)
	 * @param TraceChannel The collision channel used for the trace
	 */
	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void HandleActorInTrace(AActor* CurrentActor, AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);

	/**
	 * @brief Handle when an actor exits the trace.
	 * 
	 * Called when the tracer component no longer detects an actor that was
	 * previously being traced. Clears hover state and triggers appropriate delegates.
	 * 
	 * @param PreviousActor The actor that was traced out
	 * @param TraceChannel The collision channel used for the trace
	 */
	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void HandleActorOutTrace(AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);
	
	/**
	 * @brief Get the inventory component.
	 * 
	 * Returns a reference to the player's inventory component for external access.
	 * 
	 * @return Pointer to the inventory component
	 */
	UFUNCTION(BlueprintCallable, Category="Game|Dieg|Player Controller")
	UDieg_InventoryComponent* GetInventoryComponent() {return InventoryComponent;};

protected:
	/**
	 * @brief Begin play for the player controller.
	 * 
	 * Called when the game starts. Initializes components, creates widgets,
	 * and sets up delegate bindings.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Setup input component bindings.
	 * 
	 * Binds input actions to their corresponding callback functions
	 * and initializes the input mapping context subsystem.
	 */
	virtual void SetupInputComponent() override;
	
	/**
	 * @brief Initialize the input mapping context subsystem.
	 * 
	 * Adds all configured input mapping contexts to the enhanced input
	 * subsystem for proper input handling.
	 */
	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void InitializeImcSubsystem();

	/**
	 * @brief Attempt to toggle the inventory state.
	 * 
	 * Blueprint implementable event that handles opening and closing
	 * the inventory interface.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void TryToggleInventory();

	/**
	 * @brief Attempt to interact with the currently hovered object.
	 * 
	 * Blueprint implementable event that handles primary interaction
	 * attempts with objects in the world.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void TryInteract();

	/**
	 * @brief Play the pickup animation montage.
	 * 
	 * Optional Blueprint implementable event for playing pickup animations
	 * when items are collected.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void PlayPickUpMontage();

	/**
	 * @brief Attempt to pick up a world item actor.
	 * 
	 * Blueprint implementable event that handles picking up items
	 * from the world and adding them to the inventory.
	 * 
	 * @param WorldItemActor The world item actor to pick up
	 * @return True if the item was successfully picked up
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	bool TryPickUpItem(ADieg_WorldItemActor* WorldItemActor);

	/**
	 * @brief Handle when the inventory is opened.
	 * 
	 * Blueprint implementable event called when the inventory interface
	 * is opened. Can be used for UI setup and state management.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void HandleOnInventoryOpened();
	
	/**
	 * @brief Handle when the inventory is closed.
	 * 
	 * Blueprint implementable event called when the inventory interface
	 * is closed. Can be used for cleanup and state management.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void HandleOnInventoryClosed();

	/**
	 * @brief Whether the inventory is currently open.
	 * 
	 * Tracks the current state of the inventory interface for
	 * state management and UI updates.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|PlayerController")
	bool bInventoryOpened;

private:
	/**
	 * @brief Notify that an interaction has started.
	 * 
	 * Implementation of IDieg_Interactor interface. Called when
	 * an interaction with an interactable object begins.
	 * 
	 * @param Interactable The object being interacted with
	 */
	virtual void NotifyInteractionStarted_Implementation(UObject* Interactable) override;
	
	/**
	 * @brief Notify that an interaction has ended.
	 * 
	 * Implementation of IDieg_Interactor interface. Called when
	 * an interaction with an interactable object ends.
	 * 
	 * @param Interactable The object that was being interacted with
	 */
	virtual void NotifyInteractionEnded_Implementation(UObject* Interactable) override;
	
	/**
	 * @brief Handle the actual interaction logic.
	 * 
	 * Implementation of IDieg_Interactor interface. Contains the
	 * core logic for interacting with objects in the world.
	 * 
	 * @param Interactable The object to interact with
	 */
	virtual void OnInteraction_Implementation(UObject* Interactable) override;
};
