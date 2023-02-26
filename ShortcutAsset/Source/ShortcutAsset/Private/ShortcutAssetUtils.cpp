#include "ShortcutAssetUtils.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/MessageDialog.h"

bool CheckLimitation(bool bIsCreateNew)
{
	int32 MaxAssets = 3;
	if (!bIsCreateNew)
	{
		MaxAssets++;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	FARFilter Filter;
	TArray<FAssetData> AssetData;
	Filter.ClassNames.Add(UShortcutAsset::StaticClass()->GetFName());

	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	if (AssetData.Num() >= MaxAssets)
	{
		FText TitleText = LOCTEXT("Title", "Reached Free Version Limitation");
		FText MessageText =
			LOCTEXT("Message", "Free version can only create up to 3 assets.\nDo you want to open the marketplace of this plugin?");
		if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, &TitleText) == EAppReturnType::Ok)
		{
			FPlatformProcess::LaunchURL(
				TEXT("https://forums.unrealengine.com/t/how-to-open-a-browser-from-the-game/24346/3"), NULL, NULL);
		}
		return false;
	}

	return true;
}