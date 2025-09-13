// Fill out your copyright notice in the Description page of Project Settings.


#include "Diegetic/Widgets/Dieg_ItemCreatorEuw.h"

#include "AssetToolsModule.h"
#include "BPF_PlugInv_DoubleLogger.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/CanvasPanel.h"
#include "Diegetic/Dieg_UtilityLibrary.h"
#include "Diegetic/UObjects/Dieg_ItemDefinitionDataAsset.h"
#include "Diegetic/UStructs/Dieg_ItemDefinition.h"
#include "Diegetic/Widgets/Dieg_Slot.h"
#include "Factories/BlueprintFactory.h"
#include "Factories/DataAssetFactory.h"
#include "UObject/SavePackage.h"

void UDieg_ItemCreatorEuw::NativePreConstruct()
{
	Super::NativePreConstruct();

	
}

void UDieg_ItemCreatorEuw::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(SinglePropertyView_Name) || !IsValid(SinglePropertyView_StaticMesh) || !IsValid(SinglePropertyView_MaxQuantity))
	{
		return;
	}
	
	// To bind to SinglePropertyView it caches its variable name. 
	if (SinglePropertyView_Name->GetPropertyName().IsNone() || SinglePropertyView_StaticMesh->GetPropertyName().IsNone() ||
		SinglePropertyView_Name->GetObject() == nullptr || SinglePropertyView_StaticMesh->GetObject() == nullptr
		|| SinglePropertyView_MaxQuantity->GetPropertyName().IsNone() || SinglePropertyView_MaxQuantity->GetObject() == nullptr)
	{
		SinglePropertyView_Name->SetObject(this);
		SinglePropertyView_Name->SetPropertyName("Property_Name");
		SinglePropertyView_StaticMesh->SetObject(this);
		SinglePropertyView_StaticMesh->SetPropertyName("Property_StaticMesh");
		SinglePropertyView_MaxQuantity->SetObject(this);
		SinglePropertyView_MaxQuantity->SetPropertyName("Property_MaxQuantity");
	}
	
	if (IsDesignTime())
		return;
	
	// CanvasPanel_Modify->SetVisibility(ESlateVisibility::Collapsed);
	SetupGrid(GetGridSize());

	if (!GridWidget->OnGridSlotHover.IsAlreadyBound(this, &ThisClass::GridSlotHover))
		GridWidget->OnGridSlotHover.AddDynamic(this, &ThisClass::GridSlotHover);

	if (!GridWidget->OnGridSlotUnHover.IsAlreadyBound(this, &ThisClass::GridSlotUnHovered))
		GridWidget->OnGridSlotUnHover.AddDynamic(this, &ThisClass::GridSlotUnHovered);
	
	if (!GridWidget->OnGridSlotPressed.IsAlreadyBound(this, &ThisClass::GridSlotPressed))
		GridWidget->OnGridSlotPressed.AddDynamic(this, &ThisClass::GridSlotPressed);

	if (!GridWidget->OnGridSlotReleased.IsAlreadyBound(this, &ThisClass::GridSlotReleased))
		GridWidget->OnGridSlotReleased.AddDynamic(this, &ThisClass::GridSlotReleased);

	if (!Button_Modify->OnClicked.IsAlreadyBound(this, &ThisClass::HandleButtonModifyClicked))
		Button_Modify->OnClicked.AddDynamic(this, &ThisClass::HandleButtonModifyClicked);
	
	if (!Button_Confirm->OnClicked.IsAlreadyBound(this, &ThisClass::HandleButtonConfirmClicked))
		Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::HandleButtonConfirmClicked);

	if (!ComboBox_DataAsset->OnSelectionChanged.IsAlreadyBound(this, &ThisClass::HandleComboBoxDaChange))
		ComboBox_DataAsset->OnSelectionChanged.AddDynamic(this, &ThisClass::HandleComboBoxDaChange);

	if (!ComboBox_DataAsset->OnOpening.IsAlreadyBound(this, &ThisClass::HandleComboBoxDaOpening))
		ComboBox_DataAsset->OnOpening.AddDynamic(this, &ThisClass::HandleComboBoxDaOpening);

	if (!SpinBox_GridSize->OnValueChanged.IsAlreadyBound(this, &ThisClass::HandleSpinBoxGridSizeChange))
		SpinBox_GridSize->OnValueChanged.AddDynamic(this, &ThisClass::HandleSpinBoxGridSizeChange);
	
	SpinBox_GridSize->SetDelta(1.0f);
	
	RefreshItemList();
}

