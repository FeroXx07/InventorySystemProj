// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Dieg_InventoryInputHandler.generated.h"


class ADieg_WorldItemActor;
class UDieg_3DInventoryComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class ADieg_PlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_InventoryInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_InventoryInputHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeImcSubsystem();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen{false};
	
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> ToggleInventoryIA;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> RotateIA;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> DragIA;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input Mapping Contexts")
	TObjectPtr<UInputMappingContext> DefaultIMCs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_PlayerController> OwningPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> HoveringInventoryComponent3D;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> HoveringItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ADieg_WorldItemActor> DraggingItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> LastHitActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bDebugDraw{false};
	
	// Max distance for interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{2000.0};

	// Trace channel enabled for interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;
	
	void ToggleInventory();
	void OpenUserInterface() const;
	void CloseUserInterface() const;
	bool LineTraceFromMouse(FHitResult& HitResult) const;
	void StartHoverInventory(UDieg_3DInventoryComponent* NewHoveringInventory);
	void StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory);
	void StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem);
	void StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem);
	void HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit);

	bool IsDraggingItem() const { return DraggingItem.IsValid(); }
	bool IsInInventory() const { return HoveringInventoryComponent3D.IsValid(); }
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
