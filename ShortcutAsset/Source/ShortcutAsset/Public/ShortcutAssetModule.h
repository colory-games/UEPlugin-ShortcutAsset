#pragma once

#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FShortcutAssetModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
	TArray<TSharedRef<FSlateStyleSet>> RegisteredSlateStyleSets;

	void RegisterActions();
	void UnregisterActions();

	void RegisterStyles();
	void UnregisterStyles();
};
