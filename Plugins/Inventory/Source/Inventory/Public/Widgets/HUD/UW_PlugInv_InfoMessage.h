// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlugInv_InfoMessage.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UPlugInv_InfoMessage : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void MessageHide();

	void SetMessage(const FString& Message);

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MessageLifetime{3.f};

	FTimerHandle MessageTimer;
	bool bIsMessageActive{false};
};
