/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "Misc/EngineVersionComparison.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "ShortcutAsset.generated.h"

UENUM(BlueprintType)
enum class EShortcutAssetLinkType : uint8
{
	// The asset object.
	Asset,

	// The directory path.
	DirectoryPath,

	MAX,
};

UCLASS(BlueprintType, DisplayName = "Shortcut")
class SHORTCUTASSET_API UShortcutAsset : public UObject
{
	GENERATED_BODY()

public:
	// Link type.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property")
	EShortcutAssetLinkType LinkType;

	// The asset path linked by this asset. Valid if LinkType is Asset Path.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property",
		meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::Asset"))
	FSoftObjectPath LinkedAsset;

	// The directory path linked by this asset. Valid if LinkType is Directory.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property",
		meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::DirectoryPath"))
	FDirectoryPath LinkedDirectoryPath;

#if UE_VERSION_NEWER_THAN_OR_EQUAL(5, 4, 0)
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;
#else
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
#endif
};