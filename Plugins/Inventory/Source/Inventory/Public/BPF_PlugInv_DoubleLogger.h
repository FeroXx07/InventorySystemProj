// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_DoubleLogger.generated.h"

#define DrawLogScreenTime 20.f

#define LOG_DOUBLE_S(DrawTime, Color, FormatStr, ...) \
do { \
FString ScreenStr = UPlugInv_DoubleLogger::FormatText(TEXT(FormatStr), ##__VA_ARGS__); \
UPlugInv_DoubleLogger::DrawToScreen(ScreenStr, Color, DrawTime); \
UE_LOG(LogInventory, Display, TEXT("%s"), *ScreenStr); \
} while (0)

#define LOG_DOUBLE_WARNING_S(DrawTime, FormatStr, ...) \
do { \
FString ScreenStr = UPlugInv_DoubleLogger::FormatText(TEXT(FormatStr), ##__VA_ARGS__); \
UPlugInv_DoubleLogger::DrawToScreen(ScreenStr, FColor::Yellow, DrawTime); \
UE_LOG(LogInventory, Warning, TEXT("%s"), *ScreenStr); \
} while (0)

#define LOG_DOUBLE_ERROR_S(DrawTime, FormatStr, ...) \
do { \
FString ScreenStr = UPlugInv_DoubleLogger::FormatText(TEXT(FormatStr), ##__VA_ARGS__); \
UPlugInv_DoubleLogger::DrawToScreen(ScreenStr, FColor::Red, DrawTime); \
UE_LOG(LogInventory, Error, TEXT("%s"), *ScreenStr); \
} while (0)



static FFormatArgumentValue ToFormatArg(int32 Val) { return FFormatArgumentValue(Val); }
static FFormatArgumentValue ToFormatArg(float Val) { return FFormatArgumentValue(Val); }
static FFormatArgumentValue ToFormatArg(bool Val) { return FFormatArgumentValue(FText::FromString(Val ? TEXT("true") : TEXT("false"))); }
static FFormatArgumentValue ToFormatArg(const FString& Val) { return FFormatArgumentValue(FText::FromString(Val)); }
static FFormatArgumentValue ToFormatArg(const FText& Val) { return FFormatArgumentValue(Val); }

static FFormatArgumentValue ToFormatArg(const FVector2D& Val)
{
	return FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("X: %.2f, Y: %.2f"), Val.X, Val.Y)));
}
static FFormatArgumentValue ToFormatArg(const FVector& Val)
{
	return FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("X: %.2f, Y: %.2f, Z: %.2f"), Val.X, Val.Y, Val.Z)));
}
static FFormatArgumentValue ToFormatArg(const FIntPoint& Val)
{
	return FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("X: %d, Y: %d"), Val.X, Val.Y)));
}

template<typename T>
static FFormatArgumentValue ToFormatArg(const T& Val)
{
	return FFormatArgumentValue(FString::Printf(TEXT("%s"), *LexToString(Val)));
}


/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_DoubleLogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void Log(const FString& ContentStr, FColor Color = FColor::Purple);
	static void LogWarning(const FString& ContentStr);
	static void LogError(const FString& ContentStr);

	static void DrawToScreen(const FString& ContentStr, FColor Color, float DisplayTime);
	
	template<typename... Args>
	static FString FormatText(const TCHAR* FormatStr, Args&&... args)
	{
		TArray<FFormatArgumentValue> FormatArgs = { ::ToFormatArg(args)... };
		FTextFormat TextFormat = FTextFormat::FromString(FString(FormatStr));
		FText Formatted = FText::Format(TextFormat, FormatArgs);
		return Formatted.ToString();
	}
	
};
