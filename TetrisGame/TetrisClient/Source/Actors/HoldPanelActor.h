#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Math/MathTypes.h"

#include "TetrisTypes.h"

class TetrominoRendererComponent;
class SpriteRendererComponent;

class HoldPanelActor : public Actor
{
public:
	explicit HoldPanelActor(IVec2 offset);
	~HoldPanelActor() override;

	void OnSpawned() override;

public:
	IVec2 GetRenderOffset();
	
	void UpdateHoldMino(TetrominoType type, bool bHasHeldThisTurn);

private:
	TetrominoRendererComponent* m_holdMinoRenderer{ nullptr };
	SpriteRendererComponent* m_panelRenderer{ nullptr };

private:
	TetrominoType m_type{ TetrominoType::None };
	IVec2 m_offset{};
};