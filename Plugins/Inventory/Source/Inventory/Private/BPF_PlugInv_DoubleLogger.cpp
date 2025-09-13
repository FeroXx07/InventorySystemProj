// Fill out your copyright notice in the Description page of Project Settings.


#include "BPF_PlugInv_DoubleLogger.h"
#include "Logging/StructuredLog.h"

#include "Inventory.h"

void UPlugInv_DoubleLogger::Log(const FString& ContentStr, FColor Color, float Duration)
{
	UE_LOG(LogInventory, Display, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, Color, Duration);
}

void UPlugInv_DoubleLogger::LogWarning(const FString& ContentStr, float Duration)
{
	UE_LOG(LogInventory, Warning, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, FColor::Yellow, Duration);
}

void UPlugInv_DoubleLogger::LogError(const FString& ContentStr, float Duration)
{
	UE_LOG(LogInventory, Error, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, FColor::Red, Duration);
}

void UPlugInv_DoubleLogger::DrawToScreen(const FString& ContentStr, const FColor Color, const float DisplayTime)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color, ContentStr);
}

