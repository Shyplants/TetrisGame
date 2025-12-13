#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Math/MathTypes.h"

#include <array>

#include "TetrisTypes.h"

class TetrominoRendererComponent;
class SpriteRendererComponent;

class PreviewPanelActor : public Actor
{
public:
	explicit PreviewPanelActor(IVec2 offset);
	~PreviewPanelActor() override;

	void OnSpawned() override;

public:
	IVec2 GetRenderOffset();

	void UpdatePreviewMinos(const std::array<TetrominoType, MINO_PREVIEW_COUNT>& previewMinoTypes);

private:
	std::array<TetrominoRendererComponent*, MINO_PREVIEW_COUNT> m_previewMinoRenderers{};
	SpriteRendererComponent* m_panelRenderer{ nullptr };

private:
	IVec2 m_offset{};
};