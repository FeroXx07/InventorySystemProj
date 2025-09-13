// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Diegetic/Widgets/Dieg_Slot.h"
#include "Dieg_InventoryInputHandler.generated.h"


class UDieg_InventoryComponent;
class ADieg_Briefcase;
class ADieg_WorldItemActor;
class UDieg_3DInventoryComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class ADieg_PlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDragHoverInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DraggedItem, FIntPoint, GrabPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDragUnHoverInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DraggedItem, FIntPoint, GrabPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverInventory, UDieg_3DInventoryComponent*, InventoryComponent3D);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnHoverInventory, UDieg_3DInventoryComponent*, InventoryComponent3D);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartHoverSlot, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartUnHoverSlot, UDieg_3DInventoryComponent*, InventoryComponent3D, UDieg_Slot*, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartDragWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, FIntPoint, GrabPoint, FVector, WorldLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartDragInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, FIntPoint, GrabPoint, FIntPoint, Coordinates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FUpdateDragSlot, bool, IsValid, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DraggedItem, const TArray<FIntPoint>&, RelevantSlots, FIntPoint, Coordinates, float, Rotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDropWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DroppedItem, FVector, DroppedLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMergeItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, DroppedItem, TArray<AActor*>, MergedActors, int32, OldQuantity, int32, NewQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FConsumedItemInMerge, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, ConsumedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FResetDragWorld, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, ResetItem, FVector, ResetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FResetDragInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, RestItem, FIntPoint, ResetCoordinates, float, ResetRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDropInInventory, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, DroppedItem, FIntPoint, DroppedCoordinates, float, DroppedRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, HoveredItem, bool, IsInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FUnHoverItem, UDieg_InventoryInputHandler*, InventoryInputHandler, UDieg_3DInventoryComponent*, InventoryComponent3D, AActor*, UnHoveredItem, bool, IsInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FRotateItem, UDieg_InventoryInputHandler*, InventoryInputHandler, AActor*, RotatedItem, float, NewRotation, FIntPoint, GripPoint);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORY_API UDieg_InventoryInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDieg_InventoryInputHandler();

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInventoryOpened OnInventoryOpened;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInventoryClosed OnInventoryClosed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDragHoverInventory OnDragHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDragUnHoverInventory OnDragUnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHoverInventory OnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUnHoverInventory OnUnHoverInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartDragWorld OnStartDragWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartDragInventory OnStartDragInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartHoverSlot OnStartHoverSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStartUnHoverSlot OnStartUnHoverSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateDragSlot OnUpdateDragSlot;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDropWorld OnDropWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FMergeItem OnMergeItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FConsumedItemInMerge OnConsumedItemInMerge;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResetDragWorld OnResetDragWorld;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResetDragInventory OnResetDragInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDropInInventory OnDropInInventory;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FHoverItem OnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUnHoverItem OnUnHoverItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FRotateItem OnRotateItem;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeImcSubsystem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	float TraceTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	float TraceInterval = 0.05f; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Component")
	bool bDebugLogs{false};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	bool bIsInventoryOpen{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diegetic Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADieg_Briefcase> BriefCaseActorClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Strong References", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADieg_Briefcase> BriefcaseActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FVector RelativeSpawnLocation{0.0f, 0.0, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Briefcase Spawn", meta = (AllowPrivateAccess = "true"))
	FRotator RelativeSpawnRotation{0.0f, 0.0, 0.0f};
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint RelativeCoordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	float ValidRotation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	float CurrentRotation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FVector ValidWorldLocation{0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint ValidCoordinates{-1, -1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	FIntPoint CurrentMouseCoordinates{-1, -1};

	// Only for visuals
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dieg Inventory", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_Slot> CurrentMouseSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> ValidInventory3D;

	// Used for default return to origin cases
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weak References", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UDieg_3DInventoryComponent> Default3DInventory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	bool bDebugDraw{false};
	
	// Max distance for interaction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	double TraceLength{500.0};

	// Trace channel enabled for interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<ECollisionChannel>> TraceChannels;
	
	void ToggleInventory();
	void OpenUserInterface();
	void CloseUserInterface();
	bool LineTraceFromMouse(FHitResult& HitResult) const;
	void StartHoverInventory(UDieg_3DInventoryComponent* NewHoveringInventory);
	void StopHoverInventory(UDieg_3DInventoryComponent* HoveringInventory);
	void StartHoverItem(ADieg_WorldItemActor* NewHoveringWorldItem);
	void StopHoverItem(ADieg_WorldItemActor* HoveringWorldItem);
	void HandleTraceHit(const FHitResult& HitResult, bool bIsBlockingHit);

	bool IsDraggingItem() const { return DraggingItem.IsValid(); }
	bool IsInInventory() const { return HoveringInventoryComponent3D.IsValid(); }
	bool IsItemInInventory(const ADieg_WorldItemActor* ItemIn, UDieg_3DInventoryComponent*& Inventory3DOut);
	
	UFUNCTION(Category = "Game|Player|Dieg")
	void TryDragItem();
	UFUNCTION(Category = "Game|Player|Dieg")
	void TryDropItem();

	UFUNCTION(Category = "Game|Player|Dieg")
	void HandleInputRotateItem();
	
	UFUNCTION(Category = "Game|Player|Dieg")
	void TryRotateItem();
	void InternalHandleInventorySlotHover(UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);

	UFUNCTION(Category = "Game|Player|Dieg")
	void HandleHoveringInventorySlotHovered(const FGeometry& Geometry, const FPointerEvent& PointerEvent, UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);
	UFUNCTION(Category = "Game|Player|Dieg")
	void HandleHoveringInventorySlotUnHovered(const FPointerEvent& PointerEvent, UDieg_3DInventoryComponent* Dieg_3DInventoryComponent, UDieg_Slot* Dieg_Slot);
	
	void StartDraggingItem();
	void StopDraggingItem();
	bool MergeItems(const TArray<FIntPoint>& RootSlotCoordinates);
	void RotateItem();
	// Connect dragging item to hovering inventory
	void ConnectItemToInventory() const;
	void DisconnectItemToInventory() const;

	// Where we clicked in local space (shape coordinates)
	FIntPoint GetGrabCoordinates() const;
	FIntPoint GetRotatedGrabCoordinates(bool UseValidated) const;
	TArray<FIntPoint> GetRelevantCoordinates() const;
	bool GetCurrentSlot(UDieg_Slot*& SlotOut) const;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
