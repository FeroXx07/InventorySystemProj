// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inv_ItemCreatorEditorUtility.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "EditorUtilityWidgetComponents.h"
#include "IContentBrowserSingleton.h"
#include "Inv_UtilityLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/CanvasPanel.h"
#include "Components/SinglePropertyView.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Factories/DataAssetFactory.h"
#include "UObject/SavePackage.h"
#include "UObjects/Inv_ItemDataAsset.h"
#include "Widgets/Inv_Grid.h"
#include "Widgets/Inv_Slot.h"
#include "Widgets/Inv_SinglePropertyViewWrapper.h"

void UInv_ItemCreatorEditorUtility::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SpawnedPropertyViews.Empty();
	ScrollBox_SPVWrapper->ClearChildren();
	
	if (!IsValid(ItemDefinitionEditable))
	{
		ItemDefinitionEditable = NewObject<UInv_ItemDefinition>(this);
	}
}

void UInv_ItemCreatorEditorUtility::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!IsValid(SinglePropertyViewWrapperClass))
	{
		return;
	}
	
	if (!IsValid(ItemDefinitionEditable))
    {
    	ItemDefinitionEditable = NewObject<UInv_ItemDefinition>(this);
    }

	SpawnedPropertyViews.Empty();
	ScrollBox_SPVWrapper->ClearChildren();
	for (TFieldIterator<FProperty> It(UInv_ItemDefinition::StaticClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		const FProperty* Property = *It;
		FName PropertyName = Property->GetFName();

		// USinglePropertyView* PropView = WidgetTree->ConstructWidget<USinglePropertyView>(USinglePropertyView::StaticClass());

		// Skip non-editable or hidden fields
		if (!Property->HasAnyPropertyFlags(CPF_Edit | CPF_BlueprintVisible))
			continue;

		// Skip non-instance properties if desired
		if (Property->HasAnyPropertyFlags(CPF_Transient | CPF_DisableEditOnInstance))
			continue;

		if (UInv_SinglePropertyViewWrapper* ViewWrapper = CreateWidget<UInv_SinglePropertyViewWrapper>(this, SinglePropertyViewWrapperClass))
		{
			SpawnedPropertyViews.Add(ViewWrapper);
			ViewWrapper->InitializeProperties(ItemDefinitionEditable, PropertyName);

			if (UPanelSlot* PanelSlot = ScrollBox_SPVWrapper->AddChild(ViewWrapper))
			{
				// VSlot->set(FMargin(0.0f, 5.0f));
			}
		}
	}

	// CanvasPanel_Modify->SetVisibility(ESlateVisibility::Collapsed);
	SetupGrid(GetGridSize());
}

void UInv_ItemCreatorEditorUtility::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IsDesignTime())
		return;

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

void UInv_ItemCreatorEditorUtility::NativeDestruct()
{
	ScrollBox_SPVWrapper->ClearChildren();
	SpawnedPropertyViews.Empty();
	ItemDefinitionEditable = nullptr;

	Super::NativeDestruct();
}

void UInv_ItemCreatorEditorUtility::SetupGrid(int32 GridSize) const
{
	GridWidget->CreateGrid(GridSize * GridSize, GridSize);
}

void UInv_ItemCreatorEditorUtility::CreateDataAsset()
{
	// const FString AssetName = Property_Name.ToString().Replace(TEXT(" "), TEXT("_"));
	const FString AssetName = ItemDefinitionEditable->Name.ToString().Replace(TEXT(" "), TEXT("_"));
	
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

	if (UInv_ItemDataAsset* ItemDataAsset = Cast<UInv_ItemDataAsset>(NewObject))
	{
		UInv_ItemDefinition* ItemDefinition = ItemDataAsset->ItemDefinition;

		// Set properties
		// ItemDefinition->Name = Property_Name;
		// ItemDefinition->WorldMesh = Property_StaticMesh;
		// ItemDefinition->DefaultShape = SelectedSlots.Array();
		// ItemDefinition->StackSizeMax = Property_MaxQuantity;
		ItemDefinition = ItemDefinitionEditable;
		
		// // Update the shape root after setting the shape
		// UInv_ItemDataAsset::SetItemDefinitionShapeRoot(ItemDefinition);
		
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

		SelectDataAsset(ItemDefinitionEditable->Name.ToString());
	}
}

