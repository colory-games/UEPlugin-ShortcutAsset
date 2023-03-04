#include "ShortcutAssetEditorToolkit.h"

#include "EditorModeManager.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "SShortcutAssetEditor.h"
#include "ShortcutAssetSubsystem.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

static const FName AppID("ShortcutAssetEditor");
static const FName TabID("Shortcut Asset Editor");

TSharedRef<SDockTab> FShortcutAssetEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == TabID)
	{
		UShortcutAssetEditor* Editor = CastChecked<UShortcutAssetEditor>(OwningAssetEditor);
		TArray<UObject*> ShortcutAssets = Editor->ObjectsToEdit;
		UShortcutAsset* SA = CastChecked<UShortcutAsset>(ShortcutAssets[0]);

		TabWidget = SNew(SShortcutAssetEditor, SA);
	}

	// clang-format off
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
	// clang-format on
}

FShortcutAssetEditorToolkit::FShortcutAssetEditorToolkit(UAssetEditor* InOwningAssetEditor) : FBaseAssetToolkit(InOwningAssetEditor)
{
	// clang-format off
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
	// clang-format on
}

FShortcutAssetEditorToolkit::~FShortcutAssetEditorToolkit()
{
	UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();
	if (Subsystem)
	{
		TArray<OBJECT_PTR(UObject)> ObjectsToEdit;
		OwningAssetEditor->GetObjectsToEdit(ObjectsToEdit);
		Subsystem->NotifyShortcutAssetEditorClosed(ObjectsToEdit);
	}
}

FName FShortcutAssetEditorToolkit::GetToolkitFName() const
{
	return FName("ShortcutEditor");
}

FText FShortcutAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLevel", "Shortcut Editor");
}

FLinearColor FShortcutAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

FString FShortcutAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Shortcut ").ToString();
}

FString FShortcutAssetEditorToolkit::GetDocumentationLink() const
{
	return FString("https://github.com/colory-games/UEPlugin-Shortcut-Asset");
}

void FShortcutAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ShortcutEditor", "Shortcut Editor"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// clang-format off
	InTabManager->RegisterTabSpawner(TabID, FOnSpawnTab::CreateSP(this, &FShortcutAssetEditorToolkit::HandleTabManagerSpawnTab, TabID))
		.SetDisplayName(LOCTEXT("ShortcutEditorTabName", "Shortcut Editor"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
	// clang-format on
}

void FShortcutAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(TabID);
}

#undef LOCTEXT_NAMESPACE