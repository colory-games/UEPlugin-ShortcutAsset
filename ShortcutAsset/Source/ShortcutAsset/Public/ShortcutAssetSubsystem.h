#pragma once

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

	void OpenShortcutAssetEditor(TArray<TObjectPtr<UObject>> ObjectsToEdit);
	void NotifyShortcutAssetEditorClosed(TArray<TObjectPtr<UObject>> ObjectsAreEditing);

	TMap<TObjectPtr<UObject>, TObjectPtr<UShortcutAssetEditor>> OpenedEditorInstances;

	TSharedPtr<IToolkitHost> ToolKitHostRef;
};
