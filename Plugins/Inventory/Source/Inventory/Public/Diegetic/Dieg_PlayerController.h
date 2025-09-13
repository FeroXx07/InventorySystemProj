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
	// Multiple Input Mapping Contexts required from UE 5.6 onwards.
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Player Controller|Input Mapping Contexts", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_TracerComponent> TracerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InventoryInputHandler> InventoryInputHandlerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> HoveredActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Game|Dieg|Player Controller|Strong References", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDieg_InteractWidget> InteractWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller|Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_InteractWidget> InteractWidget;

	ADieg_PlayerController();
	
	// Primary IA for interaction. 
	UPROPERTY(EditDefaultsOnly, Category = "Game|Dieg|Player Controller|Input Actions")
	TObjectPtr<UInputAction> PrimaryInteractIA;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	FHandleTraceActorIn OnHandleTraceActorIn;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	FHandleTraceActorOut OnHandleTraceActorOut;

	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void HandleActorInTrace(AActor* CurrentActor, AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);

	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void HandleActorOutTrace(AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);
	
	UFUNCTION(BlueprintCallable, Category="Game|Dieg|Player Controller")
	UDieg_InventoryComponent* GetInventoryComponent() {return InventoryComponent;};

protected:
	virtual void BeginPlay() override;
	
	// Binding of the IA (Input Actions) and callback functions.
	virtual void SetupInputComponent() override;
	
	// Initiating all stored Input Mapping Contexts.
	UFUNCTION(Category = "Game|Dieg|Player Controller")
	void InitializeImcSubsystem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void TryToggleInventory();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void TryInteract();

	// Optional
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void PlayPickUpMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	bool TryPickUpItem(ADieg_WorldItemActor* WorldItemActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void HandleOnInventoryOpened();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Player Controller")
	void HandleOnInventoryClosed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Game|Dieg|Player Controller", meta = (AllowPrivateAccess = "true"))
	bool bInventoryOpened;

private:
	virtual void NotifyInteractionStarted_Implementation(UObject* Interactable) override;
	virtual void NotifyInteractionEnded_Implementation(UObject* Interactable) override;
	virtual void OnInteraction_Implementation(UObject* Interactable) override;
};
