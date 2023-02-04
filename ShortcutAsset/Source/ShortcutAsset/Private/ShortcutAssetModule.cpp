#include "ShortcutAssetModule.h"

#include "ShortcutAssetActions.h"

void FShortcutAssetModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TSharedRef<IAssetTypeActions> ShortcutAssetActions = MakeShareable(new FShortcutAssetActions());

	AssetTools.RegisterAssetTypeActions(ShortcutAssetActions);
	RegisteredAssetTypeActions.Add(ShortcutAssetActions);
}

void FShortcutAssetModule::ShutdownModule()
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

bool FShortcutAssetModule::SupportsDynamicReloading()
{
	return true;
}

IMPLEMENT_MODULE(FShortcutAssetModule, ShortcutAssetModule);