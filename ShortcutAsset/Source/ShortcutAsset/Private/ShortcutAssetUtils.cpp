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
#include "ContentBrowserDataSubsystem.h"
#include "Internationalization/Regex.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/MessageDialog.h"
#include "ShortcutAsset.h"
#if UE_VERSION_NEWER_THAN(5, 0, 0)
#include "ContentBrowserItemPath.h"
#endif

#include "Editor.h"

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

bool GetObjectPathFromClipboard(const FString& Clipboard, FString& ObjectPath)
{
	TArray<FString> Lines;
	Clipboard.ParseIntoArrayLines(Lines, true);
	if (Lines.Num() != 1)
	{
		return false;
	}

	FString Line = Lines[0];
	FRegexPattern Pattern(TEXT("/Script/Engine.Blueprint'(.+)'"));
	FRegexMatcher Matcher(Pattern, Line);
	FString Path;
	if (Matcher.FindNext())
	{
		Path = Matcher.GetCaptureGroup(1);
	}
	else
	{
		return false;
	}

	FString PackageName = FPackageName::ObjectPathToPackageName(Path);
	if (!FPackageName::IsValidLongPackageName(PackageName))
	{
		return false;
	}

	ObjectPath = Path;
	return true;
}

bool GetDirectoryPathFromClipboard(const FString& Clipboard, FString& DirectoryPath)
{
	TArray<FString> Lines;
	Clipboard.ParseIntoArrayLines(Lines, true);
	if (Lines.Num() != 1)
	{
		return false;
	}

	FString Line = Lines[0];
	FRegexPattern Pattern(TEXT("/Game/(.+)"));
	FRegexMatcher Matcher(Pattern, Line);
	FString Path;
	if (Matcher.FindNext())
	{
		Path = Matcher.GetCaptureGroup(0);
	}
	else
	{
		return false;
	}

	if (!FPackageName::IsValidLongPackageName(Path))
	{
		return false;
	}

	UContentBrowserDataSubsystem* ContentBrowserDataSubsystem = GEditor->GetEditorSubsystem<UContentBrowserDataSubsystem>();
	if (!ContentBrowserDataSubsystem)
	{
		return false;
	}

	FContentBrowserItem Item =
		ContentBrowserDataSubsystem->GetItemAtPath(FName(*Path), EContentBrowserItemTypeFilter::IncludeFolders);
	if (Item.IsValid())
	{
		return false;
	}

	DirectoryPath = Path;
	return true;
}

#undef LOCTEXT_NAMESPACE
