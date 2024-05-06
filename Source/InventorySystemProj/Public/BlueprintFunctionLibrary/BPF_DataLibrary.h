// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_DataLibrary.generated.h"

/**
 * 
 */

class UItemCore;
USTRUCT(BlueprintType)
struct INVENTORYSYSTEMPROJ_API FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TObjectPtr<UTexture2D> IconImage;

	UPROPERTY(EditAnywhere, Category = "Item")
	FIntVector2 SlotsSize;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	int32 MaxStacks;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TSubclassOf<AActor> PhysicalActor;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TArray<TSubclassOf<UObject>> LogicDrivers;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TArray<TSubclassOf<UUserWidget>> UserWidgets;
};

UCLASS(Blueprintable)
class INVENTORYSYSTEMPROJ_API UInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TObjectPtr<UItemCore> DataAssetItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ContainerIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Stacks;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int64 UniqueID;
};

UENUM()
enum class EContainerType : uint8
{
	Grid UMETA(DisplayName = "Grid"),
	List UMETA(DisplayName = "List"),
};

USTRUCT(BlueprintType)
struct INVENTORYSYSTEMPROJ_API FInventoryContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Container")
	EContainerType ContainerType = EContainerType::Grid;
	
	UPROPERTY(EditAnywhere, Category = "Container")
	FIntVector2 SlotsSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container")
	int64 UniqueID;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Inventory")
	TArray<TObjectPtr<UInventoryItem>> InventoryItems;
	
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Item")
	TArray<TSubclassOf<UUserWidget>> UserWidgets;
};

UCLASS()
class INVENTORYSYSTEMPROJ_API UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
