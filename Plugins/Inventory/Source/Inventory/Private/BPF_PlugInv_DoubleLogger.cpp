// Fill out your copyright notice in the Description page of Project Settings.


#include "BPF_PlugInv_DoubleLogger.h"

#include "Inventory.h"

void UPlugInv_DoubleLogger::Log(const FString& ContentStr)
{
	UE_LOG(LogInventory, Display, TEXT("%s"), *ContentStr);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ContentStr);
}

void UPlugInv_DoubleLogger::LogWarning(const FString& ContentStr)
{
	UE_LOG(LogInventory, Warning, TEXT("%s"), *ContentStr);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, ContentStr);
}

void UPlugInv_DoubleLogger::LogError(const FString& ContentStr)
{
	UE_LOG(LogInventory, Error, TEXT("%s"), *ContentStr);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ContentStr);
}


