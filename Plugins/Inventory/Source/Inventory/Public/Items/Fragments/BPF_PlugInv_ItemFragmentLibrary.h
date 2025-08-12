// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructUtils/InstancedStruct.h"
#include "BPF_PlugInv_ItemFragmentLibrary.generated.h"

class UPlugInv_CompositeBase;
class APlayerController;
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

	virtual void Manifest() {}

	const FGameplayTag& GetFragmentTag() const
	{
		return FragmentTag;
	}

	void SetFragmentTag(const FGameplayTag& FragmentTag_)
	{
		this->FragmentTag = FragmentTag_;
	}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="FragmentTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

/*
 * Item fragment specifically for assimilation into a widget.
 */
USTRUCT(BlueprintType)
struct FPlugInv_InventoryItemFragment : public FPlugInv_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UPlugInv_CompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UPlugInv_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType)
struct FPlugInv_TextFragment : public FPlugInv_InventoryItemFragment
{
	GENERATED_BODY()

	FText GetText() const { return FragmentText; }
	void SetText(const FText& Text) { FragmentText = Text; }
	virtual void Assimilate(UPlugInv_CompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;
};

USTRUCT(BlueprintType)
struct FPlugInv_LabeledNumberFragment : public FPlugInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UPlugInv_CompositeBase* Composite) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
	// When manifesting for the first time, this fragment will randomize. However, onee equipped
	// and dropped, an item should retain the same value, so randomization should not occur.
	bool bRandomizeOnManifest{true};

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText Text_Label{};

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Min{0};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Max{0};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{1};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};
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
struct FPlugInv_ImageFragment : public FPlugInv_InventoryItemFragment
{
	GENERATED_BODY()
	
	const TObjectPtr<UTexture2D>& GetIcon() const
	{
		return IconTexture;
	}

	virtual void Assimilate(UPlugInv_CompositeBase* Composite) const override;

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

USTRUCT(BlueprintType)
struct FPlugInv_ConsumeModifier : public FPlugInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}
};

USTRUCT(BlueprintType)
struct FPlugInv_ConsumableFragment : public FPlugInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC);
	virtual void Assimilate(UPlugInv_CompositeBase* Composite) const override;
	virtual void Manifest() override;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FPlugInv_ConsumeModifier>> ConsumeModifiers;
};

USTRUCT(BlueprintType)
struct FPlugInv_HealthPotionFragment : public FPlugInv_ConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FPlugInv_ManaPotionFragment : public FPlugInv_ConsumeModifier
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) override;
};