// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Widgets/Dieg_InteractWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"


void UDieg_InteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_InteractText->SetText(FText::FromString("Empty"));
	SetVisibility(ESlateVisibility::Hidden);
}

void UDieg_InteractWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetWidgetPositionFromWorldPosition(this->WorldLocation);
}

bool UDieg_InteractWidget::SetWidgetPositionFromWorldPosition_Implementation(const FVector& WorldPos)
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController) return false;

	FVector2D ScreenPosition = FVector2D::ZeroVector;
	bool bOK = false;

	// Check if this widget has a parent slot (means it's inside a UMG hierarchy)
	if (UPanelSlot* ParentSlot = Slot)
	{
		//  Widget is inside another widget (e.g. CanvasPanel)
		bOK = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
			PlayerController,
			WorldPos,
			ScreenPosition,
			/*bPlayerViewportRelative=*/false
		);

		if (!bOK)
		{
			return false;
		}

		// Convert from screen space → local panel space
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ParentSlot))
		{
			FVector2D LocalPos;
			USlateBlueprintLibrary::ScreenToWidgetLocal(
				PlayerController,
				CanvasSlot->Parent->GetCachedGeometry(),
				ScreenPosition,
				LocalPos
			);

			CanvasSlot->SetPosition(LocalPos);
		}
	}
	else
	{
		// Widget is added directly to the viewport
		bOK = PlayerController->ProjectWorldLocationToScreen(
			WorldPos,
			ScreenPosition,
			false
		);

		if (!bOK)
		{
			return false;
		}

		// Adjust for pivot and optionally DPI
		const FVector2D Pivot = this->GetRenderTransformPivot();
		const FVector2D WidgetSize = GetDesiredSize();
		FVector2D AdjustedPosition = ScreenPosition - WidgetSize * Pivot;

		// Optionally divide by viewport scale if needed
		AdjustedPosition /= UWidgetLayoutLibrary::GetViewportScale(this);

		SetPositionInViewport(AdjustedPosition, false);
	}

	return true;
}


void UDieg_InteractWidget::SetWorldPosition(const FVector& NewPosition)
{
	WorldLocation = NewPosition;
}

void UDieg_InteractWidget::SetInteractText(const FText& Text)
{
	TextBlock_InteractText->SetText(Text);
}
