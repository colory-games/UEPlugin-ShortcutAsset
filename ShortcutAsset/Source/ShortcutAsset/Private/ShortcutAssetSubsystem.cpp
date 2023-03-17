
/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetSubsystem.h"

void UShortcutAssetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UShortcutAssetSubsystem::Deinitialize()
{
}

void UShortcutAssetSubsystem::OpenShortcutAssetEditor(TArray<OBJECT_PTR(UObject)> ObjectsToEdit)
{
	// TODO: Handle more than 2 objects at once.
	if (ObjectsToEdit.Num() == 1)
	{
		OBJECT_PTR(UObject) Object = ObjectsToEdit[0];
		if (OpenedEditorInstances.Contains(Object))
		{
			OpenedEditorInstances[Object]->GetInstanceInterface()->FocusWindow(Object);
			return;
		}

		UShortcutAsset* ShortcutAsset = Cast<UShortcutAsset>(Object);
		if (ShortcutAsset == nullptr)
		{
			return;
		}

		UShortcutAssetEditor* ShortcutAssetEditor = NewObject<UShortcutAssetEditor>(this);
		ShortcutAssetEditor->Initialize(ObjectsToEdit);

		for (auto& O : ObjectsToEdit)
		{
			OpenedEditorInstances.Add(O, ShortcutAssetEditor);
		}
	}
}

void UShortcutAssetSubsystem::NotifyShortcutAssetEditorClosed(TArray<OBJECT_PTR(UObject)> ObjectsAreEditing)
{
	for (auto Object : ObjectsAreEditing)
	{
		OpenedEditorInstances.Remove(Object);
	}
}
