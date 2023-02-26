#include "ShortcutAssetSubsystem.h"

void UShortcutAssetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UShortcutAssetSubsystem::Deinitialize()
{
}

void UShortcutAssetSubsystem::OpenShortcutAssetEditor(TArray<UObject*> ObjectsToEdit)
{
	// TODO: Handle more than 2 objects at once.
	if (ObjectsToEdit.Num() == 1)
	{
		UObject* Object = ObjectsToEdit[0];
		if (OpenedEditorInstances.Contains(Object))
		{
			OpenedEditorInstances[Object]->FocusWindow(Object);
			return;
		}

		UShortcutAsset* ShortcutAsset = Cast<UShortcutAsset>(Object);
		if (ShortcutAsset == nullptr)
		{
			return;
		}

		ShortcutAssetEditor = NewObject<UShortcutAssetEditor>(this);
		ShortcutAssetEditor->Initialize(ObjectsToEdit);

		for (auto& O : ObjectsToEdit)
		{
			OpenedEditorInstances.Add(O, ShortcutAssetEditor);
		}
	}
}

void UShortcutAssetSubsystem::NotifyShortcutAssetEditorClosed(TArray<UObject*> ObjectsAreEditing)
{
	for (auto Object : ObjectsAreEditing)
	{
		OpenedEditorInstances.Remove(Object);
	}
}
