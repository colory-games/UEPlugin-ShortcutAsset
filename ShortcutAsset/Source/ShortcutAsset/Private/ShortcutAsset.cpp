/*!
 * ShortcutAsset
 *
 * Copyright (c) 2026 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAsset.h"

#include "UObject/AssetRegistryTagsContext.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

void UShortcutAsset::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{
    Super::GetAssetRegistryTags(Context);

    Context.AddTag(FAssetRegistryTag(
        TEXT("LinkType"),
        LinkType == EShortcutAssetLinkType::Asset ? TEXT("Asset") :
            LinkType == EShortcutAssetLinkType::DirectoryPath ? TEXT("Directory Path") : TEXT("Unknown"),
        FAssetRegistryTag::ETagType::TT_Alphabetical
    ));

    FString AssetPath = LinkedAsset.GetLongPackageName();
    if (LinkType != EShortcutAssetLinkType::Asset)
    {
        AssetPath = FString();
    }
    Context.AddTag(FAssetRegistryTag(
        TEXT("LinkedAsset"),
        AssetPath,
        FAssetRegistryTag::ETagType::TT_Alphabetical
    ));

    FString DirectoryPath = LinkedDirectoryPath.Path;
    if (LinkType != EShortcutAssetLinkType::DirectoryPath)
    {
        DirectoryPath = FString();
    }
    Context.AddTag(FAssetRegistryTag(
        TEXT("LinkedDirectoryPath"),
        DirectoryPath,
        FAssetRegistryTag::ETagType::TT_Alphabetical
    ));
}

#undef LOCTEXT_NAMESPACE
