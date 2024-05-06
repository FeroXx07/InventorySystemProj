// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActorComponent/AC_ItemDriver.h"
#include "O_ItemDriverData.generated.h"

UENUM()
enum class EInstanceType : uint8
{
	SinglePerActor UMETA(DisplayName = "SinglePerActor"),
	SinglePerUse UMETA(DisplayName = "SinglePerUse"),
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEMPROJ_API UItemDriverData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UItemDriver> ItemDriver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	EInstanceType InstanceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FGameplayTagContainer TagContainer;
};
