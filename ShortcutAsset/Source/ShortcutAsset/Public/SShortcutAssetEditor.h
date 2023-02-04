#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailCustomNodeBuilder.h"
#include "IDetailCustomization.h"
#include "ShortcutAsset.h"
#include "Widgets/SCompoundWidget.h"

class SShortcutAssetEditor : public SCompoundWidget
{
	TObjectPtr<UShortcutAsset> ShortcutAsset;

	TSharedPtr<IDetailsView> PropertiesView;

public:
	SLATE_BEGIN_ARGS(SShortcutAssetEditor)
	{
	}
	SLATE_END_ARGS()

	virtual ~SShortcutAssetEditor();

	void Construct(const FArguments& InArgs, TObjectPtr<UShortcutAsset> InShortcutAsset);
};

class FShortcutAssetPropertiesLayout : public IDetailCustomNodeBuilder
{
	TWeakObjectPtr<UShortcutAsset> ShortcutAsset;

public:
	FShortcutAssetPropertiesLayout(TWeakObjectPtr<UShortcutAsset> InShortcutAsset);

	virtual void GenerateChildContent(IDetailChildrenBuilder& ChildrenBuilder) override;
	virtual void GenerateHeaderRowContent(FDetailWidgetRow& NodeRow) override
	{
	}
	virtual void Tick(float DeltaTime) override
	{
	}
	virtual bool RequiresTick() const override
	{
		return false;
	}
	virtual FName GetName() const override;
	virtual bool InitiallyCollapsed() const override
	{
		return false;
	}
};

class FShortcutAssetPropertiesDetailCustomization : public IDetailCustomization
{
	TWeakObjectPtr<UShortcutAsset> ShortcutAsset;
	TSharedPtr<FShortcutAssetPropertiesLayout> PropertyLayout;

public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FShortcutAssetPropertiesDetailCustomization);
	}

	FShortcutAssetPropertiesDetailCustomization()
	{
	}

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
};