// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Components/Dieg_InventoryInputHandler.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetComponent.h"
#include "Diegetic/Dieg_PlayerController.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/Actors/Dieg_Briefcase.h"
#include "Diegetic/Actors/Dieg_WorldItemActor.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"
#include "Diegetic/Components/Dieg_InventoryComponent.h"
#include "Diegetic/UObjects/Dieg_ItemInstance.h"
#include "Diegetic/Widgets/Dieg_Grid.h"
#include "Diegetic/Widgets/Dieg_Slot.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UDieg_InventoryInputHandler::UDieg_InventoryInputHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}


// Called when the game starts
void UDieg_InventoryInputHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwningPlayerController = UDieg_UtilityLibrary::GetOwningPlayerController(this);

	if (OwningPlayerController.IsValid())
	{
		InitializeImcSubsystem();

		// Bind functions to input actions delegates
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwningPlayerController.Get()->InputComponent))
		{
			EnhancedInputComponent->BindAction(ToggleInventoryIA, ETriggerEvent::Triggered, this, &ThisClass::ToggleInventory);
			
			EnhancedInputComponent->BindAction(DragIA, ETriggerEvent::Started, this, &ThisClass::TryDragItem);
			EnhancedInputComponent->BindAction(DragIA, ETriggerEvent::Canceled, this, &ThisClass::TryDropItem);
			EnhancedInputComponent->BindAction(DragIA, ETriggerEvent::Completed, this, &ThisClass::TryDropItem);
			
			EnhancedInputComponent->BindAction(RotateIA, ETriggerEvent::Triggered, this, &ThisClass::HandleInputRotateItem);
		}
	}
}

void UDieg_InventoryInputHandler::InitializeImcSubsystem()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwningPlayerController.Get()->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(DefaultIMCs, 0);
	}
}


void UDieg_InventoryInputHandler::ToggleInventory()
{
	if (bIsInventoryOpen)
	{
		bIsInventoryOpen = false;
		CloseUserInterface();
		OnInventoryClosed.Broadcast();
	}
	else
	{
		bIsInventoryOpen = true;
		OpenUserInterface();
		OnInventoryOpened.Broadcast();
	}
}

void UDieg_InventoryInputHandler::OpenUserInterface()
{
	if (OwningPlayerController.IsValid())
	{
		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetHideCursorDuringCapture(false);
		OwningPlayerController->SetInputMode(InputModeGameAndUI);
		OwningPlayerController->SetShowMouseCursor(true);
		OwningPlayerController->SetIgnoreLookInput(true);
		OwningPlayerController->SetIgnoreMoveInput(true);
	}
	
	if (!IsValid(BriefcaseActor) && IsValid(BriefCaseActorClass))
	{
		const FTransform SpawnTransform = FTransform(RelativeSpawnRotation, RelativeSpawnLocation);

		// Constructor
		BriefcaseActor = GetWorld()->SpawnActorDeferred<ADieg_Briefcase>(BriefCaseActorClass, SpawnTransform);

		if (IsValid(BriefcaseActor))
		{
			// Safe place to set values BEFORE PreInitializeComponents → PostInitializeComponents → OnConstruction → BeginPlay
			BriefcaseActor->AttachToActor(OwningPlayerController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
			BriefcaseActor->Initialize(OwningPlayerController->GetInventoryComponent());

			UGameplayStatics::FinishSpawningActor(BriefcaseActor, SpawnTransform);
		}
	}

	SetComponentTickEnabled(true);
}

void UDieg_InventoryInputHandler::CloseUserInterface()
{
	if (OwningPlayerController.IsValid())
	{
		OwningPlayerController->SetInputMode(FInputModeGameOnly());
		OwningPlayerController->SetShowMouseCursor(false);
		OwningPlayerController->ResetIgnoreInputFlags();
	}

	if (IsValid(BriefcaseActor))
	{
		BriefcaseActor->CloseBriefcase();
		BriefcaseActor = nullptr;

		OwningPlayerController->SetViewTargetWithBlend(OwningPlayerController.Get()->GetPawn(), 0.625f);
	}

	SetComponentTickEnabled(false);
}

bool UDieg_InventoryInputHandler::LineTraceFromMouse(FHitResult& HitResult) const
{
	FVector MouseWorldLocation = FVector::ZeroVector;
	FVector MouseWorldDirection = FVector::ZeroVector;
	OwningPlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	const FVector& TraceStart = OwningPlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector TraceEnd = TraceStart + MouseWorldDirection * TraceLength;

	// FString TempName = this->GetOwner()->GetActorNameOrLabel().Append(" " + this->GetName());
	// LOG_DOUBLE_S(0.5f, FColor::Emerald, "LineTraceFromMouse in {0}. MouseWorldDirection: {1}, TraceStart: {2}, TraceEnd: {3}"
	// 	,TempName, MouseWorldDirection, TraceStart, TraceEnd);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	if (bDebugDraw)
	{
		// Draw the trace line
		DrawDebugLine(
			GetWorld(),
			TraceStart,
			TraceEnd,
			bHit ? FColor::Red : FColor::Green,
			false, // not persistent
			0.5f,  // duration
			0,     // depth priority
			0.1f   // thickness
		);
	}
	
	if (bHit && bDebugDraw)
	{
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Blue, false, 2.0f);
	}

	return bHit;
}

