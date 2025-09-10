// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Components/Dieg_InventoryInputHandler.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Diegetic/Dieg_PlayerController.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/Actors/Dieg_Briefcase.h"
#include "Diegetic/Actors/Dieg_WorldItemActor.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"
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
	}
	else
	{
		bIsInventoryOpen = true;
		OpenUserInterface();
	}
}

void UDieg_InventoryInputHandler::OpenUserInterface()
{
	if (OwningPlayerController.IsValid())
	{
		OwningPlayerController->SetInputMode(FInputModeGameAndUI());
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
}

void UDieg_InventoryInputHandler::StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory)
{
	//HoveringInventoryComponent3D.Reset();
}

void UDieg_InventoryInputHandler::StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem)
{
	HoveringItem = NewHoveringWorldItem;
}

void UDieg_InventoryInputHandler::StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem)
{
	HoveringItem->Reset();
}


void UDieg_InventoryInputHandler::HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit)
{
	AActor* HitActor = HitResult.GetActor();
	
	if (HitActor == LastHitActor)
	{
		return;
	}
	
	LastHitActor = HitActor;
	FString TempName = this->GetOwner()->GetActorNameOrLabel().Append(" " + this->GetName());

	// Find if it is hovering an inventory
	UDieg_3DInventoryComponent* NewHoveringInventory = HitActor->FindComponentByClass<UDieg_3DInventoryComponent>();
	if (!NewHoveringInventory && HitActor->GetOwner())
	{
		NewHoveringInventory = HitActor->GetOwner()->FindComponentByClass<UDieg_3DInventoryComponent>();

		LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. Hovering Inventory via Item",TempName);
	}

	if (IsValid(NewHoveringInventory))
	{
		if (NewHoveringInventory != HoveringInventoryComponent3D)
		{
			if (HoveringInventoryComponent3D.IsValid())
			{
				StopHoverInventory(HoveringInventoryComponent3D.Get());
				LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. SWAP. Stop Hovering Inventory",TempName);
				HoveringInventoryComponent3D.Reset();
			}
			
			StartHoverInventory(NewHoveringInventory);
			LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. Hovering Inventory",TempName);
		}
	}
	else
	{
		if (HoveringInventoryComponent3D.IsValid())
		{
			StopHoverInventory(HoveringInventoryComponent3D.Get());
			LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. Stop Hovering Inventory",TempName);
			HoveringInventoryComponent3D.Reset();
		}
	}

	if (ADieg_WorldItemActor* NewHoveringItem = Cast<ADieg_WorldItemActor>(HitActor))
	{
		if (HoveringItem.IsValid())
		{
			StopHoverItem(HoveringItem.Get());
			LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. SWAP. Stop Hover Item",TempName);
			HoveringItem.Reset();
		}
		
		if (NewHoveringItem != HoveringItem)
		{
			StartHoverItem(NewHoveringItem);
			LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. Start Hover Item",TempName);
		}
	}
	else
	{
		if (HoveringItem.IsValid())
		{
			StopHoverItem(HoveringItem.Get());
			LOG_DOUBLE_S(1.0, FColor::Emerald, "HandleTraceHit in {0}. Stop Hover Item",TempName);
			HoveringItem.Reset();
		}
	}

	if (IsDraggingItem())
	{
		const FVector& ImpactPoint = HitResult.ImpactPoint;
		DraggingItem->SetActorLocation(ImpactPoint);

		if (IsInInventory())
		{
			
		}
	}
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

