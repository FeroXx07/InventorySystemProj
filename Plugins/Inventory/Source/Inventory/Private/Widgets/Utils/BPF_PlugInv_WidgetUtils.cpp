// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Utils/BPF_PlugInv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"

int32 UPlugInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UPlugInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

FVector2D UPlugInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	// FGeometry has info
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	// From the geometry get the viewport space location of its top left corner
	FVector2D ViewportPosition;
	const FVector2D LocalTopLeft = USlateBlueprintLibrary::GetLocalTopLeft(Geometry);
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry, LocalTopLeft, PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D UPlugInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UPlugInv_WidgetUtils::IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize,
	const FVector2D& MousePos)
{
	return MousePos.X >= BoundaryPos.X && MousePos.X <= (BoundaryPos.X + WidgetSize.X) &&
		MousePos.Y >= BoundaryPos.Y && MousePos.Y <= (BoundaryPos.Y + WidgetSize.Y);
}

FVector2D UPlugInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize,
	const FVector2D& MousePos)
{
	FVector2D ClampedPosition = MousePos;
	
	// Adjust the horizontal position to ensure that the widget stays within the boundary
	if (MousePos.X + WidgetSize.X > Boundary.X) // Widget exceeds the right edge
	{
		ClampedPosition.X = Boundary.X - WidgetSize.X;
	}
	
	if (MousePos.X < 0.f) // Widget exceeds the left edge
	{
		ClampedPosition.X = 0.f;
	}

	// Adjust vertical position to ensure that the widget stays within the boundary
	if (MousePos.Y + WidgetSize.Y > Boundary.Y) // Widget exceeds the bottom edge
	{
		ClampedPosition.Y = Boundary.Y - WidgetSize.Y;
	}
	
	if (MousePos.Y < 0.f) // Widget exceeds the top edge
	{
		ClampedPosition.Y = 0.f;
	}

	return ClampedPosition;
}
