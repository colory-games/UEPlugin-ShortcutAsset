/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetActions.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "ShortcutAsset.h"
#include "ShortcutAssetEditorToolkit.h"
#include "ShortcutAssetFactoryNew.h"
#include "ShortcutAssetSubsystem.h"
#include "ShortcutAssetUtils.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

FShortcutAssetEditLinkActions::FShortcutAssetEditLinkActions()
{
}

bool FShortcutAssetEditLinkActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}

void FShortcutAssetEditLinkActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
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
		FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
		FUIAction(
			FExecuteAction::CreateLambda([=]
			{
#ifdef SA_FREE_VERSION
				if (ReachFreeVersionLimitation())
				{
					return;
				}
#endif	  // SA_FREE_VERSION

				TArray<OBJECT_PTR(UObject)> Objects = {InObjects[0]};
				UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
				Subsystem->OpenShortcutAssetEditor(Objects);
			}),
			FCanExecuteAction::CreateLambda([=]
			{
				return true;
			})
		)
	);

	if (ShortcutAsset->LinkType == EShortcutAssetLinkType::Asset)
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("ShortcutAsset_OpenLinkedAssetLocation", "Open Asset Location"),
			LOCTEXT("ShortcutAsset_OpenLinkedAssetLocationTip", "Open directory where the linked asset is located."),
			FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
			FUIAction(
				FExecuteAction::CreateLambda([=]
				{
					FString AssetPath = ShortcutAsset->LinkedAsset.GetAssetPathString();
					FString LocationPath = FPaths::GetPath(AssetPath);
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

					ContentBrowserModule.Get().SyncBrowserToFolders({ LocationPath }, false, false);
				}),
				FCanExecuteAction::CreateLambda([=]
				{
					return ShortcutAsset->LinkType == EShortcutAssetLinkType::Asset;
				})
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("ShortcutAsset_OpenLinkedAssetLocationInNewContentBrowser", "Open Asset Location in New Content Browser"),
			LOCTEXT("ShortcutAsset_OpenLinkedAssetLocationInNewContentBrowserTip", "Open directory where the linked asset is located (Open in new Content Browser)."),
			FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
			FUIAction(
				FExecuteAction::CreateLambda([=]
				{
					FString AssetPath = ShortcutAsset->LinkedAsset.GetAssetPathString();
					FString LocationPath = FPaths::GetPath(AssetPath);
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

					ContentBrowserModule.Get().SyncBrowserToFolders({ LocationPath }, false, false, FName(), true);
				}),
				FCanExecuteAction::CreateLambda([=]
				{
					return ShortcutAsset->LinkType == EShortcutAssetLinkType::Asset;
				})
			)
		);
	}
	else if (ShortcutAsset->LinkType == EShortcutAssetLinkType::DirectoryPath)
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryLocation", "Open Directory Location"),
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryLocationTip", "Open directory where the linked asset is located."),
			FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
			FUIAction(
				FExecuteAction::CreateLambda([=]
				{
					FString DirectoryPath = ShortcutAsset->LinkedDirectoryPath.Path;
					FString LocationPath = FPaths::GetPath(DirectoryPath);
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

					ContentBrowserModule.Get().SyncBrowserToFolders({ LocationPath }, false, false);
				}),
				FCanExecuteAction::CreateLambda([=]
				{
					return ShortcutAsset->LinkType == EShortcutAssetLinkType::DirectoryPath;
				})
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryLocationInNewContentBrowser", "Open Directory Location in New Content Browser"),
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryLocationInNewContentBrowserTip", "Open directory where the linked directory is located (Open in new Content Browser)."),
			FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
			FUIAction(
				FExecuteAction::CreateLambda([=]
				{
					FString DirectoryPath = ShortcutAsset->LinkedDirectoryPath.Path;
					FString LocationPath = FPaths::GetPath(DirectoryPath);
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

					ContentBrowserModule.Get().SyncBrowserToFolders({ LocationPath }, false, false, FName(), true);
				}),
				FCanExecuteAction::CreateLambda([=]
				{
					return ShortcutAsset->LinkType == EShortcutAssetLinkType::DirectoryPath;
				})
			)
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryInNewContentBrowser", "Open Linked Directory in New Content Browser"),
			LOCTEXT("ShortcutAsset_OpenLinkedDirectoryInNewContentBrowserTip", "Open the liked directory in new Content Browser."),
			FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
			FUIAction(
				FExecuteAction::CreateLambda([=]
				{
					FString DirectoryPath = ShortcutAsset->LinkedDirectoryPath.Path;
					FContentBrowserModule& ContentBrowserModule =
						FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

					ContentBrowserModule.Get().SyncBrowserToFolders({ DirectoryPath }, false, false, FName(), true);
				}),
				FCanExecuteAction::CreateLambda([=]
				{
					return ShortcutAsset->LinkType == EShortcutAssetLinkType::DirectoryPath;
				})
			)
		);
	}
	// clang-format on
}

void FShortcutAssetEditLinkActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
#ifdef SA_FREE_VERSION
	if (ReachFreeVersionLimitation())
	{
		return;
	}
