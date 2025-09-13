// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Diegetic/Dieg_DataLibrary.h"
#include "Diegetic/Interfaces/Dieg_Interactable.h"
#include "Diegetic/UStructs/Dieg_InventorySlot.h"
#include "Diegetic/UStructs/Dieg_PrePopulate.h"
#include "GameFramework/Actor.h"
#include "Dieg_WorldItemActor.generated.h"

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
	void AdjustLocation(const FIntPoint& Coordinates);
	void AdjustRotation() const;
	void AdjustForGrabPoint(const FVector2D& GrabPoint) const;
	void AdjustTransformForGrid() const;
	void AdjustTransformForWorld();
	float GetUnitScaled() const;
	FVector GetLocationMultiplier() const;
	
	void HandleDragEnterInventory(const FVector2D& GrabPoint);
	void HandleDragLeaveInventory();
	
	void HandleStartDragInventory();
	void HandleStartDragWorld();
	void HandleStopDragInventory();
	void HandleStopDragWorld();

	void HandleResetDragInventory();
	void HandleResetDragWorld();

	void HandleRotateItemInventory();
	void HandleMergeItemInventory();
	void HandleConsumedItemInventory();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnInteract_Implementation(UObject* Interactor) override;

	bool IsOwnedByInventory() const;

	const TObjectPtr<UDieg_ItemInstance>& GetItemInstance() const;
	TObjectPtr<UDieg_ItemInstance>& GetItemInstanceMutable();

	float GetCurrentRotation() const { return CurrentRotation; }
	FIntPoint GetCoordinates() const { return LastInventorySlotData.Coordinates; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void UpdateFromDataAsset(UDieg_ItemDefinitionDataAsset* InItemDataAsset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetFromItemInstance(UDieg_ItemInstance* Instance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Item")
	void SetFromInventorySlot(const FDieg_InventorySlot& InventorySlot);
private:
	
	bool SetMesh(const UDieg_ItemDefinitionDataAsset* InItemDataAsset) const;
};
