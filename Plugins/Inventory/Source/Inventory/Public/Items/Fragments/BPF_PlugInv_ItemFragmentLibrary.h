// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_PlugInv_ItemFragmentLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_ItemFragmentLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

/**
 * Any trait of an Item can be defined as what is known in game dev as Item Fragment.
 */
USTRUCT(BlueprintType)
struct INVENTORY_API FPlugInv_ItemFragment
{
	GENERATED_BODY()
	
	// Since inheritance is key to Item Fragments and structs are also key due to their lightweight,
	// when destroying child structs manage correctly polymorphic destruction.
	// 5 special member functions, since we need the virtual destructor but that will disable by default, the other 4 special member functions
	// Solution to this is to define these 4 to default behaviors like below:
	FPlugInv_ItemFragment(){}
	
	FPlugInv_ItemFragment(const FPlugInv_ItemFragment&) = default; // Copy operation with const ref
	FPlugInv_ItemFragment& operator=(const FPlugInv_ItemFragment&) = default; // Copy operation 
	FPlugInv_ItemFragment(FPlugInv_ItemFragment&&) = default; // Move operation with universal ref (&&)
	FPlugInv_ItemFragment& operator=(FPlugInv_ItemFragment&&) = default; // Move operation with universal ref (&&)
	virtual ~FPlugInv_ItemFragment() {} // Destructor

	const FGameplayTag& GetFragmentTag() const
	{
		return FragmentTag;
	}

	void SetFragmentTag(const FGameplayTag& FragmentTag_)
	{
		this->FragmentTag = FragmentTag_;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FPlugInv_GridFragment : public FPlugInv_ItemFragment
{
	GENERATED_BODY()

	const FIntPoint& GetGridSize() const
	{
		return GridSize;
	}

	void SetGridSize(const FIntPoint& GridSize_)
	{
		this->GridSize = GridSize_;
	}

	float GetGridPadding() const
	{
		return GridPadding;
	}

	void SetGridPadding(const float GridPadding_)
	{
		this->GridPadding = GridPadding_;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1, 1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{0.0f};


};

USTRUCT(BlueprintType)
struct FPlugInv_ImageFragment : public FPlugInv_ItemFragment
{
	GENERATED_BODY()
	
	const TObjectPtr<UTexture2D>& GetIcon() const
	{
		return IconTexture;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> IconTexture{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimensions{44.f, 44.f};
};

USTRUCT(BlueprintType)
struct FPlugInv_StackableFragment : public FPlugInv_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const
	{
		return MaxStackSize;
	}

	void SetMaxStackSize(const int32 NewMaxStackSize)
	{
		this->MaxStackSize = NewMaxStackSize;
	}
	
	int32 GetStackCount() const
	{
		return StackCount;
	}

	void SetStackCount(const int32 NewStackCount)
	{
		this->StackCount = NewStackCount;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};
};