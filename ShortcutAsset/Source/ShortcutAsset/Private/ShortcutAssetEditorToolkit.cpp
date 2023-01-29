#include "ShortcutAssetEditorToolkit.h"

#include "EditorReimportHandler.h"
#include "ShortcutAssetSubsystem.h"
#include "EditorModeManager.h"
#include "SShortcutAssetEditor.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

static const FName AppID("ShortcutAssetEditor");
static const FName TabID("Shortcut Asset Editor");

TSharedRef<SDockTab> FShortcutAssetEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == TabID)
	{
		UShortcutAssetEditor* Editor = CastChecked<UShortcutAssetEditor>(OwningAssetEditor);
		TArray<TObjectPtr<UObject>> ShortcutAssets = Editor->ObjectsToEdit;
		TObjectPtr<UShortcutAsset> SA = CastChecked<UShortcutAsset>(ShortcutAssets[0]);

		TabWidget = SNew(SShortcutAssetEditor, SA);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}

FShortcutAssetEditorToolkit::FShortcutAssetEditorToolkit(UAssetEditor* InOwningAssetEditor) : FBaseAssetToolkit(InOwningAssetEditor)
{
	StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_ShortcutAssetEditor")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split(
				FTabManager::NewStack()
				->AddTab(TabID, ETabState::OpenedTab)
				->SetHideTabWell(true)
				->SetSizeCoefficient(1.0f)
			)
		);
}

FShortcutAssetEditorToolkit::~FShortcutAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
	if (Subsystem)
	{
		TArray<TObjectPtr<UObject>> ObjectsToEdit;
		OwningAssetEditor->GetObjectsToEdit(ObjectsToEdit);
		Subsystem->NotifyShortcutAssetEditorClosed(ObjectsToEdit);
	}
}

FName FShortcutAssetEditorToolkit::GetToolkitFName() const
{
	return FName("ShortcutAssetEditor");
}

FText FShortcutAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLevel", "Shortcut Asset Editor");
}

FLinearColor FShortcutAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

FString FShortcutAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Shortcut Asset ").ToString();
}

FString FShortcutAssetEditorToolkit::GetDocumentationLink() const
{
	return FString("https://github.com/colory-games/UEPlugin-Shortcut-Asset");
}

void FShortcutAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(
		LOCTEXT("WorkspaceMenu_ShortcutAssetEditor", "Shortcut Asset Editor")
	);

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(TabID, FOnSpawnTab::CreateSP(this, &FShortcutAssetEditorToolkit::HandleTabManagerSpawnTab, TabID))
		.SetDisplayName(LOCTEXT("ShortcutAssetEditorTabName", "Shortcut Asset Editor"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}

void FShortcutAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(TabID);
}

#undef LOCTEXT_NAMESPACE