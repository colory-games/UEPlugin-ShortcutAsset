/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"


class FShortcutAssetModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
	TArray<TSharedRef<FSlateStyleSet>> RegisteredSlateStyleSets;
	TArray<FDelegateHandle> RegisteredContentBrowserAssetExtenderHandles;
	TArray<FDelegateHandle> RegisteredContentBrowserDirectoryPathExtenderHandles;

	void RegisterActions();
	void UnregisterActions();

	void RegisterStyles();
	void UnregisterStyles();

	void RegisterMenus();
	void UnregisterMenus();
};
