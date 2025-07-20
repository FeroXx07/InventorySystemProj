// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_DoubleLogger.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_DoubleLogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void Log(const FString& ContentStr);
	static void LogWarning(const FString& ContentStr);
	static void LogError(const FString& ContentStr);
};
