// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dieg_Interactable.generated.h"

class IDieg_Interactor;

/**
 * @brief UInterface implementation for the interactable interface.
 * 
 * This class does not need to be modified. It provides the UInterface
 * implementation for the IDieg_Interactable interface.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDieg_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for objects that can be interacted with.
 * 
 * IDieg_Interactable defines the contract for objects that can be
 * interacted with by interactor objects in the diegetic inventory system.
 * This interface is typically implemented by world items, inventory actors,
 * and other objects that players can interact with.
 * 
 * The interface provides:
 * - Main interaction handling
 * - Interaction prompt text
 * - Interaction availability checking
 * 
 * @note This interface is Blueprint-compatible and can be implemented in Blueprint.
 * @note Objects implementing this interface can be interacted with by IDieg_Interactor objects.
 * 
 * @see IDieg_Interactor
 * @see ADieg_WorldItemActor
 * @see ADieg_WorldInventoryActor
 * 
 * @since 1.0
 */
class INVENTORY_API IDieg_Interactable
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when this object is interacted with.
	 * 
	 * This is the main interaction function that gets called when an
	 * interactor interacts with this object.
	 * 
	 * @param Interactor The object that is interacting with this object
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract(UObject* Interactor);

	/**
	 * @brief Gets the interaction prompt text for this object.
	 * 
	 * Returns the text that should be displayed to the player when
	 * they can interact with this object (e.g., "Press E to open").
	 * 
	 * @return The interaction prompt text
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractionPrompt();

	/**
	 * @brief Checks if this object can be interacted with by the given interactor.
	 * 
	 * This function is useful for conditional interactions, long interactions
	 * (holding a button, progress bar, etc.), or other interaction requirements.
	 * 
	 * @param Interactor The object that wants to interact with this object
	 * @return True if the interaction is allowed, false otherwise
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(UObject* Interactor);
};
