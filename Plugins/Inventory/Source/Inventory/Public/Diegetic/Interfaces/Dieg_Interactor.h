// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dieg_Interactor.generated.h"

class IDieg_Interactable;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDieg_Interactor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IDieg_Interactor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteraction(UObject* Interactable);

	// Called when an interaction starts (useful for UI feedback, animations, etc.)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyInteractionStarted(UObject* Interactable);

	// Called when an interaction ends
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotifyInteractionEnded(UObject* Interactable);
};