void UInv_ItemCreatorEditorUtility::ModifyDataAsset()
{
	UInv_ItemDataAsset* SelectedItemDataAsset = GetSelectedDataAsset();
	UInv_ItemDefinition* ItemDefinition = SelectedItemDataAsset->ItemDefinition;
	// Set properties
	// ItemDefinition->Name = Property_Name;
	// ItemDefinition->WorldMesh = Property_StaticMesh;
	// ItemDefinition->DefaultShape = SelectedSlots.Array();
	// ItemDefinition->StackSizeMax = Property_MaxQuantity;
	ItemDefinition = ItemDefinitionEditable;
	
	// // Update the shape root after setting the shape
	// UInv_ItemDataAsset::SetItemDefinitionShapeRoot(ItemDefinition);
		
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

void UInv_ItemCreatorEditorUtility::SelectDataAsset(const FString& Option)
{
	FText SelectedDataAssetName = FText::GetEmpty();
	UStaticMesh* SelectedDataAssetMesh = nullptr;
	float ItemSize = 3.0f;
	int32 MaxQuantity = 1;
	TArray<FIntPoint> Shape;
	
	if (Option != EmptySelectionOption)
	{
		ComboBox_DataAsset->SetSelectedOption(Option);
		UInv_ItemDataAsset* SelectedItemDataAsset = GetSelectedDataAsset();
		UInv_ItemDefinition* ItemDefinition = SelectedItemDataAsset->ItemDefinition;

		SelectedDataAssetName = ItemDefinition->Name;

		if (ItemDefinition->WorldMesh.IsValid())
		{
			SelectedDataAssetMesh = ItemDefinition->WorldMesh.Get();
		}
		else
		{
			SelectedDataAssetMesh = ItemDefinition->WorldMesh.LoadSynchronous();
		}
		
		MaxQuantity = ItemDefinition->StackSizeMax;
		
		if (!ItemDefinition->DefaultShape.IsEmpty())
		{
			const FIntPoint DefaultShapeMaxSpan = UInv_UtilityLibrary::GetShapeSpan(ItemDefinition->DefaultShape);
			ItemSize = DefaultShapeMaxSpan.GetMax();
			Shape = ItemDefinition->DefaultShape;
		}
		
		ItemDefinitionEditable = ItemDefinition;
	}

	// Property_Name = SelectedDataAssetName;
	// Property_StaticMesh = SelectedDataAssetMesh;
	// Property_MaxQuantity = MaxQuantity;
	
	SpinBox_GridSize->SetValue(ItemSize);
	SelectedSlots.Empty();
	SetupGrid(ItemSize);

	UInv_UtilityLibrary::ForEachSlotInShape(GridWidget->SlotMap, Shape, [&](UInv_Slot* GridSlot)
	{
		HandleSlotPressed(GridSlot);
	});
}

void UInv_ItemCreatorEditorUtility::AddDataAssetToMap(FString Name, UInv_ItemDataAsset* DataAsset)
{
	DataAssetMap.Add(FName(*Name), DataAsset);
	ComboBox_DataAsset->AddOption(Name);
}

void UInv_ItemCreatorEditorUtility::RefreshItemList()
{
	// UPlugInv_DoubleLogger::Log("UDieg_ItemCreatorEuw::RefreshItemList()");
	if (!IsValid(DataAssetClass))
	{
		// UPlugInv_DoubleLogger::LogError(20.0f, TEXT("Data Asset Class in Item Creator Editor Utility Widget is null. Cannot refresh item list."));
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
		TObjectPtr<UInv_ItemDataAsset> ItemDataAsset = Cast<UInv_ItemDataAsset>(Data.GetAsset());
		if (!IsValid(ItemDataAsset))
			continue;
		
		const UInv_ItemDefinition* ItemDefinition = ItemDataAsset->ItemDefinition;
		AddDataAssetToMap(ItemDefinition->Name.ToString(), ItemDataAsset);
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

void UInv_ItemCreatorEditorUtility::HandleSlotPressed(UInv_Slot* GridSlot)
{
	// Clicked slot
	const FIntPoint SlotCoordinates = GridSlot->GetGridCoordinates();

	// Check if already selected
	if (SelectedSlots.Contains(SlotCoordinates))
	{
		// Remove and set it to hover (not none because mouse is still overlapping)
		SelectedSlots.Remove(SlotCoordinates);
		GridSlot->SetState(EInv_SlotState::Hover);
	}
	else
	{
		// Normally it would be pressed, but for our purpose in this class is locked
		SelectedSlots.Add(SlotCoordinates);
		GridSlot->SetState(EInv_SlotState::Locked);
	}
}

FString UInv_ItemCreatorEditorUtility::GetSelectedName() const
{
	return ComboBox_DataAsset->GetSelectedOption();
}

UInv_ItemDataAsset* UInv_ItemCreatorEditorUtility::GetSelectedDataAsset()
{
	const FName DataAssetName = FName(*GetSelectedName());
	TObjectPtr<UInv_ItemDataAsset> DataAsset = DataAssetMap.FindChecked(DataAssetName);
	return DataAsset;
}

int32 UInv_ItemCreatorEditorUtility::GetGridSize() const
{
	return FMath::TruncToInt(SpinBox_GridSize->GetValue());
}

bool UInv_ItemCreatorEditorUtility::IsValidAsset() const
{
	return ComboBox_DataAsset->GetSelectedIndex() != 0;
}

void UInv_ItemCreatorEditorUtility::GridSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UInv_Slot* GridSlot)
{
}

void UInv_ItemCreatorEditorUtility::GridSlotUnHovered(const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot)
{
}

void UInv_ItemCreatorEditorUtility::GridSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UInv_Slot* GridSlot)
{
	HandleSlotPressed(GridSlot);
}

void UInv_ItemCreatorEditorUtility::GridSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UInv_Slot* GridSlot)
{
}

void UInv_ItemCreatorEditorUtility::HandleButtonModifyClicked()
{
	ESlateVisibility NewState = CanvasPanel_Modify->IsVisible() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
	CanvasPanel_Modify->SetVisibility(NewState);
}

void UInv_ItemCreatorEditorUtility::HandleButtonConfirmClicked()
{
	if (TObjectPtr<UInv_ItemDataAsset> FoundDataAsset = DataAssetMap.FindChecked(FName(*GetSelectedName()));
		IsValid(FoundDataAsset) && ComboBox_DataAsset->GetSelectedIndex() != 0)
	{
		// Modify DA
		ModifyDataAsset();
	}
	else if (ItemDefinitionEditable->Name.IsEmpty() == false)
	{
		// Create DA
		CreateDataAsset();
	}
	CanvasPanel_Modify->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemCreatorEditorUtility::HandleComboBoxDaChange(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// Only selection changes that weren't set in code.
	if (!SelectedItem.IsEmpty() && SelectionType != ESelectInfo::Type::Direct)
	{
		SelectDataAsset(SelectedItem);
	}
}

void UInv_ItemCreatorEditorUtility::HandleComboBoxDaOpening()
{
	RefreshItemList();
}

void UInv_ItemCreatorEditorUtility::HandleSpinBoxGridSizeChange(float InValue)
{
	SetupGrid(floor(InValue));
}

