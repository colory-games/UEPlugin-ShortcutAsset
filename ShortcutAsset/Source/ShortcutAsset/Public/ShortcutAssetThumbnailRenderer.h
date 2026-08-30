/*!
 * ShortcutAsset
 *
 * Copyright (c) 2026 Colory Games
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include "ThumbnailRendering/ThumbnailRenderer.h"

#include "ShortcutAssetThumbnailRenderer.generated.h"

UCLASS()
class UShortcutAssetThumbnailRenderer : public UThumbnailRenderer
{
	GENERATED_UCLASS_BODY()

	virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	void DrawOverlay(uint32 Width, uint32 Height, FCanvas* Canvas) const;
	virtual bool CanVisualizeAsset(UObject* Object) override;
};
