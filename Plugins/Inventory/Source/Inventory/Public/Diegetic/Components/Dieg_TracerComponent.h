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
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FNewTraceActorIn OnActorInTrace;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOldTraceActorOut OnActorOutTrace;
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FHitResult DoSingleTrace(const FVector& TraceStart, const FVector& TraceEnd,
	                         TEnumAsByte<ECollisionChannel> Channel);
private:
	// Function to trace for collisions that have item trace channel
	void DoMultipleTraces();
	
	ADieg_PlayerController* CacheOwningPlayerController();
	
	// Max distance for interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{100.0};

	// Trace channel enabled for interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;

	// // The Actor currently in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;
	//
	// // The last Actor different to current Actor in trace line.
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	// TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	UPROPERTY(VisibleAnywhere, Category = "Weak References")
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> CurrentTraceActor;

	UPROPERTY(VisibleAnywhere, Category = "Weak References")
	TMap<TEnumAsByte<ECollisionChannel>, TWeakObjectPtr<AActor>> PreviousTraceActor;

	// Polling method
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = CurrentTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	// Polling method
	UFUNCTION(BlueprintCallable)
	AActor* GetPreviousTraceActor(const TEnumAsByte<ECollisionChannel> Channel) const
	{
		if (const TWeakObjectPtr<AActor>* Found = PreviousTraceActor.Find(Channel))
		{
			return Found->Get();
		}
		return nullptr;
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;
};