void UDieg_InventoryInputHandler::StartHoverInventory(UDieg_3DInventoryComponent* NewHoveringInventory)
{
	HoveringInventoryComponent3D = NewHoveringInventory;

	// Bind events
	if (!HoveringInventoryComponent3D->On3DWidgetSlotHovered.IsAlreadyBound(this, &ThisClass::HandleHoveringInventorySlotHovered))
	{
		HoveringInventoryComponent3D->On3DWidgetSlotHovered.AddDynamic(this, &ThisClass::HandleHoveringInventorySlotHovered);
	}
	if (!HoveringInventoryComponent3D->On3DWidgetSlotUnHovered.IsAlreadyBound(this, &ThisClass::HandleHoveringInventorySlotUnHovered))
    {
    	HoveringInventoryComponent3D->On3DWidgetSlotUnHovered.AddDynamic(this, &ThisClass::HandleHoveringInventorySlotUnHovered);
    }

	OnHoverInventory.Broadcast(HoveringInventoryComponent3D.Get());

	if (IsDraggingItem())
	{
		ConnectItemToInventory();

		// Disable all currently placed items' collision.
		TArray<ADieg_WorldItemActor*> ItemsInInventory = HoveringInventoryComponent3D->GetItems();
		for (const TWeakObjectPtr<ADieg_WorldItemActor>& ItemActor : ItemsInInventory)
		{
			ItemActor->SetActorEnableCollision(false);
		}

		// We don't want now the mouse to change the state so lock the appearance
		HoveringInventoryComponent3D->GetGridWidget()->ModifyAllSlotsAppearance(true,true, EDieg_SlotStatus::None);

		OnDragHoverInventory.Broadcast(this, HoveringInventoryComponent3D.Get(), DraggingItem.Get(), RelativeCoordinates);
	}
	else
	{
		// Entered a new inventory widget, clear/empty any previous slot visual state.
		HoveringInventoryComponent3D->GetGridWidget()->ModifyAllSlotsAppearance(false,true, EDieg_SlotStatus::None);
	}
}

void UDieg_InventoryInputHandler::StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory)
{
	//HoveringInventoryComponent3D.Reset();
	// Unbind events
	if (HoveringInventoryComponent3D->On3DWidgetSlotHovered.IsAlreadyBound(this, &ThisClass::HandleHoveringInventorySlotHovered))
	{
		HoveringInventoryComponent3D->On3DWidgetSlotHovered.RemoveDynamic(this, &ThisClass::HandleHoveringInventorySlotHovered);
	}
	if (HoveringInventoryComponent3D->On3DWidgetSlotUnHovered.IsAlreadyBound(this, &ThisClass::HandleHoveringInventorySlotUnHovered))
	{
		HoveringInventoryComponent3D->On3DWidgetSlotUnHovered.RemoveDynamic(this, &ThisClass::HandleHoveringInventorySlotUnHovered);
	}

	OnUnHoverInventory.Broadcast(HoveringInventoryComponent3D.Get());

	if (IsDraggingItem())
	{
		// Reset old inventory widget's slots
		const TArray<FIntPoint> Empty;
		HoveringInventoryComponent3D->GetGridWidget()->UpdateHoveringSlots(Empty,EDieg_SlotStatus::None, EDieg_SlotStatus::None);

		// Reset item actors rotation to flat, since inventory no longer exists and item is in world
		DraggingItem->SetActorRotation(FRotator::ZeroRotator);

		// Disable all currently placed items' collision.
		TArray<ADieg_WorldItemActor*> ItemsInInventory = HoveringInventoryComponent3D->GetItems();
		for (const TWeakObjectPtr<ADieg_WorldItemActor>& ItemActor : ItemsInInventory)
		{
			ItemActor->SetActorEnableCollision(true);
		}

		OnDragUnHoverInventory.Broadcast(this, HoveringInventoryComponent3D.Get(), DraggingItem.Get(), RelativeCoordinates);
	}
	
}

