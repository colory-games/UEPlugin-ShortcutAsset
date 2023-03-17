/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "Compatibility.h"
#include "EditorSubsystem.h"
#include "ShortcutAssetEditor.h"
#include "ShortcutAssetEditorToolkit.h"

#include "ShortcutAssetSubsystem.generated.h"

UCLASS()
class SHORTCUTASSET_API UShortcutAssetSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void OpenShortcutAssetEditor(TArray<OBJECT_PTR(UObject)> ObjectsToEdit);
	void NotifyShortcutAssetEditorClosed(TArray<OBJECT_PTR(UObject)> ObjectsAreEditing);

	TMap<OBJECT_PTR(UObject), OBJECT_PTR(UShortcutAssetEditor)> OpenedEditorInstances;

	TSharedPtr<IToolkitHost> ToolKitHostRef;
};
