// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "A_PlugInv_PlayerController.generated.h"

/**
 * 
 */

class UPlugInv_InventoryComponent;
class UInputMappingContext;
class UInputAction;
class UPlugInv_HUDWidget;

UCLASS()
class INVENTORY_API APlugInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APlugInv_PlayerController();

	virtual void Tick(float DeltaSeconds) override;

	// Callback function for IA inventory.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

protected:
	virtual void BeginPlay() override;

	// Initiating all stored Input Mapping Contexts.
	void InitIMCSubsystem();
private:

	// Since it is added through blueprint it doesn't deal with GB.
	TWeakObjectPtr<UPlugInv_InventoryComponent> InventoryComponent;

	// Multiple Input Mapping Contexts required from UE 5.6 onwards.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	// Primary IA for interaction. 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractIA;

	// IA for toggling inventory. 
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryIA;

	// Storage for the HUD widget class type for later instantiation.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UPlugInv_HUDWidget> HUDWidgetClass;

	// Storage for the instantiated HUD widget.
	UPROPERTY()
	TObjectPtr<UPlugInv_HUDWidget> HUDWidget;

	// Max distance for interaction.
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	// Trace channel enabled for interactions
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	// The Actor currently in trace line.
	TWeakObjectPtr<AActor> CurrentTraceActor;

	// The last Actor different to current Actor in trace line.
	TWeakObjectPtr<AActor> PreviousTraceActor;

	// Callback function for IA interaction.
	void PrimaryInteract();
	
	// Function for creating and adding to viewport the HUD widget.
	void CreateHUDWidget();

	// Function to trace for collisions that have item trace channel
	void TraceForItem();

protected:

	// Binding of the IA (Input Actions) and callback functions.
	virtual void SetupInputComponent() override;
};
