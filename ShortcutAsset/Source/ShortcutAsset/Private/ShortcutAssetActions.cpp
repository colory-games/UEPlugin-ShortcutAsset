#include "ShortcutAssetActions.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ShortcutAsset.h"
#include "ShortcutAssetEditorToolkit.h"
#include "ShortcutAssetSubsystem.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

FShortcutAssetActions::FShortcutAssetActions()
{
}

bool FShortcutAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FShortcutAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	if (InObjects.Num() != 1)
	{
		return;
	}

	auto ShortcutAsset = Cast<UShortcutAsset>(InObjects[0]);
	if (ShortcutAsset == nullptr)
	{
		return;
	}

	// clang-format off
	MenuBuilder.AddMenuEntry(
		LOCTEXT("ShortcutAsset_EditLinkedAssetPath", "Edit Link"),
		LOCTEXT("ShortcutAsset_EditLinkedAssetPathTip", "Edit the link saved on the shortcut asset."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=]
			{
				TArray<UObject*> Objects = {InObjects[0]};
				UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
				Subsystem->OpenShortcutAssetEditor(Objects);
			}),
			FCanExecuteAction::CreateLambda([=]
			{
				return true;
			})
		)
	);
	// clang-format on
}

void FShortcutAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for (auto It = InObjects.CreateConstIterator(); It; ++It)
	{
		auto ShortcutAsset = Cast<UShortcutAsset>(*It);
		if (ShortcutAsset != nullptr)
		{
			switch (ShortcutAsset->LinkType)
			{
				case EShortcutAssetLinkType::Asset:
				{
					if (ShortcutAsset->LinkedAsset != nullptr)
					{
						GEditor->EditObject(ShortcutAsset->LinkedAsset);
					}
					break;
				}
				case EShortcutAssetLinkType::SoftAsset:
				{
					FString LinkedPath = ShortcutAsset->LinkedAssetPath.GetAssetPathString();
					const FString DISPLAY_NAME_FOR_GAME_FOLDER = "/Content";
					if (LinkedPath.StartsWith(DISPLAY_NAME_FOR_GAME_FOLDER, ESearchCase::CaseSensitive))
					{
						LinkedPath = "/Game" + LinkedPath.RightChop(DISPLAY_NAME_FOR_GAME_FOLDER.Len());
					}

					FAssetRegistryModule& AssetRegistryModule =
						FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
					FARFilter Filter;
					TArray<FAssetData> AssetData;
					FString DirectoryPath;
					if (LinkedPath.Split(TEXT("/"), &DirectoryPath, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
					{
						Filter.PackagePaths.Add(*DirectoryPath);
						AssetRegistryModule.Get().GetAssets(Filter, AssetData);

						for (auto& Data : AssetData)
						{
							if (Data.ObjectPath.ToString() == LinkedPath)
							{
								UObject* Asset = Data.GetAsset();
								GEditor->EditObject(Asset);
								break;
							}
						}
					}

					break;
				}
				case EShortcutAssetLinkType::Directory:
				{
					FString LinkedPath = ShortcutAsset->LinkedDirectoryPath.Path;
					const FString DISPLAY_NAME_FOR_GAME_FOLDER = "/Content";
					if (LinkedPath.StartsWith(DISPLAY_NAME_FOR_GAME_FOLDER, ESearchCase::CaseSensitive))
					{
						LinkedPath = "/Game" + LinkedPath.RightChop(DISPLAY_NAME_FOR_GAME_FOLDER.Len());
					}

					// TODO: Use UEditorUtilityLibrary::SyncBrowserToFolders for UE 5.1.
					//       When implementing this, remove #include "ContentBrowserModule.h" and
					//       #include "IContentBrowserSingleton.h" from this file, and "ContentBrowser"
					//       module from ShortcutAsset.Build.cs.
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
					ContentBrowserModule.Get().SyncBrowserToFolders({LinkedPath}, false, true);
					break;
				}
			}
		}
	}
}

uint32 FShortcutAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

FText FShortcutAssetActions::GetName() const
{
	return LOCTEXT("AssetTypeActions_Shortcut", "Shortcut");
}

UClass* FShortcutAssetActions::GetSupportedClass() const
{
	return UShortcutAsset::StaticClass();
}

FColor FShortcutAssetActions::GetTypeColor() const
{
	return FColor::White;
}

#undef LOCTEXT_NAMESPACE