#include "ShortcutAssetEditor.h"

#include "ShortcutAssetSubsystem.h"

void UShortcutAssetEditor::Initialize(const TArray<UObject*>& InObjects)
{
	UShortcutAssetSubsystem* Subsystem = GEditor->GetEditorSubsystem<UShortcutAssetSubsystem>();

	ObjectsToEdit = InObjects;

	UAssetEditor::Initialize();
}

IAssetEditorInstance* UShortcutAssetEditor::GetInstanceInterface()
{
	return ToolkitInstance;
}

void UShortcutAssetEditor::GetObjectsToEdit(TArray<UObject*>& OutObjects)
{
	OutObjects.Append(ObjectsToEdit);
}

TSharedPtr<FBaseAssetToolkit> UShortcutAssetEditor::CreateToolkit()
{
	TSharedPtr<FShortcutAssetEditorToolkit> Toolkit = MakeShared<FShortcutAssetEditorToolkit>(this);

	return Toolkit;
}