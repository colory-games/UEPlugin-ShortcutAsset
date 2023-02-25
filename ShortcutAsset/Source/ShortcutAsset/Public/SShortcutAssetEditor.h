#pragma once

#include "IPropertyTypeCustomization.h"
#include "Widgets/SCompoundWidget.h"
#include "ShortcutAsset.h"


class SShortcutAssetEditor : public SCompoundWidget
{
	UShortcutAsset* ShortcutAsset;

	TSharedPtr<IDetailsView> PropertiesView;

public:
	SLATE_BEGIN_ARGS(SShortcutAssetEditor)
	{
	}
	SLATE_END_ARGS()

	virtual ~SShortcutAssetEditor();

	void Construct(const FArguments& InArgs, UShortcutAsset* InShortcutAsset);
};

class FShortcutAssetPropertyTypeCustomization : public IPropertyTypeCustomization
{

public:
	FShortcutAssetPropertyTypeCustomization();

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FShortcutAssetPropertyTypeCustomization);
	}

	bool IsSoftObjectPathProperty(const FProperty* Property) const;
	bool IsObjectProperty(const FProperty* Property) const;

	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(
		TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& Builder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};
