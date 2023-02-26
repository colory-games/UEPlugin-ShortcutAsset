#pragma once

#include "EditorSubsystem.h"
#include "ShortcutAssetEditor.h"
#include "ShortcutAssetEditorToolkit.h"

#include "ShortcutAssetSubsystem.generated.h"

UCLASS()
class SHORTCUTASSET_API UShortcutAssetSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	UShortcutAssetEditor* ShortcutAssetEditor = nullptr;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void OpenShortcutAssetEditor(TArray<UObject*> ObjectsToEdit);
	void NotifyShortcutAssetEditorClosed(TArray<UObject*> ObjectsAreEditing);

	TMap<UObject*, UShortcutAssetEditor*> OpenedEditorInstances;

	TSharedPtr<IToolkitHost> ToolKitHostRef;
};
