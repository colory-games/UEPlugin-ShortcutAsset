#include "ShortcutAssetModule.h"

#include "ShortcutAssetActions.h"
#include "Styling/SlateStyleRegistry.h"

void FShortcutAssetModule::StartupModule()
{
	RegisterActions();
	RegisterStyles();
}

void FShortcutAssetModule::ShutdownModule()
{
	UnregisterStyles();
	UnregisterActions();
}

bool FShortcutAssetModule::SupportsDynamicReloading()
{
	return true;
}

void FShortcutAssetModule::RegisterActions()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedRef<IAssetTypeActions> ShortcutAssetActions = MakeShareable(new FShortcutAssetActions());

	AssetTools.RegisterAssetTypeActions(ShortcutAssetActions);
	RegisteredAssetTypeActions.Add(ShortcutAssetActions);
}

void FShortcutAssetModule::UnregisterActions()
{
	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetToools");
	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}
}

void FShortcutAssetModule::RegisterStyles()
{
	FString ProjectPluginDirPath =
		FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir() + "ShortcutAsset"));
	FString EnginePluginDirPath =
		FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::EnginePluginsDir() + "Marketplace/ShortcutAsset"));
	FString ShortcutAssetDirPath;

	if (FPaths::DirectoryExists(ProjectPluginDirPath))
	{
		ShortcutAssetDirPath = ProjectPluginDirPath;
	}
	else if (FPaths::DirectoryExists(EnginePluginDirPath))
	{
		ShortcutAssetDirPath = EnginePluginDirPath;
	}
	else
	{
		return;
	}

	TSharedRef<FSlateStyleSet> NewStyle = MakeShareable(new FSlateStyleSet("ShortcutAssetStyle"));
	NewStyle->SetContentRoot(ShortcutAssetDirPath);
	NewStyle->Set("ClassThumbnail.ShortcutAsset",
		new FSlateImageBrush(NewStyle->RootToContentDir(TEXT("Resources/AssetIcons/ShortcutAssetThumbnail_64"), TEXT(".png")),
			FVector2D(64.0f, 64.0f)));
	NewStyle->Set("ClassIcon.ShortcutAsset",
		new FSlateImageBrush(
			NewStyle->RootToContentDir(TEXT("Resources/AssetIcons/ShortcutAssetIcon_16"), TEXT(".png")), FVector2D(16.0f, 16.0f)));

	FSlateStyleRegistry::RegisterSlateStyle(*NewStyle);
	RegisteredSlateStyleSets.Add(NewStyle);
}

void FShortcutAssetModule::UnregisterStyles()
{
	for (auto Style : RegisteredSlateStyleSets)
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(Style->GetStyleSetName());
	}
}

IMPLEMENT_MODULE(FShortcutAssetModule, ShortcutAssetModule);