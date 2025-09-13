// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Dieg_PlayerController.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Diegetic/Actors/Dieg_WorldItemActor.h"
#include "Diegetic/Components/Dieg_InventoryComponent.h"
#include "Diegetic/Components/Dieg_InventoryInputHandler.h"
#include "Diegetic/Components/Dieg_TracerComponent.h"
#include "Diegetic/Interfaces/Dieg_Interactable.h"
#include "Diegetic/Widgets/Dieg_InteractWidget.h"

ADieg_PlayerController::ADieg_PlayerController()
{
	TracerComponent = CreateDefaultSubobject<UDieg_TracerComponent>(TEXT("Tracer Component"));
	InventoryComponent = CreateDefaultSubobject<UDieg_InventoryComponent>(TEXT("Inventory Component"));
	InventoryInputHandlerComponent = CreateDefaultSubobject<UDieg_InventoryInputHandler>(TEXT("Inventory Input Handler Component"));
}

void ADieg_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create interact widget
	if (InteractWidget = CreateWidget<UDieg_InteractWidget>(this, InteractWidgetClass); IsValid(InteractWidget))
	{
		InteractWidget->AddToViewport();
	}

	// Bind handlers to delegates
	if (IsValid(TracerComponent))
	{
		if (!TracerComponent->OnActorInTrace.IsAlreadyBound(this, &ThisClass::HandleActorInTrace))
		{
			TracerComponent->OnActorInTrace.AddDynamic(this, &ThisClass::HandleActorInTrace);
		}
		if (!TracerComponent->OnActorOutTrace.IsAlreadyBound(this, &ThisClass::HandleActorOutTrace))
		{
			TracerComponent->OnActorOutTrace.AddDynamic(this, &ThisClass::HandleActorOutTrace);
		}
	}

	if (IsValid(InventoryInputHandlerComponent))
	{
		if (!InventoryInputHandlerComponent->OnInventoryOpened.IsAlreadyBound(this, &ThisClass::HandleOnInventoryOpened))
		{
			InventoryInputHandlerComponent->OnInventoryOpened.AddDynamic(this, &ThisClass::HandleOnInventoryOpened);
		}
		if (!InventoryInputHandlerComponent->OnInventoryClosed.IsAlreadyBound(this, &ThisClass::HandleOnInventoryClosed))
		{
			InventoryInputHandlerComponent->OnInventoryClosed.AddDynamic(this, &ThisClass::HandleOnInventoryClosed);
		}
	}
}

void ADieg_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add mapping and initialize input mapping contexts
	InitializeImcSubsystem();

	// Bind functions to input actions delegates
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PrimaryInteractIA, ETriggerEvent::Triggered, this, &ADieg_PlayerController::TryInteract);
		// EnhancedInputComponent->BindAction(ToggleInventoryIA, ETriggerEvent::Triggered, this, &ADieg_PlayerController::TryToggleInventory);
	}
}

void ADieg_PlayerController::InitializeImcSubsystem()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->ClearAllMappings();
		for (const UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void ADieg_PlayerController::HandleOnInventoryClosed_Implementation()
{
	bInventoryOpened = false;
}

void ADieg_PlayerController::HandleOnInventoryOpened_Implementation()
{
	bInventoryOpened = true;
}

void ADieg_PlayerController::TryToggleInventory_Implementation()
{
}

void ADieg_PlayerController::TryInteract_Implementation()
{
	// Early return if invalid members
	if (!HoveredActor.IsValid() || !IsValid(InventoryComponent))
	{
		return;
	}

	// Interface interactable execution
	if (HoveredActor.Get()->Implements<UDieg_Interactable>())
	{
		IDieg_Interactable::Execute_OnInteract(HoveredActor.Get(), this);
		IDieg_Interactor::Execute_OnInteraction(this, HoveredActor.Get());
	}
}

void ADieg_PlayerController::PlayPickUpMontage_Implementation()
{
}

bool ADieg_PlayerController::TryPickUpItem_Implementation(ADieg_WorldItemActor* WorldItemActor)
{
	int32 Remaining = INT32_MAX;
	const bool bAddedItemToInventory = InventoryComponent->TryAddItem(WorldItemActor->GetItemInstance(), Remaining);
	if (bAddedItemToInventory && Remaining <= 0)
	{
		WorldItemActor->Destroy();
	}
	return bAddedItemToInventory;
}


void ADieg_PlayerController::NotifyInteractionStarted_Implementation(UObject* Interactable)
{
	IDieg_Interactor::NotifyInteractionStarted_Implementation(Interactable);
	UPlugInv_DoubleLogger::Log("ADieg_PlayerController::NotifyInteractionStarted_Implementation");
}

void ADieg_PlayerController::NotifyInteractionEnded_Implementation(UObject* Interactable)
{
	IDieg_Interactor::NotifyInteractionEnded_Implementation(Interactable);
	UPlugInv_DoubleLogger::Log("ADieg_PlayerController::NotifyInteractionEnded_Implementation");
}

void ADieg_PlayerController::OnInteraction_Implementation(UObject* Interactable)
{
	IDieg_Interactor::OnInteraction_Implementation(Interactable);

	if (ADieg_WorldItemActor* WorldItemActor = Cast<ADieg_WorldItemActor>(Interactable))
	{
		// Play montage
		// Inventory component pick up
		TryPickUpItem(WorldItemActor);
	}
}

void ADieg_PlayerController::HandleActorInTrace(AActor* CurrentActor, AActor* PreviousActor,
                                                TEnumAsByte<ECollisionChannel> TraceChannel)
{
	HoveredActor = CurrentActor;

	OnHandleTraceActorIn.Broadcast(CurrentActor, PreviousActor, TraceChannel);
}

void ADieg_PlayerController::HandleActorOutTrace(AActor* PreviousActor, TEnumAsByte<ECollisionChannel> TraceChannel)
{
	if (HoveredActor == PreviousActor)
	{
		HoveredActor = nullptr;
		HoveredActor.Reset();
	}

	OnHandleTraceActorOut.Broadcast(PreviousActor, TraceChannel);
}

