#include "ShortcutAssetFactoryNew.h"

#include "ShortcutAsset.h"

UShortcutAssetFactoryNew::UShortcutAssetFactoryNew(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UShortcutAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UShortcutAssetFactoryNew::FactoryCreateNew(
	UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UShortcutAsset>(InParent, InClass, InName, Flags);
}

bool UShortcutAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}