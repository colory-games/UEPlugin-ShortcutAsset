/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "Tools/UAssetEditor.h"

#include "ShortcutAssetEditor.generated.h"

UCLASS()
class SHORTCUTASSET_API UShortcutAssetEditor : public UAssetEditor
{
	GENERATED_BODY()

public:
	void Initialize(const TArray<UObject*>& InObjects);

	IAssetEditorInstance* GetInstanceInterface();

	void GetObjectsToEdit(TArray<UObject*>& OutObjects) override;
	virtual TSharedPtr<FBaseAssetToolkit> CreateToolkit() override;

	UPROPERTY()
	TArray<UObject*> ObjectsToEdit;
};
