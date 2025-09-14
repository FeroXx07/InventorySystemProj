// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_DoubleLogger.generated.h"

#define DrawLogScreenTime 20.f

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

// TArray support
template<typename T>
static FFormatArgumentValue ToFormatArg(const TArray<T>& Array)
{
	TArray<FString> StrElements;
	StrElements.Reserve(Array.Num());

	for (const T& Elem : Array)
	{
		// Recursively call ToFormatArg on each element and convert to string
		FFormatArgumentValue ArgVal = ::ToFormatArg(Elem);
		StrElements.Add(ArgVal.GetTextValue().ToString());
	}

	FString Combined = FString::Join(StrElements, TEXT(", "));
	return FFormatArgumentValue(FText::FromString(FString::Printf(TEXT("[%s]"), *Combined)));
}


// For all non-enum types
template<typename T>
static typename TEnableIf<!TIsEnum<T>::Value, FFormatArgumentValue>::Type
ToFormatArg(const T& Val)
{
	return FFormatArgumentValue(FString::Printf(TEXT("%s"), *LexToString(Val)));
}


// For enum types with UENUM reflection
template<typename T>
static typename TEnableIf<TIsEnum<T>::Value, FFormatArgumentValue>::Type
ToFormatArg(const T& Val)
{
	if (const UEnum* EnumPtr = StaticEnum<T>())
	{
		return FFormatArgumentValue(FText::FromString(EnumPtr->GetNameStringByValue((int64)Val)));
		// Or, if you want the UMETA(DisplayName):
		//return FFormatArgumentValue(EnumPtr->GetDisplayNameTextByValue((int64)Val));
	}

	return FFormatArgumentValue(FText::FromString("Invalid Enum"));
}


/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_DoubleLogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void Log(const FString& ContentStr, FColor Color = FColor::Purple, float Duration = 5.0f);
	template<typename... Args>
	static void Log(float Duration, const TCHAR* FormatStr, FColor Color, Args&&... args);

	static void LogWarning(const FString& ContentStr, float Duration = 5.0f);
	template<typename... Args>
	static void LogWarning(float Duration, const TCHAR* FormatStr, Args&&... args);
	
	static void LogError(const FString& ContentStr, float Duration = 5.0f);
	template<typename... Args>
	static void LogError(float Duration, const TCHAR* FormatStr, Args&&... args);
	
	static void DrawToScreen(const FString& ContentStr, FColor Color, float DisplayTime);
	
	template<typename... Args>
	static FString FormatText(const TCHAR* FormatStr, Args&&... args);
	
};

template <typename ... Args>
void UPlugInv_DoubleLogger::Log(float Duration, const TCHAR* FormatStr, FColor Color, Args&&... args)
{
	FString Formatted = FormatText(FormatStr, Forward<Args>(args)...);
	Log(Formatted, Color, Duration); 
}

template <typename ... Args>
void UPlugInv_DoubleLogger::LogWarning(float Duration, const TCHAR* FormatStr, Args&&... args)
{
	FString Formatted = FormatText(FormatStr, Forward<Args>(args)...);
	Log(Formatted, FColor::Yellow, Duration); 
}

template <typename ... Args>
void UPlugInv_DoubleLogger::LogError(float Duration, const TCHAR* FormatStr, Args&&... args)
{
	FString Formatted = FormatText(FormatStr, Forward<Args>(args)...);
	Log(Formatted, FColor::Red, Duration);
}

template <typename ... Args>
FString UPlugInv_DoubleLogger::FormatText(const TCHAR* FormatStr, Args&&... args)
{
	TArray<FFormatArgumentValue> FormatArgs = { ::ToFormatArg(args)... };
	FTextFormat TextFormat = FTextFormat::FromString(FString(FormatStr));
	FText Formatted = FText::Format(TextFormat, FormatArgs);
	return Formatted.ToString();
}
