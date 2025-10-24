// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Inv_ItemCreatorEditorUtility.generated.h"

class UEditorUtilityScrollBox;
class UInv_ItemDefinition;
class UVerticalBox;
class UInv_SinglePropertyViewWrapper;
class UInv_Grid;
class UInv_Slot;
class UInv_ItemDataAsset;
class USinglePropertyView;
class USpinBox;
class UEditorUtilityComboBoxString;
class UEditorUtilityButton;
class UCanvasPanel;
class USizeBox;
/**
 * 
 */
UCLASS()
class INVENTORYSYS_API UInv_ItemCreatorEditorUtility : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CanvasPanel_Modify;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityButton> Button_Modify;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityButton> Button_Confirm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityComboBoxString> ComboBox_DataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USpinBox> SpinBox_GridSize;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	// TObjectPtr<USinglePropertyView> SinglePropertyView_Name;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	// TObjectPtr<USinglePropertyView> SinglePropertyView_StaticMesh;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	// TObjectPtr<USinglePropertyView> SinglePropertyView_MaxQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInv_SinglePropertyViewWrapper>> SpawnedPropertyViews;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditorUtilityScrollBox> ScrollBox_SPVWrapper;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Editor|Inv", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UInv_Grid> GridWidget;

	UPROPERTY(EditAnywhere, Category = "Editor|Inv", meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<UInv_ItemDataAsset>> DataAssetMap;

	UPROPERTY(EditAnywhere, Category = "Editor|Inv", meta = (AllowPrivateAccess = "true"))
	TSet<FIntPoint> SelectedSlots;

	const FString EmptySelectionOption = "Create New Item";

	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void SetupGrid(int32 GridSize) const;

	UPROPERTY(EditAnywhere, Category = "Editor|Inv")
	TSubclassOf<UInv_SinglePropertyViewWrapper> SinglePropertyViewWrapperClass;
	
	UPROPERTY(EditAnywhere, Category = "Editor|Inv")
	TSubclassOf<UPrimaryDataAsset> DataAssetClass;
	
	UPROPERTY(EditAnywhere, Category = "Editor|Inv")
	FDirectoryPath ItemsDataAssetsStoragePath;

	UPROPERTY(EditAnywhere, Category = "Editor|Inv")
	FString ItemsDataAssetsPrefix = "DA_Items_";

	UPROPERTY(EditAnywhere, Category = "Editor|Inv")
	TObjectPtr<UInv_ItemDefinition> ItemDefinitionEditable;

	// // To bind to SinglePropertyView it caches its variable name. 
	// UPROPERTY(EditInstanceOnly, Category = "Editor|Inv")
	// FText Property_Name;
	//
	// // To bind to SinglePropertyView it caches its variable name. 
	// UPROPERTY(EditInstanceOnly, Category = "Editor|Inv")
	// TObjectPtr<UStaticMesh> Property_StaticMesh;
	//
	// // To bind to SinglePropertyView it caches its variable name. 
	// UPROPERTY(EditInstanceOnly, Category = "Editor|Inv")
	// int32 Property_MaxQuantity;

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void CreateDataAsset();
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void ModifyDataAsset();
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void SelectDataAsset(const FString& Option);
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void AddDataAssetToMap(FString Name, UInv_ItemDataAsset* DataAsset);
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void RefreshItemList();

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleSlotPressed(UInv_Slot* GridSlot);
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	FString GetSelectedName() const;
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	UInv_ItemDataAsset* GetSelectedDataAsset();
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	int32 GetGridSize() const;
	
	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	bool IsValidAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void GridSlotHover(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void GridSlotUnHovered(const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void GridSlotPressed(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void GridSlotReleased(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UInv_Slot* GridSlot);

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleButtonModifyClicked();

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleButtonConfirmClicked();

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleComboBoxDaChange(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleComboBoxDaOpening();

	UFUNCTION(BlueprintCallable, Category = "Editor|Inv")
	void HandleSpinBoxGridSizeChange(float InValue);
};
