#pragma once

#include "SShortcutAssetEditor.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

SShortcutAssetEditor::~SShortcutAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

void SShortcutAssetEditor::Construct(const FArguments& InArgs, UShortcutAsset* InShortcutAsset)
{
	ShortcutAsset = InShortcutAsset;

	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FOnGetDetailCustomizationInstance OnGetPropertiesViewCustomizationInstance =
		FOnGetDetailCustomizationInstance::CreateStatic(&FShortcutAssetPropertiesDetailCustomization::MakeInstance);
	FDetailsViewArgs OnGetPropertiesViewCustomizationInstanceArgs(
		false, false, false, FDetailsViewArgs::HideNameArea, true, nullptr, false, FName("UShortcutAsset"));
	PropertiesView = EditModule.CreateDetailView(OnGetPropertiesViewCustomizationInstanceArgs);
	PropertiesView->RegisterInstancedCustomPropertyLayout(UObject::StaticClass(), OnGetPropertiesViewCustomizationInstance);
	PropertiesView->SetObject(ShortcutAsset);

	// clang-format off
	ChildSlot
	[
		SNew(SSplitter)
		.Orientation(EOrientation::Orient_Vertical)
		+ SSplitter::Slot()
		[
			SNew(SBorder)
			[
				PropertiesView.ToSharedRef()
			]
		]
	];
	// clang-format on
}

FShortcutAssetPropertiesLayout::FShortcutAssetPropertiesLayout(TWeakObjectPtr<UShortcutAsset> InShortcutAsset)
	: ShortcutAsset(InShortcutAsset)
{
}

void FShortcutAssetPropertiesLayout::GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder)
{
	if (!ShortcutAsset.IsValid())
	{
		return;
	}
}

FName FShortcutAssetPropertiesLayout::GetName() const
{
	if (ShortcutAsset.IsValid())
	{
		return ShortcutAsset->GetFName();
	}

	return NAME_None;
}

void FShortcutAssetPropertiesDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	IDetailCategoryBuilder& ShortcutAssetCategory =
		DetailLayout.EditCategory("ShortcutAsset", FText::FromString("Property"), ECategoryPriority::Important);
	const TArray<TWeakObjectPtr<UObject>> Objects = DetailLayout.GetDetailsView()->GetSelectedObjects();
	check(Objects.Num() == 1);

	if (Objects.Num() == 1)
	{
		ShortcutAsset = CastChecked<UShortcutAsset>(Objects[0].Get());
		PropertyLayout = MakeShareable(new FShortcutAssetPropertiesLayout(ShortcutAsset));
		ShortcutAssetCategory.AddCustomBuilder(PropertyLayout.ToSharedRef());
	}
}
