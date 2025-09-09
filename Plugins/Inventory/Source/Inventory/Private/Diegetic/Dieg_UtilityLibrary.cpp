// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Dieg_UtilityLibrary.h"

#include "Materials/MaterialExpressionSceneColor.h"

FIntPoint UDieg_UtilityLibrary::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

int32 UDieg_UtilityLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

TArray<FIntPoint> UDieg_UtilityLibrary::GetSlotPoints(int32 TotalSlots, int32 Columns)
{
	TArray<FIntPoint> Coordinates;
	Coordinates.Reserve(TotalSlots); // Preallocate memory

	for (int32 Index = 0; Index < TotalSlots; ++Index)
	{
		const int32 InX = Index % Columns;       // Column index
		const int32 InY = Index / Columns;       // Row index
		Coordinates.Add(FIntPoint(InX, InY));
	}

	return Coordinates;
}

FIntPoint UDieg_UtilityLibrary::GetShapeSpan(const TArray<FIntPoint>& Shape)
{
	TArray<int32> X, Y;
	int32 SpanX = 0, SpanY = 0;
	int32 IndexX = -1; int32 IndexY = -1;
	for (const FIntPoint& Element : Shape)
	{
		X.Add(Element.X);
		Y.Add(Element.Y);
	}
	SpanX = FindMaxOfArray(X, IndexX);
	SpanY = FindMaxOfArray(Y, IndexY);

	// 2x2 Size block: Coordinates span from 0..1, its max value would say 1 instead of 2, add 1.
	return FIntPoint(SpanX + 1, SpanY + 1);
}

FIntPoint UDieg_UtilityLibrary::GetMinSpan(const TArray<FIntPoint>& Shape)
{
	TArray<int32> X, Y;
	int32 MinX = 0, MinY = 0;
	for (const FIntPoint& Element : Shape)
	{
		X.Add(Element.X);
		Y.Add(Element.Y);
	}
	FindMinOfArray(X, MinX);
	FindMinOfArray(Y, MinY);

	return FIntPoint(MinX, MinY);

}
FVector2D UDieg_UtilityLibrary::Rotate2D(const FVector2D& Coordinates, float AngleDegrees, bool bRelativeToOrigin)
{
	/**
	 * Rotates a 2D coordinate by the given angle.
	 *
	 * bRelativeToOrigin = true → Uses Unreal's default GetRotated()
	 *     - Rotation is always relative to the global origin (0,0).
	 *     - Example: A shape offset to (5,5) will "swing around" the origin when rotated.
	 *       Good for general 2D math, but not for grid-based shapes.
	 *
	 * bRelativeToOrigin = false → Uses custom grid rotation
	 *     - Rotation is relative to the coordinate itself (like rotating in place on a grid).
	 *     - Example: A Tetris "L" block at (5,5) rotates around its own local pivot
	 *       without drifting across the grid.
	 *
	 * Example with an "L" shape:
	 *
	 * Original L at (0,0):
	 *   X
	 *   X
	 *   X X
	 *
	 * Rotate 90° (origin-based):
	 *   X X X
	 *   X
	 *   (works fine at origin)
	 *
	 * Same L at (5,5):
	 *   (5,5), (5,6), (5,7), (6,7)
	 *
	 * Rotate 90° with origin-based rotation:
	 *   Shape moves away from (5,5), "orbiting" around (0,0).
	 *
	 * Rotate 90° with grid-relative rotation:
	 *   Shape stays in place at (5,5), rotated correctly like in Tetris.
	 */

	if (bRelativeToOrigin)
	{
		// Uses Unreal's built-in rotation around (0,0)
		return Coordinates.GetRotated(AngleDegrees);
	}

	// Normalize angle to [0, 360)
	const float NormalizedAngle = FMath::Fmod(AngleDegrees, 360.0f);

	// Handle discrete right-angle rotations (grid-friendly, no trig needed)
	if (FMath::IsNearlyEqual(NormalizedAngle, 0.0f))
	{
		return Coordinates; // no rotation
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 90.0f))
	{
		return FVector2D(-Coordinates.Y, Coordinates.X);
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 180.0f))
	{
		return FVector2D(-Coordinates.X, -Coordinates.Y);
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 270.0f) || FMath::IsNearlyEqual(NormalizedAngle, -90.0f))
	{
		return FVector2D(Coordinates.Y, -Coordinates.X);
	}

	// Fallback: unsupported angle → no rotation (grid logic usually doesn't need this)
	return Coordinates;
}

