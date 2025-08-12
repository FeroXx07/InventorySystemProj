// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_PlugInv_Leaf.h"
#include "UW_PlugInv_Leaf_LabeledValue.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_Leaf_LabeledValue : public UPlugInv_Leaf
{
	GENERATED_BODY()
public:

	void SetText_Label(const FText& Text, bool bCollapse) const;
	void SetText_Value(const FText& Text, bool bCollapse) const;
	virtual void NativePreConstruct() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Label;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Value;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize_Label{18};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize_Value{12};
};
