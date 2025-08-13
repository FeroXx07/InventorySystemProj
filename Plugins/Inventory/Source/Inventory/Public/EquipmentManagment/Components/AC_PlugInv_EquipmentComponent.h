// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "AC_PlugInv_EquipmentComponent.generated.h"


struct FPlugInv_EquipmentFragment;
struct FPlugInv_ItemManifest;
class APlugInv_EquipActor;
class UPlugInv_InventoryComponent;
class UPlugInv_InventoryItem;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlugInv_EquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:

	TWeakObjectPtr<UPlugInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;
	
	UPROPERTY()
	TArray<TObjectPtr<APlugInv_EquipActor>> EquippedActors;
	
	UFUNCTION()
	void OnItemEquipped(UPlugInv_InventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnequipped(UPlugInv_InventoryItem* UnEquippedItem);

	void InitPlayerController();
	void InitInventoryComponent();

	APlugInv_EquipActor* SpawnEquippedActor(FPlugInv_EquipmentFragment* EquipmentFragment, const FPlugInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh);
	APlugInv_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn);
};
