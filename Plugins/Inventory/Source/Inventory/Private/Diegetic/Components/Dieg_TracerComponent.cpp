// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Components/Dieg_TracerComponent.h"

#include "Diegetic/Dieg_PlayerController.h"
#include "Kismet/GameplayStatics.h"

UDieg_TracerComponent::UDieg_TracerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDieg_TracerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDieg_TracerComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheOwningPlayerController();
}

void UDieg_TracerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DoMultipleTraces();
}

void UDieg_TracerComponent::DoMultipleTraces()
{
	if (!OwningPlayerController.IsValid()|| !IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize/2.f;
	FVector TraceStart;
	FVector TraceForward;

	if (!UGameplayStatics::DeprojectScreenToWorld(OwningPlayerController.Get(), ViewportCenter, TraceStart, TraceForward)) return;

	const FVector TraceEnd = TraceStart + TraceForward * TraceLength;
	FHitResult HitResult;
	for (const TEnumAsByte<ECollisionChannel>& Channel : TraceChannels)
	{
		DoSingleTrace(TraceStart, TraceEnd, Channel);
	}
}

FHitResult UDieg_TracerComponent::DoSingleTrace(
    const FVector& TraceStart,
    const FVector& TraceEnd,
    const TEnumAsByte<ECollisionChannel> Channel)
{
    FHitResult HitResult;
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, Channel);

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
			1.0f   // thickness
		);
	}
	
    if (bHit && bDebugDraw)
    {
        DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Blue, false, 2.0f);
    }

    // Get previous and current actor for this channel
    TWeakObjectPtr<AActor> PreviousActor = CurrentTraceActor.FindRef(Channel);
    TWeakObjectPtr<AActor> CurrentActor = HitResult.GetActor();

    // --- Handle destroyed previous actor ---
    if (!PreviousActor.IsValid() && CurrentTraceActor.Contains(Channel))
    {
        if (OnActorOutTrace.IsBound())
        {
            OnActorOutTrace.Broadcast(nullptr, Channel); // or cache info if needed
        }
        CurrentTraceActor.Remove(Channel);
    }

    // --- Update current actor map ---
    if (CurrentActor.IsValid())
    {
        CurrentTraceActor.Add(Channel, CurrentActor);
    }
    else if (CurrentTraceActor.Contains(Channel))
    {
        CurrentTraceActor.Remove(Channel);
    }

    // --- Update previous actor map ---
    if (PreviousActor.IsValid())
    {
        PreviousTraceActor.Add(Channel, PreviousActor);
    }
    else if (PreviousTraceActor.Contains(Channel))
    {
        PreviousTraceActor.Remove(Channel);
    }

    // --- Handle normal in/out events ---
    if (CurrentActor != PreviousActor)
    {
        // New actor entered trace
        if (CurrentActor.IsValid() && OnActorInTrace.IsBound())
        {
            OnActorInTrace.Broadcast(CurrentActor.Get(), PreviousActor.Get(), Channel);
        }

        // Old actor left trace
        if (PreviousActor.IsValid() && OnActorOutTrace.IsBound())
        {
            OnActorOutTrace.Broadcast(PreviousActor.Get(), Channel);
        }
    }

    return HitResult;
}


ADieg_PlayerController* UDieg_TracerComponent::CacheOwningPlayerController()
{
	if (!OwningPlayerController.IsValid())
	{
		if (ADieg_PlayerController* PlayerController = Cast<ADieg_PlayerController>(GetOwner()))
		{
			OwningPlayerController = PlayerController;
			return PlayerController;
		}
	}
	return nullptr;
}
