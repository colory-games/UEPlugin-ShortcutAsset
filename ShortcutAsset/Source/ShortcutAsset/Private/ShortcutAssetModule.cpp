/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetModule.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IContentBrowserSingleton.h"
#include "ShortcutAsset.h"
#include "ShortcutAssetActions.h"
#include "ShortcutAssetUtils.h"
#include "Styling/SlateStyleRegistry.h"
#include "ToolMenus.h"
#include "ShortcutAssetThumbnailRenderer.h"
#include "ShortcutAssetSubsystem.h"

void FShortcutAssetModule::StartupModule()
{
	UThumbnailManager::Get().RegisterCustomRenderer(UShortcutAsset::StaticClass(), UShortcutAssetThumbnailRenderer::StaticClass());

	RegisterActions();
	RegisterStyles();
	RegisterMenus();
	//RegisterHandle();		// If enabled, Unreal Engine will be crashed randomly.
}

void FShortcutAssetModule::ShutdownModule()
{
	//UnregisterHandle();
	UnregisterMenus();
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
	TSharedRef<IAssetTypeActions> ShortcutAssetActions = MakeShareable(new FShortcutAssetEditLinkActions());

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
	RegisteredAssetTypeActions.Empty();
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
	RegisteredSlateStyleSets.Empty();
}

void FShortcutAssetModule::RegisterMenus()
{
	FToolMenuOwnerScoped Owner(this);

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	UToolMenu* ContextMenu = UToolMenus::Get()->ExtendMenu("ContentBrowser.AddNewContextMenu");
	FToolMenuSection& ContextMenuSection = ContextMenu->FindOrAddSection("ContentBrowserNewAsset");
	ContextMenuSection.AddDynamicEntry("Shortcut", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection) {
		FString Clipboard;
		FPlatformApplicationMisc::ClipboardPaste(Clipboard);

		EShortcutAssetLinkType LinkType = EShortcutAssetLinkType::MAX;
		FString Path;
		if (GetObjectPathFromClipboard(Clipboard, Path))
		{
			LinkType = EShortcutAssetLinkType::Asset;
		}
		else if (GetDirectoryPathFromClipboard(Clipboard, Path))
		{
			LinkType = EShortcutAssetLinkType::DirectoryPath;
		}
		else
		{
			return;
		}

		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();
		TArray<FString> CurrentPaths;
		ContentBrowserSingleton.GetSelectedPathViewFolders(CurrentPaths);
		if (CurrentPaths.Num() != 1)
		{
			return;
		}

		FString CurrentPath = CurrentPaths[0];
		if (CurrentPath.StartsWith("/All/Game"))
		{
			CurrentPath = CurrentPath.Replace(TEXT("/All/Game"), TEXT("/Game"));
		}

		switch (LinkType)
		{
			case EShortcutAssetLinkType::Asset:
			{
				FAssetRegistryModule& AssetRegistryModule =
					FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*Path));
				FString PackageName = FPackageName::ObjectPathToPackageName(Path);
				FString AssetName = FPackageName::GetLongPackageAssetName(AssetData.GetAsset()->GetOutermost()->GetName());

				InSection.AddMenuEntry("Shortcut", FText::FromString(FString::Printf(TEXT("Shortcut to %s"), *AssetName)),
					FText::FromString(FString::Printf(TEXT("Create an asset link to %s."), *AssetName)),
					FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"), MakeCreateAssetLinkAction(AssetData, CurrentPath));
				break;
			}
			case EShortcutAssetLinkType::DirectoryPath:
			{
				InSection.AddMenuEntry("Shortcut", FText::FromString(FString::Printf(TEXT("Shortcut to %s"), *Path)),
					FText::FromString(FString::Printf(TEXT("Create an asset link to %s."), *Path)),
					FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"),
					MakeCreateDirectoryPathLinkAction(Path, CurrentPath));
				break;
			}
			default:
				break;
		}
	}));

	FContentBrowserMenuExtender_SelectedPaths PathExtender =
		FContentBrowserMenuExtender_SelectedPaths::CreateLambda([](const TArray<FString>& SelectedPaths) {
			TSharedRef<FExtender> Extender = MakeShared<FExtender>();

			Extender->AddMenuExtension("PathViewFolderOptions", EExtensionHook::Before, nullptr,
				FMenuExtensionDelegate::CreateLambda([SelectedPaths](FMenuBuilder& MenuBuilder) {
					if (SelectedPaths.Num() != 1)
					{
						return;
					}

					FString Path = SelectedPaths[0];

					MenuBuilder.BeginSection("ShortcutActionSession", FText::FromString("Shortcut Action"));
					MenuBuilder.AddMenuEntry(FText::FromString("Create Shortcut"),
						FText::FromString("Create a directory shortcut."),
						FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"), MakeCreateDirectoryPathLinkAction(Path));
					MenuBuilder.EndSection();
				}));

			return Extender;
		});
	ContentBrowserModule.GetAllPathViewContextMenuExtenders().Add(PathExtender);
	RegisteredContentBrowserDirectoryPathExtenderHandles.Add(PathExtender.GetHandle());

	FContentBrowserMenuExtender_SelectedAssets AssetExtender =
		FContentBrowserMenuExtender_SelectedAssets::CreateLambda([](const TArray<FAssetData>& SelectedAssets) {
			TSharedRef<FExtender> Extender = MakeShared<FExtender>();

			Extender->AddMenuExtension("GetAssetActions", EExtensionHook::After, nullptr,
				FMenuExtensionDelegate::CreateLambda([SelectedAssets](FMenuBuilder& MenuBuilder) {
					if (SelectedAssets.Num() != 1)
					{
						return;
					}

					FAssetData AssetData = SelectedAssets[0];
					UObject* Asset = SelectedAssets[0].GetAsset();
					auto ShortcutAsset = Cast<UShortcutAsset>(Asset);
					if (ShortcutAsset != nullptr)
					{
						return;
					}

					MenuBuilder.BeginSection("ShortcutActionSession", FText::FromString("Shortcut Action"));
					MenuBuilder.AddMenuEntry(FText::FromString("Create Shortcut"), FText::FromString("Create an asset shortcut."),
						FSlateIcon("ShortcutAssetStyle", "ClassIcon.ShortcutAsset"), MakeCreateAssetLinkAction(AssetData));
					MenuBuilder.EndSection();
				}));

			return Extender;
		});
	ContentBrowserModule.GetAllAssetViewContextMenuExtenders().Add(AssetExtender);
	RegisteredContentBrowserAssetExtenderHandles.Add(AssetExtender.GetHandle());
}

