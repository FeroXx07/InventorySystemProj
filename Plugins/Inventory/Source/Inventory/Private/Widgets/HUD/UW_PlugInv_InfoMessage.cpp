// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/UW_PlugInv_InfoMessage.h"

#include "Components/TextBlock.h"

void UPlugInv_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

void UPlugInv_InfoMessage::SetMessage(const FString& Message)
{
	Text_Message->SetText(FText::FromString(Message));

	if (!bIsMessageActive)
	{
		MessageShow();
	}

	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
	{
		MessageHide();
		bIsMessageActive = false;
	}, MessageLifetime, false);
}
