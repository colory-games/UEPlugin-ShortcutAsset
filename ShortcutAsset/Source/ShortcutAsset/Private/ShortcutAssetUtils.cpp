/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetUtils.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/MessageDialog.h"
#include "ShortcutAsset.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

bool ReachFreeVersionLimitation(bool bIsCreateNew)
{
	int32 MaxAssets = 3;
	if (!bIsCreateNew)
	{
		MaxAssets++;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	FARFilter Filter;
	TArray<FAssetData> AssetData;
#if UE_VERSION_NEWER_THAN(5, 1, 0)
	Filter.ClassPaths.Add(UShortcutAsset::StaticClass()->GetClassPathName());
#else
	Filter.ClassNames.Add(UShortcutAsset::StaticClass()->GetFName());
#endif

	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	if (AssetData.Num() >= MaxAssets)
	{
		FText TitleText = LOCTEXT("Title", "Reached Free Version Limitation");
		FText MessageText =
			LOCTEXT("Message", "Free version can only create up to 3 assets.\nDo you want to open the Fab page of this plugin?");
#if UE_VERSION_OLDER_THAN(5, 4, 0)
		if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, &TitleText) == EAppReturnType::Ok)
#else
		if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, TitleText) == EAppReturnType::Ok)
#endif
		{
			FPlatformProcess::LaunchURL(TEXT("https://www.fab.com/listings/32616e78-e6ef-4645-903b-22af25e2a1dd"), NULL, NULL);
		}
		return true;
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
