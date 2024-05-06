// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_Interactor.h"
#include "BlueprintFunctionLibrary/BPF_DataLibrary.h"
#include "Components/ActorComponent.h"
#include "AC_Inventory.generated.h"


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEMPROJ_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Inventory")
	TObjectPtr<UInteractor> Interactor;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Inventory")
	TArray<FInventoryContainer> InventoryContainers;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Inventory")
	TArray<TObjectPtr<UInventoryItem>> InventoryItems;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(UInventoryItem* ItemToAdd, const int64 ContainerUniqueId);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventoryByDataAsset(UItemCore* ItemToAddDataAsset, const int64 ContainerUniqueId);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventory(UInventoryItem* ItemToAdd, const int64 ContainerUniqueId);
};
