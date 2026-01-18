/*!
 * ShortcutAsset
 *
 * Copyright (c) 2026 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAsset.h"

#if !UE_VERSION_OLDER_THAN(5, 4, 0)
#include "UObject/AssetRegistryTagsContext.h"
#endif

#define LOCTEXT_NAMESPACE "ShortcutAsset"

#if UE_VERSION_OLDER_THAN(5, 4, 0)

void UShortcutAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	Super::GetAssetRegistryTags(OutTags);

	OutTags.Add(FAssetRegistryTag(TEXT("LinkType"),
		LinkType == EShortcutAssetLinkType::Asset
		? TEXT("Asset")
		: LinkType == EShortcutAssetLinkType::DirectoryPath ? TEXT("Directory Path") : TEXT("Unknown"),
		FAssetRegistryTag::ETagType::TT_Alphabetical));

	FString AssetPath = LinkedAsset.GetLongPackageName();
	if (LinkType != EShortcutAssetLinkType::Asset)
	{
		AssetPath = FString();
	}
	OutTags.Add(FAssetRegistryTag(TEXT("LinkedAsset"), AssetPath, FAssetRegistryTag::ETagType::TT_Alphabetical));

	FString DirectoryPath = LinkedDirectoryPath.Path;
	if (LinkType != EShortcutAssetLinkType::DirectoryPath)
	{
		DirectoryPath = FString();
	}
	OutTags.Add(FAssetRegistryTag(TEXT("LinkedDirectoryPath"), DirectoryPath, FAssetRegistryTag::ETagType::TT_Alphabetical));
}

#else

void UShortcutAsset::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
	Super::GetAssetRegistryTags(Context);

	Context.AddTag(FAssetRegistryTag(TEXT("LinkType"),
		LinkType == EShortcutAssetLinkType::Asset
		? TEXT("Asset")
		: LinkType == EShortcutAssetLinkType::DirectoryPath ? TEXT("Directory Path") : TEXT("Unknown"),
		FAssetRegistryTag::ETagType::TT_Alphabetical));

	FString AssetPath = LinkedAsset.GetLongPackageName();
	if (LinkType != EShortcutAssetLinkType::Asset)
	{
		AssetPath = FString();
	}
	Context.AddTag(FAssetRegistryTag(TEXT("LinkedAsset"), AssetPath, FAssetRegistryTag::ETagType::TT_Alphabetical));

	FString DirectoryPath = LinkedDirectoryPath.Path;
	if (LinkType != EShortcutAssetLinkType::DirectoryPath)
	{
		DirectoryPath = FString();
	}
	Context.AddTag(FAssetRegistryTag(TEXT("LinkedDirectoryPath"), DirectoryPath, FAssetRegistryTag::ETagType::TT_Alphabetical));
}

#endif

#undef LOCTEXT_NAMESPACE
