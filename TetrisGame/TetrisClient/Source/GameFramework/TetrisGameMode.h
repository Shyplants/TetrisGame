#pragma once

#include "Engine/Core/GameFramework/GameMode.h"
#include "TetrisBag.h"

class TetrisBoardActor;
class TetrominoActor;
class HoldPanelActor;
class SidePanelActor;
class PreviewPanelActor;

class TetrisGameMode : public GameMode
{
public:
	explicit TetrisGameMode(World* world);
	~TetrisGameMode() override;

	void OnBeginPlay() override;
	void Tick(float deltaTime) override;

public:
	void SetBoardActor(TetrisBoardActor* boardActor) { m_boardActor = boardActor; }
	void SetHoldPanelActor(HoldPanelActor* holdPanelActor) { m_holdPanelActor = holdPanelActor; }
	void SetSidePanelActor(SidePanelActor* sidePanelActor) { m_sidePanelActor = sidePanelActor; }
	void SetPreviewPanelActor(PreviewPanelActor* previewActor) { m_previewPanelActor = previewActor; }

	void SetRenderOffset(const DirectX::XMFLOAT2& offset);
	void StartGame();

private:
	void HandleInput(float deltaTime);

	void UpdateFalling(float deltaTime);
	void UpdateGhostMino();

	bool TryRotateMino(bool cw);
	bool TryHoldMino();
	bool TryMoveMino(int32 dx, int32 dy);

	void LockMinoAndProceed();

	void SpawnNextMino();
	void ResetCurrentMino();
	

private:
	TetrisBoardActor* m_boardActor{ nullptr };
	HoldPanelActor* m_holdPanelActor{ nullptr };
	SidePanelActor* m_sidePanelActor{ nullptr };
	PreviewPanelActor* m_previewPanelActor{ nullptr };

	TetrominoActor* m_currentMinoActor{ nullptr };
	TetrominoActor* m_ghostMinoActor{ nullptr };
	
	TetrominoType m_holdMinoType{ TetrominoType::None };

	TetrisBag m_bag;

	float m_fallTimer{ 0.0f };
	float m_fallInterval{ 0.5f };

	bool m_isGameOver{ false };
	bool m_bHasHeldThisTurn{ false };

	DirectX::XMFLOAT2 m_renderOffset = { 0.f, 0.f };
};