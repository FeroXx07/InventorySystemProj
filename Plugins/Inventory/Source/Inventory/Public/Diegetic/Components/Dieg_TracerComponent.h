// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dieg_TracerComponent.generated.h"

class ADieg_PlayerController;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNewTraceActorIn, AActor*, CurrentActor, AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOldTraceActorOut,  AActor*, PreviousActor, TEnumAsByte<ECollisionChannel>, TraceChannel);

UCLASS()
class INVENTORY_API UDieg_TracerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_TracerComponent();

	// Delegate methods
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	FNewTraceActorIn OnActorInTrace;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	FOldTraceActorOut OnActorOutTrace;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	bool bDebugDraw{false};
	
	// Max distance for interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{100.0};

	// Trace channel enabled for interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|Tracer Component|Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;

	// // The Actor currently in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;
	//
	// // The last Actor different to current Actor in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;

	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	UPROPERTY(VisibleAnywhere, Category = "Game|Dieg|Tracer Component|Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	FHitResult DoSingleTrace(const FVector& TraceStart, const FVector& TraceEnd,
	                         TEnumAsByte<ECollisionChannel> Channel);

private:
	// Function to trace for collisions that have item trace channel
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	void DoMultipleTraces();
	
	UFUNCTION(Category = "Game|Dieg|Tracer Component")
	ADieg_PlayerController* CacheOwningPlayerController();

	// Polling method
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	AActor* GetCurrentTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = CurrentTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	// Polling method
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	AActor* GetPreviousTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = PreviousTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	// Blueprint getter functions for private properties
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	TArray<AActor*> GetCurrentTraceActors() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	TArray<AActor*> GetPreviousTraceActors() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Tracer Component")
	ADieg_PlayerController* GetOwningPlayerController() const { return OwningPlayerController.Get(); }
};
