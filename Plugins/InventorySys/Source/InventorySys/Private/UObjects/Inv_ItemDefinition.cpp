// Fill out your copyright notice in the Description page of Project Settings.

#include "UObjects/Inv_ItemDefinition.h"
#include "Engine/Engine.h"

TArray<FIntPoint> UInv_ItemDefinition::GetShapeAtRotation(float Rotation) const
{
	TArray<FIntPoint> RotatedShape = DefaultShape;
	
	// Normalize rotation to 0, 90, 180, 270 degrees
	int32 RotationSteps = FMath::RoundToInt(Rotation / 90.0f) % 4;
	if (RotationSteps < 0) RotationSteps += 4;
	
	// Apply rotation steps
	for (int32 Step = 0; Step < RotationSteps; ++Step)
	{
		for (FIntPoint& Point : RotatedShape)
		{
			// 90-degree rotation: (x, y) -> (-y, x)
			int32 NewX = -Point.Y;
			int32 NewY = Point.X;
			Point.X = NewX;
			Point.Y = NewY;
		}
	}
	
	return RotatedShape;
}

TArray<FIntPoint> UInv_ItemDefinition::GetShapeAtPosition(const FIntPoint& Position, float Rotation) const
{
	TArray<FIntPoint> Shape = GetShapeAtRotation(Rotation);
	
	// Offset by the position
	for (FIntPoint& Point : Shape)
	{
		Point += Position;
	}
	
	return Shape;
}

bool UInv_ItemDefinition::IsValidShape() const
{
	// Check if shape has at least one point
	if (DefaultShape.Num() == 0)
	{
		return false;
	}
	
	// Check if all points are valid (non-negative coordinates)
	for (const FIntPoint& Point : DefaultShape)
	{
		if (Point.X < 0 || Point.Y < 0)
		{
			return false;
		}
	}
	
	return true;
}

void UInv_ItemDefinition::AutoCalculateShapeDimensions()
{
	if (DefaultShape.Num() == 0)
	{
		ShapeWidth = 1;
		ShapeHeight = 1;
		return;
	}
	
	int32 MinX = INT_MAX, MaxX = INT_MIN;
	int32 MinY = INT_MAX, MaxY = INT_MIN;
	
	for (const FIntPoint& Point : DefaultShape)
	{
		MinX = FMath::Min(MinX, Point.X);
		MaxX = FMath::Max(MaxX, Point.X);
		MinY = FMath::Min(MinY, Point.Y);
		MaxY = FMath::Max(MaxY, Point.Y);
	}
	
	ShapeWidth = MaxX - MinX + 1;
	ShapeHeight = MaxY - MinY + 1;
}

FIntPoint UInv_ItemDefinition::GetShapeBounds() const
{
	return FIntPoint(ShapeWidth, ShapeHeight);
}
