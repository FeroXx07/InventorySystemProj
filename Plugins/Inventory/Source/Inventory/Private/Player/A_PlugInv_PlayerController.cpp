// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/A_PlugInv_PlayerController.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Interaction/I_PlugInv_Highlightable.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Items/Components/AC_PlugInv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/UW_PlugInv_HUDWidget.h"

APlugInv_PlayerController::APlugInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.0;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void APlugInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
}

void APlugInv_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;

	InventoryComponent->ToggleInventoryMenu();
}

void APlugInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogInventory, Log, TEXT("BeginPlay for PlugInv_PlayerController"));

	InventoryComponent = FindComponentByClass<UPlugInv_InventoryComponent>();

	CreateHUDWidget();
}

void APlugInv_PlayerController::InitIMCSubsystem()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->ClearAllMappings();
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void APlugInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InitIMCSubsystem();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractIA, ETriggerEvent::Triggered, this, &APlugInv_PlayerController::PrimaryInteract);
	EnhancedInputComponent->BindAction(ToggleInventoryIA, ETriggerEvent::Triggered, this, &APlugInv_PlayerController::ToggleInventory);
}

void APlugInv_PlayerController::PrimaryInteract()
{
	UPlugInv_DoubleLogger::Log("Primary interact for APlugInv_PlayerController");

	if (!CurrentTraceActor.IsValid())
		return;

	UPlugInv_ItemComponent* ItemComponent = CurrentTraceActor->FindComponentByClass<UPlugInv_ItemComponent>();
	if (!IsValid(ItemComponent) || !InventoryComponent.IsValid())
		return;

	InventoryComponent->TryAddItem(ItemComponent);
}

void APlugInv_PlayerController::CreateHUDWidget()
{
	// To avoid instancing widgets on server or non-local player on clients.
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UPlugInv_HUDWidget>(this, HUDWidgetClass);

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void APlugInv_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize/2.f;
	FVector TraceStart;
	FVector TraceForward;

	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, TraceForward)) return;

	const FVector TraceEnd = TraceStart + TraceForward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	PreviousTraceActor = CurrentTraceActor;
	CurrentTraceActor = HitResult.GetActor();

	if (!CurrentTraceActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (CurrentTraceActor == PreviousTraceActor) return;

	if (CurrentTraceActor.IsValid())
	{
		if (UActorComponent* Highlightable = CurrentTraceActor->FindComponentByInterface(UPlugInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			// https://dev.epicgames.com/documentation/en-us/unreal-engine/interfaces-in-unreal-engine
			IPlugInv_Highlightable::Execute_Highlight(Highlightable);
		}
		
		// UE_LOG(LogTemp, Warning, TEXT("Started tracing a new actor"))
		UPlugInv_ItemComponent* ItemComponent = CurrentTraceActor->FindComponentByClass<UPlugInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
	}

	if (PreviousTraceActor.IsValid())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Stopped tracing last actor"))
		if (UActorComponent* Highlightable = PreviousTraceActor->FindComponentByInterface(UPlugInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			// https://dev.epicgames.com/documentation/en-us/unreal-engine/interfaces-in-unreal-engine
			IPlugInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}
}