void UDieg_InventoryInputHandler::StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem)
{
	if (HoveringItem.IsValid())
	{
		StopHoverItem(HoveringItem.Get());
	}
	
	HoveringItem = NewHoveringWorldItem;

	if (HoveringInventoryComponent3D.IsValid())
	{
		HoveringInventoryComponent3D->OnHoverItem.Broadcast(this, HoveringItem.Get());
	}
	
	OnHoverItem.Broadcast(this, HoveringInventoryComponent3D.Get(), HoveringItem.Get(), IsInInventory());
}

void UDieg_InventoryInputHandler::StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem)
{
	UDieg_3DInventoryComponent* OwningInventory = nullptr;
	bool ItemIsInInventory = IsItemInInventory(HoveringWorldItem,OwningInventory);
	OnUnHoverItem.Broadcast(this, HoveringInventoryComponent3D.Get(), HoveringWorldItem, ItemIsInInventory);

	if (ItemIsInInventory && HoveringInventoryComponent3D.IsValid())
	{
		HoveringInventoryComponent3D.Get()->OnUnHoverItem.Broadcast(this, HoveringWorldItem);
	}
	
	HoveringItem->Reset();
}

void UDieg_InventoryInputHandler::HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit)
{
	AActor* HitActor = HitResult.GetActor();
	
	if (HitActor != LastHitActor)
	{
		LastHitActor = HitActor;
		FString TempName = this->GetOwner()->GetActorNameOrLabel().Append(" " + this->GetName());

		// Find if it is hovering an inventory
		UDieg_3DInventoryComponent* NewHoveringInventory = HitActor->FindComponentByClass<UDieg_3DInventoryComponent>();
		if (!NewHoveringInventory && HitActor->GetOwner())
		{
			NewHoveringInventory = HitActor->GetOwner()->FindComponentByClass<UDieg_3DInventoryComponent>();

			if (bDebugLogs)
				UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. Hovering Inventory via Item"), FColor::Emerald, TempName);
		}

		if (IsValid(NewHoveringInventory))
		{
			if (NewHoveringInventory != HoveringInventoryComponent3D)
			{
				if (HoveringInventoryComponent3D.IsValid())
				{
					StopHoverInventory(HoveringInventoryComponent3D.Get());
					if (bDebugLogs)
						UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. SWAP. Stop Hovering Inventory"), FColor::Emerald, TempName);
					HoveringInventoryComponent3D.Reset();
				}
				
				StartHoverInventory(NewHoveringInventory);
				if (bDebugLogs)
					UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. Hovering Inventory"), FColor::Emerald, TempName);
			}
		}
		else
		{
			if (HoveringInventoryComponent3D.IsValid())
			{
				StopHoverInventory(HoveringInventoryComponent3D.Get());
				if (bDebugLogs)
					UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. Stop Hovering Inventory"), FColor::Emerald, TempName);
				HoveringInventoryComponent3D.Reset();
			}
		}

		if (ADieg_WorldItemActor* NewHoveringItem = Cast<ADieg_WorldItemActor>(HitActor))
		{
			if (HoveringItem.IsValid())
			{
				StopHoverItem(HoveringItem.Get());
				if (bDebugLogs)
					UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. SWAP. Stop Hover Item"), FColor::Emerald, TempName);
				HoveringItem.Reset();
			}
			
			if (NewHoveringItem != HoveringItem)
			{
				StartHoverItem(NewHoveringItem);
				if (bDebugLogs)
					UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. Start Hover Item"), FColor::Emerald, TempName);
			}
		}
		else
		{
			if (HoveringItem.IsValid())
			{
				StopHoverItem(HoveringItem.Get());
				if (bDebugLogs)
					UPlugInv_DoubleLogger::Log(5.0f, TEXT("HandleTraceHit in {0}. Stop Hover Item"), FColor::Emerald, TempName);
				HoveringItem.Reset();
			}
		}
	}
	
	if (IsDraggingItem())
	{
		const FVector ImpactPoint = HitResult.ImpactPoint;

		// if (bDebugLogs)
		// 	UPlugInv_DoubleLogger::Log(1.0f, TEXT("IsDraggingItem, ImpactPoint: {0}. Tick"), FColor::Emerald, ImpactPoint);
		
		DraggingItem->SetActorLocation(ImpactPoint, false, nullptr, ETeleportType::None);

		if (IsInInventory())
		{
			
		}
	}
}

