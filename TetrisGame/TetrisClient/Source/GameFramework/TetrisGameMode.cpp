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
	UpdateGhostMino();
	UpdateFalling(deltaTime);
}

void TetrisGameMode::SetRenderOffset(const DirectX::XMFLOAT2& offset)
{
	m_renderOffset = offset;
	
	if (m_board)
		m_board->SetRenderOffset(offset);
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
		int32 d = 0;
		return;
	}
	
	TetrominoType next = m_bag.Pop();
	m_currentMino = GetWorld()->SpawnActor<TetrominoActor>(next);
	m_ghostMino = GetWorld()->SpawnActor<TetrominoActor>(next);
	
	int32 spawnX = m_board->GetWidth() / 2 - 1;
	int32 spawnY = m_board->GetHeight() - 2;

	m_currentMino->SetPos(spawnX, spawnY);
	m_currentMino->SetRenderOffset(m_renderOffset);
	m_currentMino->AttachToActor(m_board, FAttachmentTransformRules::KeepRelativeTransform);

	m_ghostMino->SetGhostMode(true);
	m_ghostMino->SetRenderOffset(m_renderOffset);
	m_ghostMino->AttachToActor(m_board, FAttachmentTransformRules::KeepRelativeTransform);

	if (m_board->WouldCollideAt(*m_currentMino, 0, 0))
		m_isGameOver = true;
	
}

void TetrisGameMode::HandleInput(float deltaTime)
{
	auto& input = InputManager::Get();

	// Move Left
	if (input.IsKeyPressed(KeyCode::Left))
	{
		if (TryMoveMino(-1, 0))
		{

		}
	}

	// Move Right
	if (input.IsKeyPressed(KeyCode::Right))
	{
		if (TryMoveMino(+1, 0))
		{

		}
	}

	// Soft Drop
	if (input.IsKeyPressed(KeyCode::Down))
	{
		if (TryMoveMino(0, -1))
		{
			m_fallTimer = 0.0f;
		}
		else
		{
			LockMinoAndProceed();
		}
	}

	// Rotate CW
	if (input.IsKeyPressed(KeyCode::Up))
	{
		if (TryRotateMino(true))
		{
			// TODO
		}
	}

	// Rotate CCW
	if (input.IsKeyPressed(KeyCode::Z))
	{
		if (TryRotateMino(false))
		{
			// TODO
		}
	}

	// Hard Drop
	if (input.IsKeyPressed(KeyCode::Space))
	{
		while (!m_board->WouldCollideAt(*m_currentMino, 0, -1))
			m_currentMino->Move(0, -1);

		LockMinoAndProceed();
	}

	// Hold
	if (input.IsKeyDown(KeyCode::C))
	{
		if (TryHoldMino())
		{
			m_fallTimer = 0.0f;
			UpdateGhostMino();
		}
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

		if (!TryMoveMino(0, -1))
		{
			LockMinoAndProceed();
		}
	}
}

void TetrisGameMode::UpdateGhostMino()
{
	m_ghostMino->SetType(m_currentMino->GetType());
	m_ghostMino->SetPos(m_currentMino->GetPos());
	m_ghostMino->SetRotation(m_currentMino->GetRotation());

	while (!m_board->WouldCollideAt(*m_ghostMino, 0, -1))
	{
		m_ghostMino->Move(0, -1);
	}
}

void TetrisGameMode::ResetCurrentMino()
{
	if (m_currentMino)
	{
		m_currentMino->Destroy();
		m_currentMino = nullptr;
	}

	if (m_ghostMino)
	{
		m_ghostMino->Destroy();
		m_ghostMino = nullptr;
	}
}

bool TetrisGameMode::TryRotateMino(bool cw)
{
	if (!m_currentMino)
		return false;

	bool ret = false;

	IVec2 shouldOffset{};
	auto curRotIdx = static_cast<size_t>(m_currentMino->GetRotation());
	auto nextRotIdx = static_cast<size_t>(NextRotation(m_currentMino->GetRotation(), Rotation::R90, cw));
	auto rotatedBlocks = m_currentMino->GetRelativeRotatedWorldBlocks(Rotation::R90, cw);

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
				auto offset = offsetData[curRotIdx][i] - offsetData[nextRotIdx][i];
				
				if (!m_board->WouldCollideAt(rotatedBlocks, offset))
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
				auto offset = offsetData[curRotIdx][i] - offsetData[nextRotIdx][i];

				if (!m_board->WouldCollideAt(rotatedBlocks, offset))
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
				auto offset = offsetData[curRotIdx][i] - offsetData[nextRotIdx][i];

				if (!m_board->WouldCollideAt(rotatedBlocks, offset))
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

bool TetrisGameMode::TryHoldMino()
{
	// 한턴에 한번씩만 홀드 가능
	if (m_bHasHeldThisTurn)
		return false;

	if (m_holdMinoType == TetrominoType::None)
	{
		// 처음 홀드
		m_holdMinoType = m_currentMino->GetType();
		
		ResetCurrentMino();
		SpawnNextMino();
	}
	else
	{
		// 홀드 미노와 스왑시 충돌 발생하는지 검사
		int32 spawnX = m_board->GetWidth() / 2 - 1;
		int32 spawnY = m_board->GetHeight() - 2;

		if (m_board->WouldCollideAt(m_holdMinoType, Rotation::R0, spawnX, spawnY))
		{
			return false;
		}

		auto oldMinoType = m_currentMino->GetType();

		// 현재 미노를 홀드 타입으로 교체 및 초기화
		m_currentMino->SetType(m_holdMinoType);
		m_currentMino->SetPos(spawnX, spawnY);
		m_currentMino->SetRotation(Rotation::R0);

		m_holdMinoType = oldMinoType;
	}

	m_bHasHeldThisTurn = true;
	return true;
}

bool TetrisGameMode::TryMoveMino(int32 dx, int32 dy)
{
	if (!m_currentMino)
	{
		__debugbreak();
		return false;
	}

	if (m_board->WouldCollideAt(*m_currentMino, dx, dy))
		return false;

	m_currentMino->Move(dx, dy);
	return true;
}

void TetrisGameMode::LockMinoAndProceed()
{
	if (!m_currentMino)
	{
		__debugbreak();
	}

	m_board->Lock(*m_currentMino);

	int32 cleared = m_board->ClearFullLines();
	if (cleared > 0)
	{
		GetGameState()->AddScore(cleared * 100);
	}

	m_bHasHeldThisTurn = false;
	ResetCurrentMino();
	SpawnNextMino();
}
