#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Math/MathTypes.h"

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

	void UpdateHoldMinoInfo(TetrominoType type, bool bHasHeldThisTurn);

private:
	SpriteRendererComponent* m_panelRenderer{ nullptr };

private:
	IVec2 m_offset{};
};