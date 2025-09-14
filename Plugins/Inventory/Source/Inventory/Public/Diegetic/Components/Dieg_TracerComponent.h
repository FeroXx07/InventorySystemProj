// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dieg_TracerComponent.generated.h"

class ADieg_PlayerController;

/**
 * @brief Delegate fired when a new actor enters the trace.
 * 
 * This delegate is triggered when the tracer component detects a new actor
 * that wasn't previously being traced. It provides information about both
 * the current and previous actors, as well as the trace channel used.
 * 
 * @param CurrentActor The actor that was just traced
 * @param PreviousActor The previously traced actor (if any)
 * @param TraceChannel The collision channel used for the trace
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNewTraceActorIn, AActor*, CurrentActor, AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

/**
 * @brief Delegate fired when an actor exits the trace.
 * 
 * This delegate is triggered when the tracer component no longer detects
 * an actor that was previously being traced. It provides information about
 * the actor that was traced out and the trace channel used.
 * 
 * @param PreviousActor The actor that was traced out
 * @param TraceChannel The collision channel used for the trace
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOldTraceActorOut,  AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

/**
 * @brief Component responsible for tracing actors in the diegetic inventory system.
 * 
 * UDieg_TracerComponent performs continuous line traces from the player's viewport
 * center to detect interactable objects and inventory items. It manages multiple
 * trace channels simultaneously and provides delegates for actor enter/exit events.
 * 
 * The component provides:
 * - Multi-channel tracing support for different object types
 * - Automatic viewport-to-world projection for accurate tracing
 * - Weak reference management for traced actors
 * - Debug visualization capabilities
 * - Delegate system for actor detection events
 * 
 * @note This component requires a valid player controller to function properly.
 * @note The component automatically caches the owning player controller on BeginPlay.
 * @note Trace channels should be configured in the editor for proper functionality.
 * 
 * @see ADieg_PlayerController
 * @see FNewTraceActorIn
 * @see FOldTraceActorOut
 * 
 * @since 1.0
 */
UCLASS()
class INVENTORY_API UDieg_TracerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor for the tracer component.
	 * 
	 * Initializes the component with default values and enables ticking
	 * for continuous trace operations.
	 */
	UDieg_TracerComponent();

	/**
	 * @brief Delegate fired when a new actor enters the trace.
	 * 
	 * This delegate is triggered when the tracer detects a new actor
	 * that wasn't previously being traced. Bind to this delegate to
	 * respond to new actor detection events.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	FNewTraceActorIn OnActorInTrace;
	
	/**
	 * @brief Delegate fired when an actor exits the trace.
	 * 
	 * This delegate is triggered when the tracer no longer detects
	 * an actor that was previously being traced. Bind to this delegate
	 * to respond to actor exit events.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	FOldTraceActorOut OnActorOutTrace;

protected:
	/**
	 * @brief Whether to draw debug visualization for traces.
	 * 
	 * When enabled, this will draw debug lines showing the trace
	 * paths and hit points in the viewport.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	bool bDebugDraw{false};
	
	/**
	 * @brief Maximum distance for trace operations.
	 * 
	 * The maximum distance that traces will extend from the viewport center.
	 * Objects beyond this distance will not be detected by the tracer.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{100.0};

	/**
	 * @brief Array of collision channels to trace against.
	 * 
	 * Each channel in this array will be traced separately, allowing
	 * the component to detect different types of objects simultaneously.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;

	// // The Actor currently in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;
	//
	// // The last Actor different to current Actor in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	/**
	 * @brief Map of currently traced actors by collision channel.
	 * 
	 * Stores weak references to actors currently being traced for each
	 * collision channel. Used to detect when actors enter or exit the trace.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;

	/**
	 * @brief Map of previously traced actors by collision channel.
	 * 
	 * Stores weak references to actors that were previously being traced
	 * for each collision channel. Used to detect when actors exit the trace.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	/**
	 * @brief Weak reference to the owning player controller.
	 * 
	 * Cached reference to the player controller that owns this component.
	 * Used for viewport-to-world projection and input handling.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;
	
	/**
	 * @brief Initialize the component.
	 * 
	 * Called when the component is created. Sets up initial state.
	 */
	virtual void InitializeComponent() override;
	
	/**
	 * @brief Begin play for the component.
	 * 
	 * Called when the game starts. Caches the owning player controller
	 * and prepares the component for tracing operations.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Tick the component.
	 * 
	 * Called every frame. Performs continuous tracing operations
	 * to detect actors in the viewport.
	 * 
	 * @param DeltaTime Time elapsed since last tick
	 * @param TickType Type of tick being performed
	 * @param ThisTickFunction The tick function for this component
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * @brief Perform a single trace operation.
	 * 
	 * Executes a line trace from the specified start point to end point
	 * using the given collision channel.
	 * 
	 * @param TraceStart Starting point of the trace
	 * @param TraceEnd Ending point of the trace
	 * @param Channel Collision channel to trace against
	 * @return Hit result containing information about what was hit
	 */
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	FHitResult DoSingleTrace(const FVector& TraceStart, const FVector& TraceEnd,
	                         TEnumAsByte<ECollisionChannel> Channel);

private:
	/**
	 * @brief Perform multiple trace operations for all configured channels.
	 * 
	 * Called during tick to perform traces for each configured collision channel.
	 * Handles viewport-to-world projection and manages actor enter/exit detection.
	 */
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	void DoMultipleTraces();
	
	/**
	 * @brief Cache the owning player controller reference.
	 * 
	 * Finds and caches a reference to the player controller that owns this component.
	 * This is called during BeginPlay to ensure proper functionality.
	 * 
	 * @return Pointer to the owning player controller, or nullptr if not found
	 */
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	ADieg_PlayerController* CacheOwningPlayerController();

	/**
	 * @brief Get the currently traced actor for a specific channel.
	 * 
	 * Returns the actor currently being traced for the specified collision channel.
	 * This is a polling method that can be called from Blueprint.
	 * 
	 * @param Channel The collision channel to query
	 * @return Pointer to the currently traced actor, or nullptr if none
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	AActor* GetCurrentTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = CurrentTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	/**
	 * @brief Get the previously traced actor for a specific channel.
	 * 
	 * Returns the actor that was previously being traced for the specified
	 * collision channel. This is a polling method that can be called from Blueprint.
	 * 
	 * @param Channel The collision channel to query
	 * @return Pointer to the previously traced actor, or nullptr if none
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	AActor* GetPreviousTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = PreviousTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	/**
	 * @brief Get all currently traced actors.
	 * 
	 * Returns an array of all actors currently being traced across all channels.
	 * This is a Blueprint-callable utility function.
	 * 
	 * @return Array of currently traced actors
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	TArray<AActor*> GetCurrentTraceActors() const;

	/**
	 * @brief Get all previously traced actors.
	 * 
	 * Returns an array of all actors that were previously being traced across all channels.
	 * This is a Blueprint-callable utility function.
	 * 
	 * @return Array of previously traced actors
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	TArray<AActor*> GetPreviousTraceActors() const;

	/**
	 * @brief Get the owning player controller.
	 * 
	 * Returns the cached reference to the player controller that owns this component.
	 * This is a Blueprint-callable utility function.
	 * 
	 * @return Pointer to the owning player controller, or nullptr if not cached
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	ADieg_PlayerController* GetOwningPlayerController() const { return OwningPlayerController.Get(); }
};