#endif	  // SA_FREE_VERSION

	for (auto It = InObjects.CreateConstIterator(); It; ++It)
	{
		auto ShortcutAsset = Cast<UShortcutAsset>(*It);
		if (ShortcutAsset != nullptr)
		{
			bool bOpenShortcutEditor = false;
			switch (ShortcutAsset->LinkType)
			{
				case EShortcutAssetLinkType::Asset:
				{
					FString LinkedPath = ShortcutAsset->LinkedAsset.GetAssetPathString();
					if (LinkedPath.IsEmpty())
					{
						bOpenShortcutEditor = true;
						break;
					}

					UObject* Asset = ShortcutAsset->LinkedAsset.ResolveObject();
					if (Asset == nullptr)
					{
						Asset = ShortcutAsset->LinkedAsset.TryLoad();
					}
					if (Asset == nullptr)
					{
						bOpenShortcutEditor = true;
						break;
					}
					GEditor->EditObject(Asset);
					break;
				}
				case EShortcutAssetLinkType::DirectoryPath:
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
					ContentBrowserModule.Get().SyncBrowserToFolders({LinkedPath}, false, false);

					// Check if the current path is changed.
#if UE_VERSION_OLDER_THAN(5, 0, 0)
					FString CurrentPath = ContentBrowserModule.Get().GetCurrentPath();
					if (CurrentPath != LinkedPath)
					{
						bOpenShortcutEditor = true;
					}
#else
					FContentBrowserItemPath CurrentPath = ContentBrowserModule.Get().GetCurrentPath();
					if (CurrentPath.GetInternalPathName().ToString() != LinkedPath)
					{
						bOpenShortcutEditor = true;
					}
#endif
					break;
				}
			}	 // switch

			if (bOpenShortcutEditor)
			{
				FString Message = FString::Format(
					TEXT("The link stored in '{0}' is missing.\nDo you want to edit the link?"), {ShortcutAsset->GetName()});
				FText TitleText = LOCTEXT("Title", "Missing Link Error");
				FText MessageText = FText::AsCultureInvariant(Message);
#if UE_VERSION_OLDER_THAN(5, 4, 0)
				if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, &TitleText) == EAppReturnType::Ok)
#else
				if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, TitleText) == EAppReturnType::Ok)
#endif
				{
					UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
					Subsystem->OpenShortcutAssetEditor({ShortcutAsset});
				}
			}
		}
	}
}

uint32 FShortcutAssetEditLinkActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

FText FShortcutAssetEditLinkActions::GetName() const
{
	return LOCTEXT("AssetTypeActions_Shortcut", "Shortcut");
}

UClass* FShortcutAssetEditLinkActions::GetSupportedClass() const
{
	return UShortcutAsset::StaticClass();
}

FColor FShortcutAssetEditLinkActions::GetTypeColor() const
{
	return FColor::White;
}

FUIAction MakeCreateDirectoryPathLinkAction(const FString& Path, FString PathToCreate)
{
	return FUIAction(FExecuteAction::CreateLambda([Path, PathToCreate]() {
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		UShortcutAssetFactoryNew* Factory = NewObject<UShortcutAssetFactoryNew>();
		FString ParentPath = FPaths::GetPath(Path);
		FString DirectoryName = FPaths::GetBaseFilename(Path);
		FString DirectoryPathToCreate = PathToCreate == "" ? ParentPath : PathToCreate;
		FString AssetNameToCreate = FString::Printf(TEXT("%s_Shortcut"), *DirectoryName);

		UObject* NewAsset =
			AssetTools.CreateAsset(AssetNameToCreate, DirectoryPathToCreate, UShortcutAsset::StaticClass(), Factory);
		UShortcutAsset* NewShortcutAsset = Cast<UShortcutAsset>(NewAsset);
		if (NewShortcutAsset == nullptr)
		{
			return;
		}
		NewShortcutAsset->LinkType = EShortcutAssetLinkType::DirectoryPath;
		NewShortcutAsset->LinkedDirectoryPath.Path = Path;

		// Move to the folder which the asset is created.
		FContentBrowserModule& ContentBrowserModule =
			FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		ContentBrowserModule.Get().SyncBrowserToFolders({DirectoryPathToCreate}, false, false);
	}));
}

FUIAction MakeCreateAssetLinkAction(const FAssetData& AssetData, FString PathToCreate)
{
	return FUIAction(FExecuteAction::CreateLambda([AssetData, PathToCreate]() {
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		UShortcutAssetFactoryNew* Factory = NewObject<UShortcutAssetFactoryNew>();
		UPackage* Package = AssetData.GetAsset()->GetOutermost();
		FString PackagePath = FPackageName::GetLongPackagePath(Package->GetName());
		FString AssetName = FPackageName::GetLongPackageAssetName(Package->GetName());
		FString DirectoryPathToCreate = PathToCreate == "" ? PackagePath : PathToCreate;
		FString AssetNameToCreate = FString::Printf(TEXT("%s_Shortcut"), *AssetName);

		UObject* NewAsset =
			AssetTools.CreateAsset(AssetNameToCreate, DirectoryPathToCreate, UShortcutAsset::StaticClass(), Factory);
		UShortcutAsset* NewShortcutAsset = Cast<UShortcutAsset>(NewAsset);
		if (NewShortcutAsset == nullptr)
		{
			return;
		}
		NewShortcutAsset->LinkType = EShortcutAssetLinkType::Asset;
#if UE_VERSION_NEWER_THAN(5, 1, 0)
		NewShortcutAsset->LinkedAsset = AssetData.GetSoftObjectPath();
#else
		NewShortcutAsset->LinkedAsset = AssetData.ObjectPath;
#endif

		// Move to the folder which the asset is created.
		FContentBrowserModule& ContentBrowserModule =
			FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		ContentBrowserModule.Get().SyncBrowserToFolders({DirectoryPathToCreate}, false, false);
	}));
}

#undef LOCTEXT_NAMESPACE
