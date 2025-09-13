// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dieg_InteractWidget.generated.h"

class UOverlay;
class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_InteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game|Dieg|Interact Widget|Properties", meta = (AllowPrivateAccess = "true"))
	FVector WorldLocation;

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	void SetWorldPosition(const FVector& NewPosition);

	UFUNCTION(BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	void SetInteractText(const FText& Text);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game|Dieg|Interact Widget")
	bool SetWidgetPositionFromWorldPosition(const FVector& WorldPosition);
};
