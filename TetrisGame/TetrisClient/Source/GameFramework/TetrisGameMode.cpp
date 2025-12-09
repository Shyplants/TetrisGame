#include "ClientPCH/ClientPCH.h"
#include "GameFramework/TetrisGameMode.h"

#include "SimpleMath.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/GameFramework/GameState.h"

#include "Actors/TetrisBoardActor.h"
#include "Actors/TetrominoActor.h"

#include "Engine/System/InputManager.h"

#include "TetrisRules.h"

using namespace DirectX::SimpleMath;

TetrisGameMode::TetrisGameMode(World* world)
	: GameMode(world)
{
}

TetrisGameMode::~TetrisGameMode()
{
}

void TetrisGameMode::OnBeginPlay()
{
	GameMode::OnBeginPlay();
	StartGame();
}

void TetrisGameMode::Tick(float deltaTime)
{
	if (m_isGameOver)
		return;

	GameMode::Tick(deltaTime);

	HandleInput(deltaTime);
	UpdateFalling(deltaTime);
}

void TetrisGameMode::StartGame()
{
	// 첫 미노 스폰
	SpawnNextMino();
}

void TetrisGameMode::SpawnNextMino()
{
	// 현재 미노는 없는 상태여야 함
	if (m_currentMino)
	{
		// TODO : Log
		return;
	}
	
	TetrominoType next = m_bag.Pop();
	m_currentMino = GetWorld()->SpawnActor<TetrominoActor>(next);
	m_currentMino->AttachToActor(m_board);

	int32 spawnX = m_board->GetWidth() / 2 - 1;
	int32 spawnY = m_board->GetHeight() - 2;

	m_currentMino->SetPos(spawnX, spawnY);

	if (m_board->IsCollide(*m_currentMino))
		m_isGameOver = true;
	
}

void TetrisGameMode::HandleInput(float deltaTime)
{
	auto& input = InputManager::Get();

	// Move Left
	if (input.IsKeyPressed(KeyCode::Left))
	{
		if (!m_board->IsCollide(*m_currentMino, -1, 0))
		{
			m_currentMino->Move(-1, 0);
		}
	}

	// Move Right
	if (input.IsKeyPressed(KeyCode::Right))
	{
		if (!m_board->IsCollide(*m_currentMino, +1, 0))
		{
			m_currentMino->Move(+1, 0);
		}
	}

	// Soft Drop
	if (input.IsKeyPressed(KeyCode::Down))
	{
		if (!m_board->IsCollide(*m_currentMino, 0, -1))
		{
			m_currentMino->Move(0, -1);
			m_fallTimer = 0.0f;
		}
		else
		{
			TryLockMino();
		}
	}

	// Rotate CW
	if (input.IsKeyPressed(KeyCode::Up))
	{
		if (TryRotate(true))
		{
			// TODO
		}
	}

	// Rotate CCW
	if (input.IsKeyPressed(KeyCode::Z))
	{
		if (TryRotate(false))
		{
			// TODO
		}
	}

	// Hard Drop
	if (input.IsKeyPressed(KeyCode::Space))
	{
		while (!m_board->IsCollide(*m_currentMino, 0, -1))
			m_currentMino->Move(0, -1);

		TryLockMino();
	}


}

void TetrisGameMode::UpdateFalling(float deltaTime)
{
	if (!m_currentMino)
		return;

	m_fallTimer += deltaTime;

	if (m_fallTimer >= m_fallInterval)
	{
		m_fallTimer = 0.0f;

		if (!m_board->IsCollide(*m_currentMino, 0, -1))
		{
			m_currentMino->Move(0, -1);
		}
		else
		{
			TryLockMino();
		}
	}
}

void TetrisGameMode::TryLockMino()
{
	if (!m_currentMino)
		return;

	m_board->Lock(*m_currentMino);

	int32 cleared = m_board->ClearFullLines();
	if (cleared > 0)
	{
		GetGameState()->AddScore(cleared * 100);
	}

	ResetCurrentMino();
}

void TetrisGameMode::ResetCurrentMino()
{
	if (m_currentMino)
	{
		m_currentMino->MarkForDestroy();
		m_currentMino = nullptr;
	}

	SpawnNextMino();
}

bool TetrisGameMode::TryRotate(bool cw)
{
	if (!m_currentMino)
		return false;

	bool ret = false;

	Vector2 shouldOffset{};
	auto curRot = m_currentMino->GetRotation();
	auto nextRot = cw ? NextCW(curRot) : NextCCW(curRot);
	auto rotatedBlock = m_currentMino->GetRotBlocks(nextRot);
	for (auto& block : rotatedBlock)
		block = block + m_currentMino->GetPos();

	switch (m_currentMino->GetType())
	{
		case TetrominoType::J:
		case TetrominoType::L:
		case TetrominoType::S:
		case TetrominoType::T:
		case TetrominoType::Z:
		{
			auto offsetData = TetrisRules::JLSTZ_Offset_Data;

			for (int32 i = 0; i < JLSTZ_OFFSET_COUNT; ++i)
			{
				auto offset = offsetData[static_cast<size_t>(curRot)][i] - offsetData[static_cast<size_t>(nextRot)][i];
				auto testBlocks = rotatedBlock;
				
				for (int32 j = 0; j < MINO_COUNT; ++j)
					testBlocks[j] += offset;

				if (!m_board->IsCollide(testBlocks))
				{
					ret = true;
					shouldOffset = offset;
					break;
				}
			}

			break;
		}
		case TetrominoType::I:
		{
			auto offsetData = TetrisRules::I_Offset_Data;

			for (int32 i = 0; i < I_OFFSET_COUNT; ++i)
			{
				auto offset = offsetData[static_cast<size_t>(curRot)][i] - offsetData[static_cast<size_t>(nextRot)][i];
				auto testBlocks = rotatedBlock;

				for (int32 j = 0; j < MINO_COUNT; ++j)
					testBlocks[j] += offset;

				if (!m_board->IsCollide(testBlocks))
				{
					ret = true;
					shouldOffset = offset;
					break;
				}
			}

			break;
		}

		case TetrominoType::O:
		{
			auto offsetData = TetrisRules::O_Offset_Data;

			for (int32 i = 0; i < O_OFFSET_COUNT; ++i)
			{
				auto offset = offsetData[static_cast<size_t>(curRot)][i] - offsetData[static_cast<size_t>(nextRot)][i];
				auto testBlocks = rotatedBlock;

				for (int32 j = 0; j < MINO_COUNT; ++j)
					testBlocks[j] += offset;

				if (!m_board->IsCollide(testBlocks))
				{
					ret = true;
					shouldOffset = offset;
					break;
				}
			}

			break;
		}

		default:
			return false;
	}

	if (ret)
	{
		m_currentMino->SetPos(m_currentMino->GetPos() + shouldOffset);
		m_currentMino->Rotate(cw);
	}

	return ret;
}