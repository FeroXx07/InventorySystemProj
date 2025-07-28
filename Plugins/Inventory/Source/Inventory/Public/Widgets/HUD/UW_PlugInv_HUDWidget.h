// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlugInv_HUDWidget.generated.h"

class UPlugInv_InfoMessage;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowPickupMessage(const FString& Message);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HidePickupMessage();

	private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPlugInv_InfoMessage> InfoMessage;

	UFUNCTION()
	void OnNoRoom();
};
