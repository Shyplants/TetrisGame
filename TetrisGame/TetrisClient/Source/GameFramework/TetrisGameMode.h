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
	void SpawnNextMino();
	void HandleInput(float deltaTime);
	void UpdateFalling(float deltaTime);
	void TryLockMino();
	void ResetCurrentMino();
	bool TryRotate(bool cw);


private:
	TetrisBoardActor* m_board{ nullptr };
	TetrominoActor* m_currentMino{ nullptr };

	TetrisBag m_bag;

	float m_fallTimer{ 0.0f };
	float m_fallInterval{ 0.5f };

	bool m_isGameOver{ false };

	DirectX::XMFLOAT2 m_renderOffset = { 0.f, 0.f };
};