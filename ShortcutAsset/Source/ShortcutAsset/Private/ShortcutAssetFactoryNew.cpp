#include "ShortcutAssetFactoryNew.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ShortcutAsset.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

UShortcutAssetFactoryNew::UShortcutAssetFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UShortcutAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UShortcutAssetFactoryNew::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
#ifdef SA_FREE_VERSION
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	FARFilter Filter;
	TArray<FAssetData> AssetData;
	Filter.ClassNames.Add(UShortcutAsset::StaticClass()->GetFName());

	AssetRegistryModule.Get().GetAssets(Filter, AssetData);
	if (AssetData.Num() >= 10)
	{
		FText TitleText = LOCTEXT("Title", "Reached Free Version Limitation");
		FText MessageText = LOCTEXT("Message", "Free version can only create up to 10 assets.\nDo you want to open the marketplace of this plugin?");
		if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, &TitleText) == EAppReturnType::Ok)
		{
			FPlatformProcess::LaunchURL(TEXT("https://forums.unrealengine.com/t/how-to-open-a-browser-from-the-game/24346/3"), NULL, NULL);
		}
		return nullptr;
	}
#endif // SA_FREE_VERSION

	return NewObject<UShortcutAsset>(InParent, InClass, InName, Flags);
}

bool UShortcutAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}

FString UShortcutAssetFactoryNew::GetDefaultNewAssetName() const
{
	return "NewShortcut";
}

#undef LOCTEXT_NAMESPACE