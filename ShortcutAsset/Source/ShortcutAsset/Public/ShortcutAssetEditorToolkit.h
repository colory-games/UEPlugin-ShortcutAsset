/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "ShortcutAsset.h"
#include "Tools/BaseAssetToolkit.h"

class FShortcutAssetEditorToolkit : public FBaseAssetToolkit
{
	UShortcutAsset* ShortcutAsset;

	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);

public:
	FShortcutAssetEditorToolkit(UAssetEditor* InOwningAssetEditor);

	virtual ~FShortcutAssetEditorToolkit();

	virtual bool IsPrimaryEditor() const override
	{
		return false;
	}

	virtual FName GetToolkitFName() const override;

	virtual FText GetBaseToolkitName() const override;

	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual FString GetWorldCentricTabPrefix() const override;

	virtual FString GetDocumentationLink() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
};