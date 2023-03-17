#pragma once

#include "SShortcutAssetEditor.h"

#include "PropertyCustomizationHelpers.h"

SShortcutAssetEditor::~SShortcutAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

void SShortcutAssetEditor::Construct(const FArguments& InArgs, UShortcutAsset* InShortcutAsset)
{
	ShortcutAsset = InShortcutAsset;

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs ViewArgs;
	ViewArgs.bAllowSearch = false;
	ViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	ViewArgs.bHideSelectionTip = true;
	PropertiesView = PropertyEditorModule.CreateDetailView(ViewArgs);
	PropertiesView->RegisterInstancedCustomPropertyTypeLayout(FName("SoftObjectPath"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShortcutAssetPropertyTypeCustomization::MakeInstance));
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

FShortcutAssetPropertyTypeCustomization::FShortcutAssetPropertyTypeCustomization()
{
}

bool FShortcutAssetPropertyTypeCustomization::IsSoftObjectPathProperty(const FProperty* Property) const
{
	const FStructProperty* StructProperty = CastField<const FStructProperty>(Property);

	return StructProperty && StructProperty->Struct == TBaseStructure<FSoftObjectPath>::Get();
}

bool FShortcutAssetPropertyTypeCustomization::IsObjectProperty(const FProperty* Property) const
{
	const FObjectProperty* ObjectProperty = CastField<const FObjectProperty>(Property);

	return ObjectProperty && ObjectProperty->PropertyClass == UObject::StaticClass();
}

void FShortcutAssetPropertyTypeCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FProperty* Property = PropertyHandle->GetProperty();

	FAssetData Data;
	PropertyHandle->GetValue(Data);

	FOnShouldFilterAsset OnShouldFilterAssetFunc;
	if (IsSoftObjectPathProperty(Property))
	{
		OnShouldFilterAssetFunc = FOnShouldFilterAsset::CreateLambda(
			[](const FAssetData& Data) -> bool { return Data.GetClass() == UShortcutAsset::StaticClass(); });
	}
	else
	{
		check(0);
	}

	// clang-format off
	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SObjectPropertyEntryBox)
			.DisplayCompactSize(true)
			.DisplayThumbnail(true)
			.AllowClear(true)
			.ThumbnailPool(CustomizationUtils.GetThumbnailPool())
			.OnShouldFilterAsset(OnShouldFilterAssetFunc)
			.PropertyHandle(PropertyHandle)
		];
	// clang-format on
}

void FShortcutAssetPropertyTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& Builder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}