bool UDieg_InventoryInputHandler::IsItemInInventory(const ADieg_WorldItemActor* ItemIn,
	UDieg_3DInventoryComponent*& Inventory3DOut)
{
	if (!ItemIn->GetOwner())
	{
		return false;
	}
	UDieg_3DInventoryComponent* Inventory3D = ItemIn->GetOwner()->FindComponentByClass<UDieg_3DInventoryComponent>();
	if (IsValid(Inventory3D))
	{
		Inventory3DOut = Inventory3D;
		return true;
	}

	return false;
}

void UDieg_InventoryInputHandler::TryDragItem()
{
	if (HoveringItem.IsValid())
	{
		if (bDebugLogs)
			UPlugInv_DoubleLogger::Log("TryDragItem");
		DraggingItem = HoveringItem;

		StartDraggingItem();
	}
}

void UDieg_InventoryInputHandler::TryDropItem()
{
	StopDraggingItem();
}

void UDieg_InventoryInputHandler::HandleInputRotateItem()
{
	TryRotateItem();
}

void UDieg_InventoryInputHandler::TryRotateItem()
{
	if (IsInInventory() && IsDraggingItem())
	{
		RotateItem();
	}
}

void UDieg_InventoryInputHandler::InternalHandleInventorySlotHover(UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot)
{
	if (IsDraggingItem())
	{
		CurrentMouseCoordinates = Dieg_Slot->GetCoordinatesInGrid();
		const TArray<FIntPoint> RelevantCoordinates = GetRelevantCoordinates();
		Dieg_3DInventoryComponent->GetGridWidget()->UpdateHoveringSlots(RelevantCoordinates, EDieg_SlotStatus::Occupied, EDieg_SlotStatus::None);

		if (bDebugLogs)
			UPlugInv_DoubleLogger::Log(5.0f, TEXT("HoverSlotHandler: Checking if slots are available {0}"), FColor::Emerald, RelevantCoordinates);
		const bool SlotsAreValid = Dieg_3DInventoryComponent->GetInventoryComponent()->AreSlotsAvailableSimple(RelevantCoordinates);
		if (SlotsAreValid)
		{
			ValidCoordinates = CurrentMouseCoordinates - GetRotatedGrabCoordinates(false);
			ValidRotation = CurrentRotation;
			ValidInventory3D = HoveringInventoryComponent3D;

			if (bDebugLogs)
				UPlugInv_DoubleLogger::Log(5.0f, TEXT("HoverSlotHandler: Valid"), FColor::Emerald);
		}
		else
		{
			if (bDebugLogs)
				UPlugInv_DoubleLogger::Log(5.0f, TEXT("HoverSlotHandler: Not Available"), FColor::Emerald);
		}

		OnUpdateDragSlot.Broadcast(SlotsAreValid, this, DraggingItem.Get(), RelevantCoordinates, CurrentMouseCoordinates - GetRotatedGrabCoordinates(false), ValidRotation);
	}
	else
	{
		OnStartHoverSlot.Broadcast(Dieg_3DInventoryComponent, Dieg_Slot);
	}
	
	CurrentMouseSlot = Dieg_Slot;
}