void FShortcutAssetModule::UnregisterMenus()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	for (auto ExtenderHandle : RegisteredContentBrowserAssetExtenderHandles)
	{
		ContentBrowserModule.GetAllAssetViewContextMenuExtenders().RemoveAll(
			[ExtenderHandle](
				const FContentBrowserMenuExtender_SelectedAssets& InDelegate) { return InDelegate.GetHandle() == ExtenderHandle; });
	}
	RegisteredContentBrowserAssetExtenderHandles.Empty();

	for (auto ExtenderHandle : RegisteredContentBrowserDirectoryPathExtenderHandles)
	{
		ContentBrowserModule.GetAllPathViewContextMenuExtenders().RemoveAll(
			[ExtenderHandle](
				const FContentBrowserMenuExtender_SelectedPaths& InDelegate) { return InDelegate.GetHandle() == ExtenderHandle; });
	}
	RegisteredContentBrowserDirectoryPathExtenderHandles.Empty();

	UToolMenus::Get()->UnregisterOwner(this);
}

void FShortcutAssetModule::RefreshAssetThumbnailsInDirectory(const FString& DirectoryPath)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FString SearchPath = DirectoryPath;
	if (SearchPath.StartsWith(TEXT("/All/")))
	{
		SearchPath = SearchPath.RightChop(4);
	}

	FARFilter Filter;
	Filter.PackagePaths.Add(FName(*SearchPath));
	Filter.bRecursivePaths = false;
	Filter.bIncludeOnlyOnDiskAssets = false;
#if UE_VERSION_NEWER_THAN(5, 1, 0)
	Filter.ClassPaths.Add(UShortcutAsset::StaticClass()->GetClassPathName());
#else
	Filter.ClassNames.Add(UShortcutAsset::StaticClass()->GetFName());
#endif

	TArray<FAssetData> Assets;
	AssetRegistry.GetAssets(Filter, Assets);
	for (const FAssetData& AssetData : Assets)
	{
		UShortcutAsset* ShortcutAsset = Cast<UShortcutAsset>(AssetData.GetAsset());
		if (ShortcutAsset == nullptr)
		{
			continue;
		}

		if (ShortcutAsset->CachedThumbnail != nullptr)
		{
			continue;
		}

		UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
		Subsystem->RefreshThumbnail(ShortcutAsset);
	}
}

void FShortcutAssetModule::RegisterHandle()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	ContentBrowserDirectoryChangedHandle = ContentBrowserModule.GetOnAssetPathChanged().AddRaw(this, &FShortcutAssetModule::RefreshAssetThumbnailsInDirectory);
}

void FShortcutAssetModule::UnregisterHandle()
{
	if (FModuleManager::Get().IsModuleLoaded("ContentBrowser"))
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::GetModuleChecked<FContentBrowserModule>("ContentBrowser");

		ContentBrowserModule.GetOnAssetPathChanged().Remove(ContentBrowserDirectoryChangedHandle);
	}
}

IMPLEMENT_MODULE(FShortcutAssetModule, ShortcutAsset);
