// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inv_SinglePropertyViewWrapper.h"

#include "Components/SinglePropertyView.h"
#include "Components/TextBlock.h"

void UInv_SinglePropertyViewWrapper::InitializeProperties_Implementation(UObject* OuterObject, const FName& PropertyName)
{
	if (!IsValid(SinglePropertyView))
	{
		return;
	}
	
	if (SinglePropertyView->GetPropertyName().IsNone() || SinglePropertyView->GetObject() == nullptr)
	{
		Text_Label->SetText(FText::FromName(PropertyName));
		SinglePropertyView->SetObject(OuterObject);
		SinglePropertyView->SetPropertyName(PropertyName);
		SinglePropertyView->SetNameOverride(FText::FromName(PropertyName));
	}
}
