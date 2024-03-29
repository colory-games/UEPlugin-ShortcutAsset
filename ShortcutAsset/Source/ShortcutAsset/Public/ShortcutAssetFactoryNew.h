/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "ShortcutAssetFactoryNew.generated.h"

UCLASS()
class UShortcutAssetFactoryNew : public UFactory
{
	GENERATED_BODY()

public:
	UShortcutAssetFactoryNew(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(
		UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
	virtual FString GetDefaultNewAssetName() const override;
};