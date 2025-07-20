// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/UW_PlugInv_InventoryGrid.h"

#include "BPF_PlugInv_DoubleLogger.h"
#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"

void UPlugInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();
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
