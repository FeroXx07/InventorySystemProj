// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_PlugInv_Leaf.h"
#include "UW_PlugInv_Leaf_Text.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_Leaf_Text : public UPlugInv_Leaf
{
	GENERATED_BODY()
public:
	void SetText(const FText& Text) const;
	virtual void NativePreConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_LeafText;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 FontSize{12};
};
