// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagment/AC_PlugInv_Inventory.h"

#include "Blueprint/UserWidget.h"

#include "Widgets/Inventory/InventoryBase/UW_PlugInv_InventoryBase.h"

// Sets default values for this component's properties
UPlugInv_Inventory::UPlugInv_Inventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlugInv_Inventory::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();	
}

void UPlugInv_Inventory::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void UPlugInv_Inventory::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);
}

void UPlugInv_Inventory::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenu)) return;

	InventoryMenu->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningPlayerController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}

void UPlugInv_Inventory::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>(GetOwner());
	checkf(OwningPlayerController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner"));
	if (!OwningPlayerController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UPlugInv_InventoryBase>(OwningPlayerController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();
	
	CloseInventoryMenu();
}