void UDieg_ItemCreatorEuw::SetupGrid(int32 GridSize) const
{
	GridWidget->CreateEmptyGrid(GridSize * GridSize, GridSize);
}

void UDieg_ItemCreatorEuw::ModifyDataAsset()
{
	UDieg_ItemDefinitionDataAsset* SelectedItemDataAsset = GetSelectedDataAsset();
	FDieg_ItemDefinition& ItemDefinition = SelectedItemDataAsset->ItemDefinition;
	// Set properties
	ItemDefinition.Name = Property_Name;
	ItemDefinition.WorldMesh = Property_StaticMesh;
	ItemDefinition.DefaultShape = SelectedSlots.Array();
	ItemDefinition.StackSizeMax = Property_MaxQuantity;
	
	// Update the shape root after setting the shape
	UDieg_ItemDefinitionDataAsset::SetItemDefinitionShapeRoot(ItemDefinition);
		
	// Mark dirty so it can be saved
	SelectedItemDataAsset->MarkPackageDirty();

	// Get the asset's package
	UPackage* Package = SelectedItemDataAsset->GetOutermost();

	// Save
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error = nullptr;
	SaveArgs.bForceByteSwapping = false;
	SaveArgs.bWarnOfLongFilename = true;

	// Convert package name to a filename
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(
		Package->GetName(),
		FPackageName::GetAssetPackageExtension()
	);

	// Save the package 
	UPackage::SavePackage(Package, SelectedItemDataAsset, *PackageFileName, SaveArgs);
}

void UDieg_ItemCreatorEuw::CreateDataAsset()
{
	const FString AssetName = Property_Name.ToString().Replace(TEXT(" "), TEXT("_"));

	// Load necessary modules
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Scoped transaction (UE 5.6 way)
	const FScopedTransaction Transaction(NSLOCTEXT("Dieg", "CreateItemAsset", "Create New Item Asset"));

	// Add prefix
	const FString PrefixedAssetName = ItemsDataAssetsPrefix + AssetName;

	// Generate unique name
	FString Name, PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(ItemsDataAssetsStoragePath.Path / PrefixedAssetName, TEXT(""), PackageName, Name);
	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

	// Create package
	UPackage* Package = CreatePackage(*PackageName);

	// Use factory for data asset
	UDataAssetFactory* Factory = NewObject<UDataAssetFactory>();
	UClass* Class = DataAssetClass.Get();
	Factory->DataAssetClass = Class;

	UObject* NewObject = AssetToolsModule.Get().CreateAsset(Name, PackagePath, Class, Factory);

	if (UDieg_ItemDefinitionDataAsset* ItemDataAsset = Cast<UDieg_ItemDefinitionDataAsset>(NewObject))
	{
		FDieg_ItemDefinition& ItemDefinition = ItemDataAsset->ItemDefinition;

		// Set properties
		ItemDefinition.Name = Property_Name;
		ItemDefinition.WorldMesh = Property_StaticMesh;
		ItemDefinition.DefaultShape = SelectedSlots.Array();
		ItemDefinition.StackSizeMax = Property_MaxQuantity;
		
		// Update the shape root after setting the shape
		UDieg_ItemDefinitionDataAsset::SetItemDefinitionShapeRoot(ItemDefinition);
		
		// Mark dirty so it can be saved
		ItemDataAsset->MarkPackageDirty();

		// Save
		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.Error = nullptr;
		SaveArgs.bForceByteSwapping = false;
		SaveArgs.bWarnOfLongFilename = true;
		const FString Filename = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
		UPackage::Save(Package, NewObject, *Filename, SaveArgs);
		
		// Inform asset registry
		AssetRegistry.AssetCreated(NewObject);
		// Tell content browser to show the newly created asset
		TArray<UObject*> Objects;
		Objects.Add(NewObject);
		ContentBrowserModule.Get().SyncBrowserToAssets(Objects);

		RefreshItemList();

		SelectDataAsset(Property_Name.ToString());
	}
}