void UDieg_InventoryInputHandler::HandleHoveringInventorySlotHovered(const FGeometry& Geometry,
                                                                     const FPointerEvent& PointerEvent, UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot)
{
	InternalHandleInventorySlotHover(Dieg_3DInventoryComponent, Dieg_Slot);
}

void UDieg_InventoryInputHandler::HandleHoveringInventorySlotUnHovered(const FPointerEvent& PointerEvent,
                                                                       UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot)
{
	OnStartUnHoverSlot.Broadcast(Dieg_3DInventoryComponent, Dieg_Slot);
}

void UDieg_InventoryInputHandler::StartDraggingItem()
{
	DraggingItem->BindEventsToHandler(this);
	
	RelativeCoordinates = GetGrabCoordinates();

	if (bDebugLogs)
		UPlugInv_DoubleLogger::Log(5.0f, TEXT("Grab coordinates are: {0}. StartDraggingItem"), FColor::Emerald, RelativeCoordinates);
	
	DraggingItem->SetActorEnableCollision(false);
	
	// It's starting dragging from world, not inventory here
	const FIntPoint RotatedGrabPoint = GetRotatedGrabCoordinates(false);
	UDieg_3DInventoryComponent* OwningInventory = nullptr;
	if (IsItemInInventory(DraggingItem.Get(),OwningInventory))
	{
		// It's starting dragging from inventory
		OwningInventory->RemoveItemFromInventory(DraggingItem.Get());
		
		ValidRotation = CurrentRotation = DraggingItem->GetCurrentRotation();
		ValidCoordinates = DraggingItem->GetCoordinates();
		// Due to mouse shenanigans (not the same clicking the same coordinate in top left corner than bottom right + other considerations)
		CurrentMouseCoordinates = ValidCoordinates + RotatedGrabPoint;
		ValidInventory3D = Default3DInventory = OwningInventory;

		if (bDebugLogs)
			UPlugInv_DoubleLogger::Log(5.0f, TEXT("CurrentMouseCoordinates {0} = ValidCoordinates {1} + RotatedGrabPoint {2}"),
				FColor::Emerald, CurrentMouseCoordinates, ValidCoordinates, RotatedGrabPoint);

		UDieg_Grid* GridWidget = OwningInventory->GetGridWidget();
		GridWidget->ModifyAllSlotsAppearance(true, true, EDieg_SlotStatus::None);
		TArray<FIntPoint> RelevantCoordinates = GetRelevantCoordinates();
		GridWidget->UpdateHoveringSlots(RelevantCoordinates, EDieg_SlotStatus::Occupied, EDieg_SlotStatus::None);

		// Later for when we drop the item, we reset the grid, still have it locked to native mouse events visuals and want to update its hover state visually.
		// CurrentMouseSlot = GridWidget->GetSlotMap().FindRef(CurrentMouseCoordinates);
		UDieg_Slot* RawSlotPtr = nullptr;
		if (const bool SlotExists = GetCurrentSlot(RawSlotPtr))
		{
			CurrentMouseSlot = RawSlotPtr;
		}
		else
		{
			CurrentMouseSlot = nullptr;
		}
		
		TArray<ADieg_WorldItemActor*>Items = OwningInventory->GetItems();
		for (ADieg_WorldItemActor* ItemActor : Items)
		{
			ItemActor->SetActorEnableCollision(false);
		}

		OnStartDragInventory.Broadcast(this, DraggingItem.Get(), RotatedGrabPoint, ValidCoordinates);
	}
	else
	{
		
		ValidWorldLocation = DraggingItem.Get()->GetActorLocation();
		OnStartDragWorld.Broadcast(this, DraggingItem.Get(), RotatedGrabPoint, ValidWorldLocation);
	}
}

