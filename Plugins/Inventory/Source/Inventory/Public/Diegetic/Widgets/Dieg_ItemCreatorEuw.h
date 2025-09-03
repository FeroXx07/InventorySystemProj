// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dieg_Grid.h"
#include "Editor/ScriptableEditorWidgets/Public/Components/SinglePropertyView.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetComponents.h"
#include "Dieg_ItemCreatorEuw.generated.h"

class UDieg_ItemDefinitionDataAsset;
class USpinBox;
class UComboBox;
class UButton;
class UCanvasPanel;
class USizeBox;
/**
 * 
 */
UCLASS()
class INVENTORY_API UDieg_ItemCreatorEuw : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void SetupGrid(int32 GridSize) const;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<UPrimaryDataAsset> DataAssetClass;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	FDirectoryPath ItemsDataAssetsStoragePath;

	UPROPERTY(EditAnywhere, Category = "Properties")
	FString ItemsDataAssetsPrefix = "DA_Items_";

	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	FText Property_Name;
	
	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	TObjectPtr<UStaticMesh> Property_StaticMesh;

	// To bind to SinglePropertyView it caches its variable name. 
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	int32 Property_MaxQuantity;
	
	void ModifyDataAsset();
	void CreateDataAsset();
	void SelectDataAsset(const FString& Option);
	void AddDataAssetToMap(FString Name, TObjectPtr<UDieg_ItemDefinitionDataAsset> DataAsset);
	void RefreshItemList();

	UFUNCTION(BlueprintNativeEvent, Category = "Loading")
	void HandleSlotPressed(UDieg_Slot* GridSlot);
	
	FString GetSelectedName() const;
	UDieg_ItemDefinitionDataAsset* GetSelectedDataAsset();
	int32 GetGridSize() const;
	bool IsValidAsset() const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Modify;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditorUtilityButton> Button_Modify;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditorUtilityButton> Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditorUtilityComboBoxString> ComboBox_DataAsset;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> SpinBox_GridSize;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USinglePropertyView> SinglePropertyView_Name;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USinglePropertyView> SinglePropertyView_StaticMesh;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USinglePropertyView> SinglePropertyView_MaxQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDieg_Grid> GridWidget;

	UPROPERTY(EditAnywhere, Category = "Containers", meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UDieg_ItemDefinitionDataAsset>> DataAssetMap;

	UPROPERTY(EditAnywhere, Category = "Containers", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SelectedSlots;

	const FString EmptySelectionOption = "Create New Item";
	
	UFUNCTION()
	void GridSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void GridSlotUnHovered(const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void GridSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void GridSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDieg_Slot* GridSlot);

	UFUNCTION()
	void HandleButtonModifyClicked();

	UFUNCTION()
	void HandleButtonConfirmClicked();

	UFUNCTION()
	void HandleComboBoxDaChange(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void HandleComboBoxDaOpening();

	UFUNCTION()
	void HandleSpinBoxGridSizeChange(float InValue);
};
