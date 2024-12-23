/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "IDetailsView.h"
#include "IPropertyTypeCustomization.h"
#include "ShortcutAsset.h"
#include "Widgets/SCompoundWidget.h"

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

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& Builder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};
