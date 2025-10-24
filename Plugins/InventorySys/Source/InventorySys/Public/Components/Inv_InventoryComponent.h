// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryContainer;
class UInv_ItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYS_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInv_InventoryComponent();

	// Spatial inventory containers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent|Containers", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInv_InventoryContainer>> InventoryContainers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game|Dieg|InventoryComponent|Storage", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer InventoryTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game|Dieg|InventoryComponent|Storage",
		meta=(ClampMin="-90.0", ClampMax="180.0", UIMin="-90.0", UIMax="180.0", AllowPrivateAccess = "true"))
	float ItemRotationPriority{90.0};
	
	// Container system properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game|Dieg|InventoryComponent|Containers", meta = (AllowPrivateAccess = "true"))
	FName DefaultContainerName{TEXT("MainContainer")};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game|Dieg|InventoryComponent|Containers", meta = (AllowPrivateAccess = "true"))
	int32 DefaultContainerWidth{10};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game|Dieg|InventoryComponent|Containers", meta = (AllowPrivateAccess = "true"))
	int32 DefaultContainerHeight{10};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|InventoryComponent")
	bool bIsInitialized{false};
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Spatial inventory initialization
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	void InitializeSpatialInventory();

	// Container management
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	UInv_InventoryContainer* CreateContainer(const FName& ContainerName, const FText& DisplayName, int32 Width, int32 Height, const FGameplayTagContainer& Tags = FGameplayTagContainer());

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool RemoveContainer(const FName& ContainerName);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	UInv_InventoryContainer* GetContainer(const FName& ContainerName) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	UInv_InventoryContainer* GetContainerAtIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	int32 GetContainerCount() const;

	// Spatial item management
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryAddItemToContainer(UInv_ItemInstance* ItemToAdd, const FName& ContainerName, const FIntPoint& GridPosition, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryAddItemToAnyContainer(UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryRemoveItemFromContainer(UInv_ItemInstance* ItemToRemove, const FName& ContainerName);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool TryMoveItemBetweenContainers(UInv_ItemInstance* Item, const FName& FromContainer, const FName& ToContainer, const FIntPoint& NewPosition, float NewRotation = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool CanPlaceItemInContainer(const UInv_ItemInstance* ItemToAdd, const FName& ContainerName, const FIntPoint& GridPosition, float Rotation = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool CanPlaceItemInAnyContainer(const UInv_ItemInstance* ItemToAdd, FName& OutContainerName, FIntPoint& OutGridPosition, float Rotation = 0.0f) const;

	// Utility functions
	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	TArray<FName> GetContainerNames() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|InventoryComponent")
	bool DoesInventoryContainItem(const UInv_ItemInstance* ItemToCheck) const;
};
