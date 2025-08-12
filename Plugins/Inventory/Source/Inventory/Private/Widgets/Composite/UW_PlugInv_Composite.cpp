// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/UW_PlugInv_Composite.h"

#include "Blueprint/WidgetTree.h"

void UPlugInv_Composite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UPlugInv_CompositeBase* Composite = Cast<UPlugInv_CompositeBase>(Widget); IsValid(Composite))
		{
			Children.Add(Composite);
			Composite->Collapse();
		}
	});
}

void UPlugInv_Composite::ApplyFunction(FUncType Function)
{
	for (const TObjectPtr<UPlugInv_CompositeBase>& Child : Children)
	{
		Child->ApplyFunction(Function);
	}
}

void UPlugInv_Composite::Collapse()
{
	for (const TObjectPtr<UPlugInv_CompositeBase>& Child : Children)
	{
		Child->Collapse();
	}
}
