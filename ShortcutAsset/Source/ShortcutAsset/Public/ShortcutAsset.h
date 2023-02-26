#pragma once

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
};