void UDieg_InventoryInputHandler::StopDraggingItem()
{
	if (bDebugLogs)
		UPlugInv_DoubleLogger::Log("StopDraggingItem");

	if (IsInInventory())
	{
		HoveringInventoryComponent3D->GetGridWidget()->ModifyAllSlotsAppearance(false, true, EDieg_SlotStatus::None);

		UDieg_Slot* RawPtr = nullptr;
		if (GetCurrentSlot(RawPtr))
		{
			CurrentMouseSlot = RawPtr;
			CurrentMouseSlot->SetStatusAndColor(EDieg_SlotStatus::Hover);
		}

		if (DraggingItem.IsValid())
		{
			TArray<FIntPoint> RelevantCoordinates = GetRelevantCoordinates();
			TArray<FIntPoint> RelevantRootSlots = HoveringInventoryComponent3D->GetInventoryComponent()->GetRelevantItems(
				RelevantCoordinates, DraggingItem->GetItemInstance());

			bool IsNotEmpty = false;
			if (!RelevantRootSlots.IsEmpty())
			{
				// Items in grid that our dragging item can merge with.
				IsNotEmpty= MergeItems(RelevantRootSlots);
				if (IsNotEmpty == false)
				{
					// Destroy item actor since its quantities have been all added.
					OnConsumedItemInMerge.Broadcast(this, HoveringInventoryComponent3D.Get(), DraggingItem.Get());
				}
			}
			
			if (ValidCoordinates != FIntPoint(-1, -1))
			{
				if (DraggingItem->GetCoordinates() != ValidCoordinates || DraggingItem->GetCurrentRotation() != ValidRotation || Default3DInventory != ValidInventory3D)
				{
					// It has moved
					DraggingItem->SetCoordinates(ValidCoordinates);
					DraggingItem->SetCurrentRotation(ValidRotation);
					ValidInventory3D->AddItemToInventorySlot(DraggingItem.Get(), ValidCoordinates, ValidRotation);
					OnDropInInventory.Broadcast(this, ValidInventory3D.Get(),
						DraggingItem.Get(), ValidCoordinates, ValidRotation);
				}
				else
				{
					// It hasn't moved, reset to its last valid data
					ValidInventory3D->AddItemToInventorySlot(DraggingItem.Get(), ValidCoordinates, ValidRotation);
					OnResetDragInventory.Broadcast(this, ValidInventory3D.Get(),
						DraggingItem.Get(), DraggingItem->GetCoordinates(), DraggingItem->GetCurrentRotation());
				}
			}
			else
			{
				// No valid place has been found
				OnResetDragWorld.Broadcast(this, DraggingItem.Get(), ValidWorldLocation);
			}
		}
		
		TArray<ADieg_WorldItemActor*> Items = HoveringInventoryComponent3D->GetItems();
		for (ADieg_WorldItemActor* ItemActor : Items)
		{
			ItemActor->SetActorEnableCollision(true);
		}
	}
	else if (DraggingItem.IsValid())
	{
		DisconnectItemToInventory();
		OnDropWorld.Broadcast(this, DraggingItem.Get(), DraggingItem->GetActorLocation());
	}
	
	if (DraggingItem.IsValid())
	{
		DraggingItem->SetActorEnableCollision(true);
		DraggingItem->UnBindEventsFromHandler(this);
		ValidCoordinates = FIntPoint(-1,-1);
		CurrentRotation = ValidRotation = 0.0f;
		Default3DInventory.Reset();
	}

	DraggingItem.Reset();
}

bool UDieg_InventoryInputHandler::MergeItems(const TArray<FIntPoint>& RootSlotCoordinates)
{
	if (!DraggingItem.IsValid())
	{
		return false;
	}

	int32 Quantity = 0;
	int32 OldQuantity = Quantity = DraggingItem->GetItemInstance()->GetQuantity();
	
	TArray<ADieg_WorldItemActor*> Items3D = HoveringInventoryComponent3D->GetItems();
	// TArray<FDieg_InventorySlot*> InventorySlots = HoveringInventoryComponent3D->GetInventoryComponent()->GetSlotsMutable();
	TArray<FDieg_InventorySlot*> InventoryRootSlots = HoveringInventoryComponent3D->GetInventoryComponent()->GetRootSlotsMutable();
	
	TArray<AActor*> MergedActors;
	for (FIntPoint RootCoordinate : RootSlotCoordinates)
	{
		if (ADieg_WorldItemActor** CurrentItem3D = Items3D.FindByPredicate(
			[&](const TWeakObjectPtr<ADieg_WorldItemActor>& Item)
			{
				return Item.IsValid() &&
					Item->GetCoordinates() == RootCoordinate;
			}))
		{
			MergedActors.Add(*CurrentItem3D);

			if (FDieg_InventorySlot** FoundSlot = InventoryRootSlots.FindByPredicate(
			[&](FDieg_InventorySlot* S)
			{
				return S && S->Coordinates == RootCoordinate;
			}))
			{
				FDieg_InventorySlot* CurrentInventorySlot = *FoundSlot;

				const int32 Added = HoveringInventoryComponent3D->GetInventoryComponent()
										->AddQuantityToSlot(CurrentInventorySlot->ItemInstance, Quantity);

				(*CurrentItem3D)->SetQuantity(CurrentInventorySlot->ItemInstance->GetQuantity());
				Quantity -= Added;

				if (Quantity <= 0)
				{
					OnMergeItem.Broadcast(this, DraggingItem.Get(), MergedActors, OldQuantity, Quantity);
					return false;
				}
			}
		}
	}
	DraggingItem->SetQuantity(DraggingItem->GetItemInstance()->GetQuantity());
	OnMergeItem.Broadcast(this, DraggingItem.Get(), MergedActors, OldQuantity, Quantity);

	return true;
}

