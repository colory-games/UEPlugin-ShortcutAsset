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
			bool bOpenShortcutEditor = false;
			switch (ShortcutAsset->LinkType)
			{
				case EShortcutAssetLinkType::Asset:
				{
					FString LinkedPath = ShortcutAsset->LinkedAsset.GetAssetPathString();
					UObject* Asset = ShortcutAsset->LinkedAsset.ResolveObject();
					if (Asset == nullptr)
					{
						Asset = ShortcutAsset->LinkedAsset.TryLoad();
					}
					if (Asset != nullptr)
					{
						GEditor->EditObject(Asset);
					}
					else
					{
						bOpenShortcutEditor = true;
					}
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
					FContentBrowserItemPath CurrentPath = ContentBrowserModule.Get().GetCurrentPath();
					if (CurrentPath.GetInternalPathName().ToString() != LinkedPath)
					{
						bOpenShortcutEditor = true;
					}
					break;
				}
			}	 // switch

			if (bOpenShortcutEditor)
			{
				FString Message = FString::Format(
					TEXT("The link stored in '{0}' is missing.\nDo you want to edit the link?"), {ShortcutAsset->GetName()});
				FText TitleText = LOCTEXT("Title", "Missing Link Error");
				FText MessageText = FText::AsCultureInvariant(Message);
				if (FMessageDialog::Open(EAppMsgType::OkCancel, MessageText, &TitleText) == EAppReturnType::Ok)
				{
					UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
					Subsystem->OpenShortcutAssetEditor({ShortcutAsset});
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