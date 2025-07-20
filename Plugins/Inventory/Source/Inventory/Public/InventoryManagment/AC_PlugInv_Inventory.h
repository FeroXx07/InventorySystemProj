// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_PlugInv_Inventory.generated.h"


class UPlugInv_InventoryBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UPlugInv_Inventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlugInv_Inventory();

	void ToggleInventoryMenu();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	// Weak ptr holding a ref back to player without affecting GB (Garbage Collection)
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UPlugInv_InventoryBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<UPlugInv_InventoryBase> InventoryMenu;
	
	// Creates the inventory widget
	void ConstructInventory();
};
