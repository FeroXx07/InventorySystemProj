// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SinglePropertyViewWrapper.generated.h"

class USinglePropertyView;
class UTextBlock;
class UVerticalBox;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_SinglePropertyViewWrapper : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> Text_Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USinglePropertyView> SinglePropertyView;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Editor|Inv")
	void InitializeProperties(UObject* OuterObject, const FName& PropertyName);
};
