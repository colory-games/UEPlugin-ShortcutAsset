#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "ShortcutAsset.generated.h"

UENUM(BlueprintType)
enum class EShortcutAssetLinkType : uint8
{
	// The asset object (hard link).
	Asset,

	// The asset path (soft link).
	AssetPath,

	// The directory path.
	DirectoryPath,

	MAX,
};

UCLASS(BlueprintType)
class SHORTCUTASSET_API UShortcutAsset : public UObject
{
	GENERATED_BODY()

public:
	// Link type.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property")
	EShortcutAssetLinkType LinkType;

	// The asset object linked by this asset. Valid if LinkType is Asset.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property",
		meta = (EditCondition = "LinkType == EShortcutAssetLinkType::Asset"))
	UObject* LinkedAsset;

	// The asset path linked by this asset. Valid if LinkType is Asset Path.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property",
		meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::AssetPath"))
	FSoftObjectPath LinkedAssetPath;

	// The directory path linked by this asset. Valid if LinkType is Directory.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Link Property",
		meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::DirectoryPath"))
	FDirectoryPath LinkedDirectoryPath;
};