// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryContainer;
class UInv_ItemInstance;
class UInv_InventoryController;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class INVENTORYSYS_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	// ============================================
	// DATA LAYER (Containers)
	// ============================================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Data")
	TArray<TObjectPtr<UInv_InventoryContainer>> InventoryContainers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Settings")
	FGameplayTagContainer InventoryTags;

	// ============================================
	// INTERACTION LAYER (Controller - owned by this component)
	// ============================================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory|Controller")
	TObjectPtr<UInv_InventoryController> Controller;

	// ============================================
	// DEFAULT SETTINGS
	// ============================================
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Settings")
	FName DefaultContainerName = TEXT("MainContainer");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Settings")
	int32 DefaultContainerWidth = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Settings")
	int32 DefaultContainerHeight = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Settings")
	bool bCreateDefaultContainer = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bIsInitialized = false;
	
	// ============================================
	// LIFECYCLE
	// ============================================
	
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ============================================
	// CONTROLLER ACCESS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInv_InventoryController* GetController() const { return Controller; }

	// ============================================
	// FORWARDING API (Optional - provides cleaner interface)
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryPickupItem(UInv_ItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryPlaceItem(UInv_ItemInstance* Item, FIntPoint Position, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryRemoveItem(UInv_ItemInstance* Item);

	// ============================================
	// CONTAINER MANAGEMENT
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	UInv_InventoryContainer* CreateContainer(FName ContainerName, FText DisplayName, int32 Width, int32 Height, const FGameplayTagContainer& Tags = FGameplayTagContainer());

	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	bool RemoveContainer(FName ContainerName);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	UInv_InventoryContainer* GetContainer(FName ContainerName) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	UInv_InventoryContainer* GetContainerAtIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	int32 GetContainerCount() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Containers")
	TArray<FName> GetContainerNames() const;

	// ============================================
	// ITEM OPERATIONS
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Items")
	bool TryAddItemToContainer(UInv_ItemInstance* ItemToAdd, FName ContainerName, FIntPoint GridPosition, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Items")
	bool TryAddItemToAnyContainer(UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Items")
	bool TryRemoveItemFromContainer(UInv_ItemInstance* ItemToRemove, FName ContainerName);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Items")
	bool TryMoveItemBetweenContainers(UInv_ItemInstance* Item, FName FromContainer, FName ToContainer, FIntPoint NewPosition, float NewRotation = 0.0f);

	// ============================================
	// QUERIES
	// ============================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
	bool CanPlaceItemInContainer(UInv_ItemInstance* ItemToAdd, FName ContainerName, FIntPoint GridPosition, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
	bool CanPlaceItemInAnyContainer(UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
	bool DoesInventoryContainItem(UInv_ItemInstance* ItemToCheck) const;
};
