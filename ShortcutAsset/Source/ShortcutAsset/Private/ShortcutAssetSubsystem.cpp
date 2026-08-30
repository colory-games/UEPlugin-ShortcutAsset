
/*!
 * ShortcutAsset
 *
 * Copyright (c) 2023 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetSubsystem.h"

#include "Slate/WidgetRenderer.h"
#include "Engine/TextureRenderTarget2D.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ThumbnailRendering/ThumbnailManager.h"

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

static UTexture2D* CreateTextureFromBrush(const FSlateBrush* Brush, uint32 Width, uint32 Height)
{
	if (Brush == nullptr)
	{
		return nullptr;
	}

	if (Brush->GetImageType() != ESlateBrushImageType::Vector)
	{
		return nullptr;
	}

	FName ResourceName = Brush->GetResourceName();
	if (ResourceName == NAME_None || !FPaths::FileExists(ResourceName.ToString()))
	{
		return nullptr;
	}

	UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>();
	RT->InitAutoFormat(Width, Height);
	RT->ClearColor = FLinearColor::Black;
	RT->UpdateResource();

	FWidgetRenderer WidgetRenderer(true);
	TSharedRef<SWidget> Widget = SNew(SBox)
		.WidthOverride(Width)
		.HeightOverride(Height)
		[
			SNew(SImage).DesiredSizeOverride(FVector2D(Width, Height)).Image(Brush).ColorAndOpacity(FLinearColor::White)
		];
	Widget->SlatePrepass(1.0f);
	WidgetRenderer.DrawWidget(RT, Widget, FVector2D(Width, Height), 0.0f);
	FlushRenderingCommands();

	TArray<FColor> OutPixels;
	RT->GameThread_GetRenderTargetResource()->ReadPixels(OutPixels);

	// If all pixels are white color, we will handle this as an invalid texture.
	bool bIsAllWhite = true;
	for (auto P : OutPixels)
	{
		if (P != FColor::White)
		{
			bIsAllWhite = false;
			break;
		}
	}
	if (bIsAllWhite)
	{
		return nullptr;
	}

	UTexture2D* NewTexture = NewObject<UTexture2D>(
		GetTransientPackage(),
		*FString::Printf(TEXT("SVGThumbnail_%s"), *ResourceName.ToString()),
		RF_Transient);
	NewTexture->Source.Init(Width, Height, 1, 1, TSF_BGRA8);
	uint8* MipData = NewTexture->Source.LockMip(0);
	FMemory::Memcpy(MipData, OutPixels.GetData(), OutPixels.Num() * sizeof(FColor));
	NewTexture->Source.UnlockMip(0);
	NewTexture->SRGB = true;
	NewTexture->CompressionSettings = TC_Default;
	NewTexture->UpdateResource();

	return NewTexture;
}

void UShortcutAssetSubsystem::RefreshThumbnail(UShortcutAsset* ShortcutAsset)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	if (ShortcutAsset->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		ShortcutAsset->CachedThumbnail = nullptr;
		return;
	}

	FAssetData LinkedAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ShortcutAsset->LinkedAsset);
	UObject* LinkedObject = LinkedAssetData.GetAsset();
	if (LinkedObject == nullptr)
	{
		ShortcutAsset->CachedThumbnail = nullptr;
		return;
	}

	// If there is rendering info, draw as it is.
	FThumbnailRenderingInfo* Info = UThumbnailManager::Get().GetRenderingInfo(LinkedObject);
	if (Info)
	{
		ShortcutAsset->CachedThumbnail = nullptr;
		return;
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(LinkedObject);
	if (Blueprint != nullptr)
	{
		UClass* GeneratedClass = Blueprint->GeneratedClass;
		if (GeneratedClass != nullptr)
		{
			UClass* SuperClass = GeneratedClass;
			while (SuperClass != nullptr)
			{
				FString ClassName = SuperClass->GetFName().ToString();
				FString PropertyName = FString::Printf(TEXT("ClassThumbnail.%s"), *ClassName);
				const FSlateBrush* Brush = FAppStyle::GetBrush(*PropertyName);
				if (Brush != nullptr && !ClassName.EndsWith("_C"))
				{
					ShortcutAsset->CachedThumbnail = CreateTextureFromBrush(Brush, 128, 128);
					break;
				}
				SuperClass = SuperClass->GetSuperClass();
			}
		}
	}
}