void UDieg_InventoryInputHandler::RotateItem()
{
	if (CurrentRotation + 90.0f <= 180.0f)
	{
		CurrentRotation += 90.0f;
	}
	else
	{
		CurrentRotation = -90.0f;
	}
	const FIntPoint RotatedGrabPoint = GetRotatedGrabCoordinates(false);
	OnRotateItem.Broadcast(this, DraggingItem.Get(), CurrentRotation, RotatedGrabPoint);

	UDieg_Slot* Slot = nullptr;
	if (GetCurrentSlot(Slot))
	{
		// This is easier than setting logic, just consider the item has been removed
		InternalHandleInventorySlotHover(HoveringInventoryComponent3D.Get(), Slot);
	}
}

void UDieg_InventoryInputHandler::ConnectItemToInventory() const
{
	UWidgetComponent* Comp = HoveringInventoryComponent3D->GetWidgetComponent().Get();
	const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative, false);
	DraggingItem->AttachToComponent(Comp, AttachmentTransformRules, NAME_None);
	DraggingItem->SetOwner(HoveringInventoryComponent3D->GetOwner());
}

void UDieg_InventoryInputHandler::DisconnectItemToInventory() const
{
	DraggingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DraggingItem->SetOwner(nullptr);
}


FIntPoint UDieg_InventoryInputHandler::GetGrabCoordinates() const
{
	// Default return value (no valid coordinates).
	FIntPoint FinalResult = FIntPoint::ZeroValue;

	// If no item is currently being dragged, we can't compute coordinates.
	if (!DraggingItem.IsValid())
	{
		return FinalResult;
	}

	// Run a line trace from the mouse to find where the user is pointing in 3D space.
	FHitResult HitResult;
	if (!LineTraceFromMouse(HitResult))
	{
		return FinalResult;
	}

	// Calculate the exact impact point (slightly offset by the surface normal).
	const FVector ImpactPoint = HitResult.ImpactPoint - HitResult.Normal;
	const TWeakObjectPtr<UPrimitiveComponent> HitComponent = HitResult.Component;

	// Get the definition of the item being dragged.
	const FDieg_ItemDefinition& DraggingItemDefinition = DraggingItem->GetItemInstance()->GetItemDefinition();

	// NOTE:
	// Even though items can be rotated, the local space of the PrimitiveShapeComponent 
	// (the hit component) is always considered "zero rotation".
	// Because of that, we do NOT need to rotate the shape here.
	// Rotation adjustments will be applied later in the pipeline when needed.

	// Find the size (span) of the item's shape in grid units.
	const FIntPoint ShapeSpan = UDieg_UtilityLibrary::GetShapeSpan(DraggingItemDefinition.DefaultShape);
	const int32 XSpan = ShapeSpan.X;
	const int32 YSpan = ShapeSpan.Y;

	// Convert the 3D hit point into local component space.
	const FVector LocalPosition3D = HitComponent->GetComponentToWorld().InverseTransformPosition(ImpactPoint);

	// We only care about X and Y for 2D grid placement.
	const FVector2D LocalPosition2D(LocalPosition3D.X, LocalPosition3D.Y);

	// Get the grid size (how big each slot is in 3D space).
	const UDieg_Slot* DefaultSlot = GetDefault<UDieg_Slot>();
	const float GridSize3D = DefaultSlot->GetGridSize3D();

	// Convert the local position into "grid space" (floating-point).
	const FVector2D GridCoordinates = LocalPosition2D / GridSize3D;

	// Map grid coordinates into shape coordinates.
	// NOTE: XSpan is used here to "invert" the Y axis so placement is aligned correctly.
	const FVector2D ShapeCoordinates(XSpan - GridCoordinates.Y, GridCoordinates.X);

	// Clamp the result to valid shape bounds.
	// We floor the values to integer cell indices, then clamp them so they stay inside the shape.
	const FVector2D ClampedCoordinates(
		FMath::Clamp(FMath::Floor(ShapeCoordinates.X), 0, XSpan - 1),
		FMath::Clamp(FMath::Floor(ShapeCoordinates.Y), 0, YSpan - 1)
	);

	// Final integer result (grab coordinates within the item's shape).
	FinalResult = FIntPoint(ClampedCoordinates.X, ClampedCoordinates.Y);

	return FinalResult;
}

