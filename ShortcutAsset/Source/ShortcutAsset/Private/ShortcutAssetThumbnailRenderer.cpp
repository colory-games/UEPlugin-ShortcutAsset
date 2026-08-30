/*!
 * ShortcutAsset
 *
 * Copyright (c) 2026 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include "ShortcutAssetThumbnailRenderer.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ShortcutAssetSubsystem.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"

#define LOCTEXT_NAMESPACE "ShortcutAsset"


UShortcutAssetThumbnailRenderer::UShortcutAssetThumbnailRenderer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShortcutAssetThumbnailRenderer::GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const
{
	OutWidth = 128;
	OutHeight = 128;
}

void UShortcutAssetThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	UShortcutAsset* ShortcutAsset = Cast<UShortcutAsset>(Object);
	if (ShortcutAsset == nullptr)
	{
		return;
	}

	FAssetData LinkedAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ShortcutAsset->LinkedAsset);
	UObject* LinkedObject = LinkedAssetData.GetAsset();
	if (LinkedObject == nullptr)
	{
		return;
	}

	// If there is rendering info, draw as it is.
	FThumbnailRenderingInfo* Info = UThumbnailManager::Get().GetRenderingInfo(LinkedObject);
	if (Info)
	{
		auto Renderer = Info->Renderer;
		Renderer->Draw(LinkedObject, 0, 0, Width, Height, RenderTarget, Canvas, true);
		DrawOverlay(Width, Height, Canvas);
		return;
	}

	UTexture2D* Texture = ShortcutAsset->CachedThumbnail;
	if (Texture != nullptr)
	{
		FTextureResource* TextureResource = Texture->GetResource();
		if (TextureResource != nullptr)
		{
			FCanvasTileItem Tile(
				FVector2D(X, Y),
				Texture->GetResource(),
				FVector2D(Width, Height),
				FLinearColor::White
			);
			Canvas->DrawItem(Tile);
		}
	}

	DrawOverlay(Width, Height, Canvas);
}

void UShortcutAssetThumbnailRenderer::DrawOverlay(uint32 Width, uint32 Height, FCanvas* Canvas) const
{
	FVector2D Position(Width / 24, Height / 24);
	FCanvasTextItem TextItem(Position, FText::FromString("Shortcut"), FSlateFontInfo(UEngine::GetMediumFont(), 12), FLinearColor::White);

	// Dummy draw call text draw to get text size.
	TextItem.DisableShadow();
	TextItem.Draw(Canvas);

	// Draw background
	FVector2D Padding(4.0f, 2.0f);
	FVector2D BackgroundSize = TextItem.DrawnSize + Padding * 2.0f;
	FVector2D BackgroundPosition = Position - Padding;
	FCanvasTileItem BackgroundItem(BackgroundPosition, BackgroundSize, FLinearColor(0.0, 0.0, 0.0, 0.7));
	BackgroundItem.BlendMode = SE_BLEND_AlphaBlend;
	Canvas->DrawItem(BackgroundItem);

	// Draw text
	TextItem.DisableShadow();
	TextItem.Draw(Canvas);
}

bool UShortcutAssetThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	UShortcutAsset* ShortcutAsset = Cast<UShortcutAsset>(Object);
	if (ShortcutAsset == nullptr)
	{
		return false;
	}

	FAssetData LinkedAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ShortcutAsset->LinkedAsset);
	UObject* LinkedObject = LinkedAssetData.GetAsset();
	if (LinkedObject == nullptr)
	{
		return false;
	}

	return true;
}

#undef LOCTEXT_NAMESPACE