void UDieg_ItemCreatorEuw::SelectDataAsset(const FString& Option)
{
	FText SelectedDataAssetName = FText::GetEmpty();
	UStaticMesh* SelectedDataAssetMesh = nullptr;
	float ItemSize = 3.0f;
	int32 MaxQuantity = 1;
	TArray<FIntPoint> Shape;
	
	if (Option != EmptySelectionOption)
	{
		ComboBox_DataAsset->SetSelectedOption(Option);
		const UDieg_ItemDefinitionDataAsset* SelectedDataAsset = GetSelectedDataAsset();
		const FDieg_ItemDefinition& ItemDefinition = SelectedDataAsset->ItemDefinition;

		SelectedDataAssetName = ItemDefinition.Name;

		if (ItemDefinition.WorldMesh.IsValid())
		{
			SelectedDataAssetMesh = ItemDefinition.WorldMesh.Get();
		}
		else
		{
			SelectedDataAssetMesh = ItemDefinition.WorldMesh.LoadSynchronous();
		}
		
		MaxQuantity = ItemDefinition.StackSizeMax;
		
		if (!ItemDefinition.DefaultShape.IsEmpty())
		{
			const FIntPoint DefaultShapeMaxSpan = UDieg_UtilityLibrary::GetShapeSpan(ItemDefinition.DefaultShape);
			ItemSize = DefaultShapeMaxSpan.GetMax();
			Shape = ItemDefinition.DefaultShape;
		}
	}

	Property_Name = SelectedDataAssetName;
	Property_StaticMesh = SelectedDataAssetMesh;
	SpinBox_GridSize->SetValue(ItemSize);
	Property_MaxQuantity = MaxQuantity;
	SelectedSlots.Empty();
	SetupGrid(ItemSize);

	TMap<FIntPoint, TObjectPtr<UDieg_Slot>>& SlotMap = GridWidget->GetSlotMap();
	UDieg_UtilityLibrary::ForEachSlotInShape(SlotMap, Shape, [&](UDieg_Slot* GridSlot)
	{
		HandleSlotPressed(GridSlot);
	});
}

void UDieg_ItemCreatorEuw::AddDataAssetToMap(FString Name, TObjectPtr<UDieg_ItemDefinitionDataAsset> DataAsset)
{
	DataAssetMap.Add(FName(*Name), DataAsset);
	ComboBox_DataAsset->AddOption(Name);
}

void UDieg_ItemCreatorEuw::RefreshItemList()
{
	UPlugInv_DoubleLogger::Log("UDieg_ItemCreatorEuw::RefreshItemList()");
	if (!IsValid(DataAssetClass))
	{
		UPlugInv_DoubleLogger::LogError(20.0f, TEXT("Data Asset Class in Item Creator Editor Utility Widget is null. Cannot refresh item list."));
		return;
	}
	
	const FString SelectedOption = GetSelectedName();
	DataAssetMap.Empty();
	ComboBox_DataAsset->ClearOptions();

	// Default option to create items
	AddDataAssetToMap(EmptySelectionOption, nullptr);
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const UClass* Class = DataAssetClass.Get();
	TArray<FAssetData> AssetData;
	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.Add(Class->GetClassPathName());
	Filter.PackagePaths.Add(FName(*ItemsDataAssetsStoragePath.Path));
	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	
	for (const FAssetData& Data : AssetData)
	{
		TObjectPtr<UDieg_ItemDefinitionDataAsset> ItemDataAsset = Cast<UDieg_ItemDefinitionDataAsset>(Data.GetAsset());
		if (!IsValid(ItemDataAsset))
			continue;
		
		const FDieg_ItemDefinition& ItemDefinition = ItemDataAsset->ItemDefinition;
		AddDataAssetToMap(ItemDefinition.Name.ToString(), ItemDataAsset);
	}

	if (SelectedOption.IsEmpty())
	{
		ComboBox_DataAsset->SetSelectedOption(EmptySelectionOption);
	}
	else
	{
		ComboBox_DataAsset->SetSelectedOption(SelectedOption);
	}
}