FIntPoint UDieg_InventoryInputHandler::GetRotatedGrabCoordinates(bool UseValidated) const
{
	// Default return value (no valid coordinates).
	FIntPoint FinalResult = FIntPoint::ZeroValue;

	// If no item is currently being dragged, we can't compute coordinates.
	if (!DraggingItem.IsValid())
	{
		return FinalResult;
	}

	// Get the definition of the item being dragged.
	const FDieg_ItemDefinition& DraggingItemDefinition = DraggingItem->GetItemInstance()->GetItemDefinition();
	int32 Index = DraggingItemDefinition.DefaultShape.Find(RelativeCoordinates);

	float RotationToUse = UseValidated ? ValidRotation : CurrentRotation;
	FIntPoint RotatedRootOut;
	TArray<FIntPoint> RotatedShape = UDieg_UtilityLibrary::Rotate2DArrayWithRoot(DraggingItemDefinition.DefaultShape, RotationToUse, RelativeCoordinates,RotatedRootOut);

	// It should be the same index as the default shape
	if (RotatedShape.IsValidIndex(Index))
	{
		FinalResult = RotatedShape[Index];
	}
	
	// Extra check
	if (FinalResult != RotatedRootOut)
	{
		// Something or calculation are wrong
		if (bDebugLogs)
			UPlugInv_DoubleLogger::Log("FinalResult != RotatedRootOut, in GetRotatedGrabCoordinates()");
	}
	
	return FinalResult;
}

TArray<FIntPoint> UDieg_InventoryInputHandler::GetRelevantCoordinates() const
{
	TArray<FIntPoint> FinalResult = TArray<FIntPoint>();
	// These are the coordinates we are moving within, are they valid
	if (!DraggingItem.IsValid())
	{
		return FinalResult;
	}

	// In here technically the hovering and owning inventory should be the same
	const FIntPoint ActualCoordinates = CurrentMouseCoordinates - GetRotatedGrabCoordinates(false);
	const FDieg_ItemDefinition& DraggingItemDefinition = DraggingItem->GetItemInstance()->GetItemDefinition();

	FIntPoint RotatedRootOut;
	return HoveringInventoryComponent3D->GetInventoryComponent()->GetRelevantCoordinates(ActualCoordinates,
		DraggingItemDefinition.DefaultShape, DraggingItemDefinition.DefaultShapeRoot, CurrentRotation,RotatedRootOut);
}

bool UDieg_InventoryInputHandler::GetCurrentSlot(UDieg_Slot*& SlotOut) const
{
	if (HoveringInventoryComponent3D.IsValid())
	{
		if (UDieg_Slot* FoundSlot = HoveringInventoryComponent3D->GetGridWidget()->GetSlotMap().FindRef(CurrentMouseCoordinates))
		{
			SlotOut = FoundSlot;
			return true;
		}
	}

	return false;
}

// Called every frame
void UDieg_InventoryInputHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsInventoryOpen && OwningPlayerController.IsValid())
	{
		FHitResult HitResult;
		if (LineTraceFromMouse(HitResult))
		{
			HandleTraceHit(HitResult, true);
		}
	}
}

