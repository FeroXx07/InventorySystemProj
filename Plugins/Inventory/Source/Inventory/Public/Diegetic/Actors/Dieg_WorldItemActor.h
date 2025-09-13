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
	// Sets default values for this actor's properties
	ADieg_WorldItemActor();
	
protected:
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> TextRendererComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items" , meta = (AllowPrivateAccess = "true")) 
	FDieg_PrePopulate PrePopulateData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDieg_ItemInstance> ItemInstance;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	float OffsetZ{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	FDieg_InventorySlot LastInventorySlotData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	float CurrentRotation{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item") 
	int32 Quantity {1};

	// Text
	void AdjustText() const;
	void ModifyTextQuantity();
	FIntPoint GetTextCoordinates(const TArray<FIntPoint>& Shape, EDieg_TextLocation TextLocation) const;

	// Transform
	void AdjustActorLocation();
	void AdjustMeshRelativeRotation() const; // Previously named adjust rotation
	void AdjustForGrabPoint(const FVector2D& GrabPoint) const;
	void AdjustRootTransformForGrid() const;
	void AdjustRootTransformForWorld();
	float GetUnitScaled() const;
	FVector GetLocationMultiplier() const;

	UFUNCTION()
	void HandleDragHoverEnterInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);
	UFUNCTION()
	void HandleDragHoverLeaveInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                              DraggedItem, FIntPoint GrabPoint);

	UFUNCTION()
	void HandleStartDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FIntPoint Coordinates);
	
	UFUNCTION()
	void HandleStartDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DraggedItem, FIntPoint GrabPoint, FVector WorldLocation);

	UFUNCTION()
	void HandleStopDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                             DroppedItem, FIntPoint DroppedCoordinates, float DroppedRotation);

	UFUNCTION()
	void HandleStopDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, FVector DroppedLocation);

	UFUNCTION()
	void HandleResetDragInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor* RestItem, FIntPoint
	                              ResetCoordinates, float ResetRotation);
	
	UFUNCTION()
	void HandleResetDragWorld(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* ResetItem, FVector ResetLocation);

	UFUNCTION()
	void HandleRotateItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* RotatedItem, float NewRotation, FIntPoint GripPoint);

	UFUNCTION()
	void HandleMergeItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, AActor* DroppedItem, TArray<AActor*> MergedActors, int32
	                              OldQuantity, int32 NewQuantity);

	UFUNCTION()
	void HandleConsumedItemInventory(UDieg_InventoryInputHandler* InventoryInputHandler, UDieg_3DInventoryComponent* InventoryComponent3D, AActor*
	                                 ConsumedActor);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(UObject* Interactor) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void BindEventsToHandler(UDieg_InventoryInputHandler* Handler);

	UFUNCTION(BlueprintNativeEvent)
	void UnBindEventsFromHandler(UDieg_InventoryInputHandler* Handler);
	
	bool IsOwnedByInventory() const;
	bool IsInInventory() const;
	const UDieg_3DInventoryComponent* GetOwnerComponent() const;
	
	const TObjectPtr<UDieg_ItemInstance>& GetItemInstance() const;
	TObjectPtr<UDieg_ItemInstance>& GetItemInstanceMutable();

	int32 GetQuantity() const;
	void SetQuantity(const int32 Value);
	void SetCurrentRotation(float Value) { CurrentRotation = Value; }
	float GetCurrentRotation() const { return CurrentRotation; }
	void SetCoordinates(const FIntPoint& Value) { LastInventorySlotData.Coordinates = Value; }
	FIntPoint GetCoordinates() const { return LastInventorySlotData.Coordinates; }
	void SetLastSlotData(const FDieg_InventorySlot& Slot) { LastInventorySlotData = Slot; }
	const FDieg_InventorySlot& GetLastSlotData() const { return LastInventorySlotData; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void UpdateFromDataAsset(UDieg_ItemDefinitionDataAsset* InItemDataAsset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetFromItemInstance(UDieg_ItemInstance* Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetFromInventorySlot(const FDieg_InventorySlot& InventorySlot);
private:
	
	bool SetMesh(const UDieg_ItemDefinitionDataAsset* InItemDataAsset) const;
};
