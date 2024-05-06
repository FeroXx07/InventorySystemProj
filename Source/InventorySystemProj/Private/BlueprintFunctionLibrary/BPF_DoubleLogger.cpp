// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/BPF_DoubleLogger.h"

void UDoubleLogger::Log(const FString& ContentStr)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *ContentStr);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ContentStr);
}
void UDoubleLogger::LogWarning(const FString& ContentStr)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *ContentStr);	if (GEngine)
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ContentStr);
}
void UDoubleLogger::LogError(const FString& ContentStr)
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *ContentStr);	if (GEngine)
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, ContentStr);
}
