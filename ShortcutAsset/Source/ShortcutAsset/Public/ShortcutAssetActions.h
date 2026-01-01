/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "AssetTypeActions_Base.h"

class FShortcutAssetEditLinkActions : public FAssetTypeActions_Base
{
public:
	FShortcutAssetEditLinkActions();

	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;

	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	virtual uint32 GetCategories() override;

	virtual FText GetName() const override;

	virtual UClass* GetSupportedClass() const override;

	virtual FColor GetTypeColor() const override;
};

FUIAction MakeCreateDirectoryPathLinkAction(const FString& Path, FString PathToCreate = "");
FUIAction MakeCreateAssetLinkAction(const FAssetData& AssetData, FString PathToCreate = "");
