#pragma once

#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FShortcutAssetModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};
