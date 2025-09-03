// Fill out your copyright notice in the Description page of Project Settings.


#include "BPF_PlugInv_DoubleLogger.h"
#include "Logging/StructuredLog.h"

#include "Inventory.h"

void UPlugInv_DoubleLogger::Log(const FString& ContentStr, const FColor Color)
{
	UE_LOG(LogInventory, Display, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, Color, 5.f);
}

void UPlugInv_DoubleLogger::LogWarning(const FString& ContentStr)
{
	UE_LOG(LogInventory, Warning, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, FColor::Yellow, 5.f);
}

void UPlugInv_DoubleLogger::LogError(const FString& ContentStr)
{
	UE_LOG(LogInventory, Error, TEXT("%s"), *ContentStr);
	DrawToScreen(ContentStr, FColor::Red, 5.f);
}

void UPlugInv_DoubleLogger::DrawToScreen(const FString& ContentStr, const FColor Color, const float DisplayTime)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color, ContentStr);
}