void UDieg_ItemCreatorEuw::HandleSlotPressed_Implementation(UDieg_Slot* GridSlot)
{
	// Clicked slot
	const FIntPoint SlotCoordinates = GridSlot->GetCoordinatesInGrid();

	// Check if already selected
	if (SelectedSlots.Contains(SlotCoordinates))
	{
		// Remove and set it to hover (not none because mouse is still overlapping)
		SelectedSlots.Remove(SlotCoordinates);
		GridSlot->SetStatusAndColor(EDieg_SlotStatus::Hover);
	}
	else
	{
		// Normally it would be pressed, but for our purpose in this class is locked
		SelectedSlots.Add(SlotCoordinates);
		GridSlot->SetStatusAndColor(EDieg_SlotStatus::Locked);
	}
}

FString UDieg_ItemCreatorEuw::GetSelectedName() const
{
	return ComboBox_DataAsset->GetSelectedOption();
}

UDieg_ItemDefinitionDataAsset* UDieg_ItemCreatorEuw::GetSelectedDataAsset()
{
	const FName DataAssetName = FName(*GetSelectedName());
	TObjectPtr<UDieg_ItemDefinitionDataAsset> DataAsset = DataAssetMap.FindChecked(DataAssetName);
	return DataAsset;
}

int32 UDieg_ItemCreatorEuw::GetGridSize() const
{
	return FMath::TruncToInt(SpinBox_GridSize->GetValue());
}

bool UDieg_ItemCreatorEuw::IsValidAsset() const
{
	return ComboBox_DataAsset->GetSelectedIndex() != 0;
}

void UDieg_ItemCreatorEuw::GridSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                         UDieg_Slot* GridSlot)
{
	UPlugInv_DoubleLogger::Log(2.0f, TEXT("EUW Slot Hover: {0}"), FColor::Yellow, GridSlot->GetCoordinatesInGrid());
}

void UDieg_ItemCreatorEuw::GridSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot)
{
}

void UDieg_ItemCreatorEuw::GridSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDieg_Slot* GridSlot)
{
	UPlugInv_DoubleLogger::Log(2.0f, TEXT("EUW Slot Pressed: {0}"), FColor::Green, GridSlot->GetCoordinatesInGrid());
	HandleSlotPressed(GridSlot);
}

void UDieg_ItemCreatorEuw::GridSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDieg_Slot* GridSlot)
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UDieg_ItemCreatorEuw::HandleButtonModifyClicked()
{
	if (CanvasPanel_Modify->IsVisible())
	{
		CanvasPanel_Modify->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		CanvasPanel_Modify->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDieg_ItemCreatorEuw::HandleButtonConfirmClicked()
{
	if (TObjectPtr<UDieg_ItemDefinitionDataAsset> FoundDataAsset = DataAssetMap.FindChecked(FName(*GetSelectedName()));
		IsValid(FoundDataAsset) && ComboBox_DataAsset->GetSelectedIndex() != 0)
	{
		// Modify DA
		ModifyDataAsset();
	}
	else if (Property_Name.IsEmpty() == false)
	{
		// Create DA
		CreateDataAsset();
	}
	CanvasPanel_Modify->SetVisibility(ESlateVisibility::Collapsed);
}

void UDieg_ItemCreatorEuw::HandleComboBoxDaChange(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// Only selection changes that weren't set in code.
	if (!SelectedItem.IsEmpty() && SelectionType != ESelectInfo::Type::Direct)
	{
		SelectDataAsset(SelectedItem);
	}
}

void UDieg_ItemCreatorEuw::HandleComboBoxDaOpening()
{
	RefreshItemList();
}

void UDieg_ItemCreatorEuw::HandleSpinBoxGridSizeChange(float InValue)
{
	SetupGrid(floor(InValue));
}
