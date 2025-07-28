// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagment/Components/AC_PlugInv_InventoryComponent.h"
#include "InventoryManagment/Utils/BPF_PlugInv_InventoryStatics.h"
#include "Items/O_PlugInv_InventoryItem.h"
#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"

void UPlugInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UPlugInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
}

void UPlugInv_InventoryGrid::ConstructGrid()
{
	UPlugInv_DoubleLogger::Log("Constructing grid");
	
	GridSlots.Reserve(Rows*Columns);

	for (int j = 0; j < Rows; ++j)
	{
		for (int i = 0; i < Columns; ++i)
		{
			// Unique temp name
			FString StrName = TEXT("Row ") + FString::FromInt(j) + TEXT(" Col ") + FString::FromInt(i);

			// Creating and adding grid slot to the canvas
			UPlugInv_GridSlot* GridSlot = CreateWidget<UPlugInv_GridSlot>(this, GridSlotClass, FName(*StrName));
			CanvasPanel->AddChild(GridSlot);
			
			const FIntPoint TilePos = FIntPoint(i, j);
			
			// Setting index
			GridSlot->SetTileIndex(UPlugInv_WidgetUtils::GetIndexFromPosition(TilePos, Columns));

			// Grid slot widget pos and size
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePos * TileSize);

			// Adding grid slot to the container array
			GridSlots.Add(GridSlot);
		}
	}
}

void UPlugInv_InventoryGrid::AddItem(UPlugInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item))
	{
		return;
	}

	UPlugInv_DoubleLogger::Log("UPlugInv_InventoryGrid::AddItem()");
}

bool UPlugInv_InventoryGrid::MatchesCategory(const UPlugInv_InventoryItem* Item) const
{
	return this->ItemCategory == Item->GetItemManifest().GetItemCategory();
}
