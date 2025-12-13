#pragma once

#include "Engine/Core/GameFramework/GameMode.h"
#include "TetrisBag.h"

class TetrisBoardActor;
class TetrominoActor;

class TetrisGameMode : public GameMode
{
public:
	explicit TetrisGameMode(World* world);
	~TetrisGameMode() override;

	void OnBeginPlay() override;
	void Tick(float deltaTime) override;

public:
	void SetBoard(TetrisBoardActor* board) { m_board = board; }
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
	TetrisBoardActor* m_board{ nullptr };
	TetrominoActor* m_currentMino{ nullptr };
	TetrominoActor* m_ghostMino{ nullptr };
	
	TetrominoType m_holdMinoType{ TetrominoType::None };

	TetrisBag m_bag;

	float m_fallTimer{ 0.0f };
	float m_fallInterval{ 0.5f };

	bool m_isGameOver{ false };
	bool m_bHasHeldThisTurn{ false };

	DirectX::XMFLOAT2 m_renderOffset = { 0.f, 0.f };
};