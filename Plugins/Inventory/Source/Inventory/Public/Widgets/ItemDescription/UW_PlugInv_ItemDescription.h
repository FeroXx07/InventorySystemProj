// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Composite/UW_PlugInv_Composite.h"
#include "UW_PlugInv_ItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_ItemDescription : public UPlugInv_Composite
{
	GENERATED_BODY()

public:
	FVector2D GetBoxSize() const;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
