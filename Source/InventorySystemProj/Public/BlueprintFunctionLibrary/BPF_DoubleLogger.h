// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogVerbosity.h"
#include "BPF_DoubleLogger.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMPROJ_API UDoubleLogger : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void Log(const FString& Content);
	static void LogWarning(const FString& Content);
	static void LogError(const FString& Content);
};