TArray<FIntPoint> UDieg_UtilityLibrary::Rotate2DArray(const TArray<FIntPoint>& Shape, float AngleDegrees)
{
	TArray<FIntPoint> RotatedShape;
	RotatedShape.Reserve(Shape.Num());

	//Rotate each point
	for (const FIntPoint& Point : Shape)
	{
		const FVector2D RotatedF = Rotate2D(Point, AngleDegrees);
		const FIntPoint RotatedI(
			FMath::RoundToInt(RotatedF.X),
			FMath::RoundToInt(RotatedF.Y)
		);

		RotatedShape.Add(RotatedI);
	}

	// Using proper bounding box calculation instead of min/max tracking
	FIntPoint MinBounds = GetMinSpan(RotatedShape);
	if (MinBounds.X != 0 || MinBounds.Y != 0)
	{
		for (FIntPoint& Point : RotatedShape)
		{
			Point -= MinBounds;
		}
	}

	return RotatedShape;
}

TArray<FIntPoint> UDieg_UtilityLibrary::Rotate2DArrayWithRoot(const TArray<FIntPoint>& Shape, const float AngleDegrees,
	const FIntPoint& RootIn, FIntPoint& RootOut)
{
	// If no rotation, return the shape as-is with original root
	if (FMath::IsNearlyEqual(AngleDegrees, 0.0f))
	{
		RootOut = RootIn;
		return Shape;
	}

	TArray<FIntPoint> RotatedShape;
	RotatedShape.Reserve(Shape.Num());

	// Rotate each point
	for (const FIntPoint& Point : Shape)
	{
		const FVector2D RotatedF = Rotate2D(Point, AngleDegrees, false);
		const FIntPoint RotatedI(
			FMath::RoundToInt(RotatedF.X),
			FMath::RoundToInt(RotatedF.Y)
		);
		RotatedShape.Add(RotatedI);
	}

	// Rotate the root
	{
		const FVector2D RotatedRootF = Rotate2D(RootIn, AngleDegrees, false);
		RootOut = FIntPoint(
			FMath::RoundToInt(RotatedRootF.X),
			FMath::RoundToInt(RotatedRootF.Y)
		);
	}

	// Using proper bounding box calculation instead of min/max tracking
	FIntPoint MinBounds = GetMinSpan(RotatedShape);
	if (MinBounds.X != 0 || MinBounds.Y != 0)
	{
		for (FIntPoint& P : RotatedShape)
		{
			P -= MinBounds;
		}
		RootOut -= MinBounds;
	}

	return RotatedShape;
}

FIntPoint UDieg_UtilityLibrary::GetOffsetBasedOnRotation(float AngleDegrees)
{
	// Normalize angle to [0, 360)
	const float NormalizedAngle = FMath::Fmod(AngleDegrees, 360.0f);

	// Handle discrete right-angle rotations (grid-friendly, no trig needed)
	if (FMath::IsNearlyEqual(NormalizedAngle, 0.0f))
	{
		return FIntPoint(0, 1); // no rotation
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 90.0f))
	{
		return FIntPoint(1, 1);
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 180.0f))
	{
		return FIntPoint(1, 0);
	}
	if (FMath::IsNearlyEqual(NormalizedAngle, 270.0f) || FMath::IsNearlyEqual(NormalizedAngle, -90.0f))
	{
		return FIntPoint(0, 0);
	}
	return FIntPoint(0, 1); // no rotation
}

// FVector2D UDieg_UtilityLibrary::Rotate2D(const FVector2D& Coordinates, const float AngleDegrees, bool DefaultImplementation)
// {
// 	FVector2D RotatedVector = FVector2D::ZeroVector;
// 	
// 	// Relative to {0,0}
// 	if (DefaultImplementation)
// 	{
// 		// Alternative is UKismetMathLibrary::GetRotated2D(), Rotates around axis (0,0,1)
// 		RotatedVector = Coordinates.GetRotated(AngleDegrees);
// 	}
// 	else
// 	{
// 		FVector2D TempVec1 = Coordinates;
// 		if (AngleDegrees == 90.0f)
// 		{
// 			// Invert, then X*-1
// 			TempVec1 = FVector2D(Coordinates.Y * -1, Coordinates.X);
// 		}
// 		else
// 		{
// 			TempVec1 = Coordinates;
// 		}
//
// 		FVector2D TempVec2 = FVector2D::ZeroVector;
// 		if (AngleDegrees == -90.0f)
// 		{
// 			// Invert, then Y*-1
// 			TempVec2 = FVector2D(Coordinates.Y, Coordinates.X * -1);
// 		}
// 		else
// 		{
// 			TempVec2 = TempVec1;
// 		}
// 		
// 		FVector2D TempVec3 = FVector2D::ZeroVector;
// 		if (AngleDegrees == 180.0f)
// 		{
// 			// Invert, then Y*-1
// 			TempVec3 = FVector2D(Coordinates.X * -1, Coordinates.Y * -1);
// 		}
// 		else
// 		{
// 			TempVec3 = TempVec2;
// 		}
//
// 		RotatedVector = TempVec3;
// 	}
// 	
// 	return RotatedVector;
// }
