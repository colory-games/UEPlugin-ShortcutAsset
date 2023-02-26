#include "ShortcutAssetFactoryNew.h"

#include "ShortcutAsset.h"
#include "ShortcutAssetUtils.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"

UShortcutAssetFactoryNew::UShortcutAssetFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UShortcutAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UShortcutAssetFactoryNew::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
#ifdef SA_FREE_VERSION
	if (ReachFreeVersionLimitation(true))
	{
		return nullptr;
	}
#endif	  // SA_FREE_VERSION

	return NewObject<UShortcutAsset>(InParent, InClass, InName, Flags);
}

bool UShortcutAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}

FString UShortcutAssetFactoryNew::GetDefaultNewAssetName() const
{
	return "NewShortcut";
}

#undef LOCTEXT_NAMESPACE