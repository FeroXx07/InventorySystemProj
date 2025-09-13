// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/Dieg_DataLibrary.h"
#include "Diegetic/Components/Dieg_3DInventoryComponent.h"
#include "Diegetic/Interfaces/Dieg_Interactable.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldItemActor.generated.h"

class UDieg_InventoryInputHandler;
class UTextRenderComponent;
class UDieg_ItemInstance;
class UDieg_ItemDefinitionDataAsset;

UCLASS()
class INVENTORY_API ADieg_WorldItemActor : public AActor, public IDieg_Interactable
{
	GENERATED_BODY()
	
public:
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Dieg|World Item Actor|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> TextRendererComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Items" , meta = (AllowPrivateAccess = "true")) 
	FDieg_PrePopulate PrePopulateData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|World Item Actor|Items", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_ItemInstance> ItemInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	float OffsetZ{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	FDieg_InventorySlot LastInventorySlotData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	float CurrentRotation{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Dieg|World Item Actor|Item", meta = (AllowPrivateAccess = "true")) 
	int32 Quantity {1};

	// Sets default values for this actor's properties
	ADieg_WorldItemActor();
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Text
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustText() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void ModifyTextQuantity();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	FIntPoint GetTextCoordinates(const TArray<FIntPoint>& Shape, EDieg_TextLocation TextLocation) const;

	// Transform
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustActorLocation();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustMeshRelativeRotation() const; // Previously named adjust rotation
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustForGrabPoint(const FVector2D& GrabPoint) const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustRootTransformForGrid() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void AdjustRootTransformForWorld();
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	float GetUnitScaled() const;
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	FVector GetLocationMultiplier() const;

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleDragHoverEnterInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleDragHoverLeaveInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStartDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FIntPoint Coordinates);
	
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStartDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FVector WorldLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStopDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                             DroppedItem, FIntPoint DroppedCoordinates, float DroppedRotation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleStopDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, FVector DroppedLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleResetDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor* RestItem, FIntPoint
	                              ResetCoordinates, float ResetRotation);
	
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleResetDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* ResetItem, FVector ResetLocation);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleRotateItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* RotatedItem, float NewRotation, FIntPoint GripPoint);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleMergeItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, TArray<AActor*> MergedActors, int32
	                              OldQuantity, int32 NewQuantity);

	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	void HandleConsumedItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                                 ConsumedActor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(UObject* Interactor) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|World Item Actor")
	void BindEventsToHandler(UDieg_InventoryInputHandler* Handler);

	UFUNCTION(BlueprintNativeEvent, Category = "Game|Dieg|World Item Actor")
	void UnBindEventsFromHandler(UDieg_InventoryInputHandler* Handler);
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	bool IsOwnedByInventory() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	bool IsInInventory() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	const UDieg_3DInventoryComponent* GetOwnerComponent() const;
	
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	UDieg_ItemInstance* GetItemInstance() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	UDieg_ItemInstance* GetItemInstanceMutable();

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	int32 GetQuantity() const;
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetQuantity(const int32 Value);
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetCurrentRotation(float Value) { CurrentRotation = Value; }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	float GetCurrentRotation() const { return CurrentRotation; }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetCoordinates(const FIntPoint& Value) { LastInventorySlotData.Coordinates = Value; }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	FIntPoint GetCoordinates() const { return LastInventorySlotData.Coordinates; }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	void SetLastSlotData(const FDieg_InventorySlot& Slot) { LastInventorySlotData = Slot; }
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|World Item Actor")
	const FDieg_InventorySlot& GetLastSlotData() const { return LastInventorySlotData; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void UpdateFromDataAsset(UDieg_ItemDefinitionDataAsset* InItemDataAsset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void SetFromItemInstance(UDieg_ItemInstance* Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Game|Dieg|World Item Actor")
	void SetFromInventorySlot(const FDieg_InventorySlot& InventorySlot);

private:
	UFUNCTION(Category = "Game|Dieg|World Item Actor")
	bool SetMesh(const UDieg_ItemDefinitionDataAsset* InItemDataAsset) const;
};
