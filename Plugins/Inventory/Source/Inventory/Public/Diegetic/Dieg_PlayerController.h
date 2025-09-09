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
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHandleTraceActorIn, AActor*, CurrentActor, AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHandleTraceActorOut,  AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

UCLASS()
class INVENTORY_API ADieg_PlayerController : public APlayerController, public IDieg_Interactor
{
	GENERATED_BODY()
public:
	ADieg_PlayerController();
	
	// Primary IA for interaction. 
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> PrimaryInteractIA;

	// IA for toggling inventory. 
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> ToggleInventoryIA;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Player|Dieg")
	FHandleTraceActorIn OnHandleTraceActorIn;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Player|Dieg")
	FHandleTraceActorOut OnHandleTraceActorOut;

	UFUNCTION(Category = "Game|Player|Dieg")
	void HandleActorInTrace(AActor* CurrentActor, AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);

	UFUNCTION(Category = "Game|Player|Dieg")
	void HandleActorOutTrace(AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel);

protected:
	virtual void BeginPlay() override;
	
	// Binding of the IA (Input Actions) and callback functions.
	virtual void SetupInputComponent() override;
	
	// Initiating all stored Input Mapping Contexts.
	void InitializeImcSubsystem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Player|Dieg")
	void TryToggleInventory();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Player|Dieg")
	void TryInteract();

	// Optional
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Player|Dieg")
	void PlayPickUpMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Player|Dieg")
	bool TryPickUpItem(ADieg_WorldItemActor* WorldItemActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Strong References")
	TObjectPtr<UDieg_TracerComponent> TracerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Strong References")
	TObjectPtr<UDieg_InventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Strong References")
	TObjectPtr<UDieg_InventoryInputHandler> InventoryInputHandlerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weak References")
	TWeakObjectPtr<AActor> HoveredActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Strong References")
	TSubclassOf<UDieg_InteractWidget> InteractWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Strong References")
	TObjectPtr<UDieg_InteractWidget> InteractWidget;
private:
	// Multiple Input Mapping Contexts required from UE 5.6 onwards.
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping Contexts")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	virtual void NotifyInteractionStarted_Implementation(UObject* Interactable) override;
	virtual void NotifyInteractionEnded_Implementation(UObject* Interactable) override;
	virtual void OnInteraction_Implementation(UObject* Interactable) override;
};
