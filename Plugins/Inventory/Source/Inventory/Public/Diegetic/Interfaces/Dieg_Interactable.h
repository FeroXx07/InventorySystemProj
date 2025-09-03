// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Dieg_Interactable.generated.h"

class IDieg_Interactor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDieg_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IDieg_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Main entry point: tells this object "You are being interacted with"
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteract(UObject* Interactor);

	// Optional: show a UI prompt like "Press E to open"
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractionPrompt();

	// Optional: for long interactions (holding a button, progress bar, etc.)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanInteract(UObject* Interactor);
};
