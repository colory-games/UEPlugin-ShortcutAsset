#pragma once

#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "ShortcutAsset.generated.h"

UENUM(BlueprintType)
enum class EShortcutAssetLinkType : uint8
{
	Asset,
	SoftAsset,
	Directory,
	MAX,
};

UCLASS(BlueprintType)
class SHORTCUTASSET_API UShortcutAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EShortcutAssetLinkType LinkType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "LinkType == EShortcutAssetLinkType::Asset"))
	TObjectPtr<UObject> LinkedAsset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::SoftAsset"))
	FSoftObjectPath LinkedAssetPath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ContentDir, EditCondition = "LinkType == EShortcutAssetLinkType::Directory"))
	FDirectoryPath LinkedDirectoryPath;
};