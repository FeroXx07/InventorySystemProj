// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dieg_Interactor.generated.h"

class IDieg_Interactable;

/**
 * @brief UInterface implementation for the interactor interface.
 * 
 * This class does not need to be modified. It provides the UInterface
 * implementation for the IDieg_Interactor interface.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDieg_Interactor : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for objects that can interact with interactable objects.
 * 
 * IDieg_Interactor defines the contract for objects that can initiate
 * interactions with interactable objects in the diegetic inventory system.
 * This interface is typically implemented by player controllers or other
 * objects that can trigger interactions.
 * 
 * The interface provides:
 * - Main interaction handling
 * - Interaction start notifications
 * - Interaction end notifications
 * 
 * @note This interface is Blueprint-compatible and can be implemented in Blueprint.
 * @note Objects implementing this interface can interact with IDieg_Interactable objects.
 * 
 * @see IDieg_Interactable
 * @see ADieg_PlayerController
 * 
 * @since 1.0
 */
class INVENTORY_API IDieg_Interactor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called when an interaction occurs.
	 * 
	 * This is the main interaction function that gets called when the
	 * interactor interacts with an interactable object.
	 * 
	 * @param Interactable The object being interacted with
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteraction(UObject* Interactable);

	/**
	 * @brief Called when an interaction starts.
	 * 
	 * This function is called when an interaction begins, useful for
	 * UI feedback, animations, or other start-of-interaction logic.
	 * 
	 * @param Interactable The object being interacted with
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyInteractionStarted(UObject* Interactable);

	/**
	 * @brief Called when an interaction ends.
	 * 
	 * This function is called when an interaction concludes, useful for
	 * cleanup, UI updates, or other end-of-interaction logic.
	 * 
	 * @param Interactable The object that was being interacted with
	 * 
	 * @note This is a Blueprint implementable event.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyInteractionEnded(UObject* Interactable);
};
