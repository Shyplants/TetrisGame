#include "ClientPCH/ClientPCH.h"
#include "GameFramework/TetrisGameMode.h"

#include "SimpleMath.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/GameFramework/GameState.h"

#include "Actors/TetrisBoardActor.h"
#include "Actors/HoldPanelActor.h"
#include "Actors/SidePanelActor.h"
#include "Actors/PreviewPanelActor.h"

#include "Actors/TetrominoActor.h"

#include "Engine/System/InputManager.h"

#include "TetrisRules.h"
#include "TetrisHelpers.h"

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
	
	if (m_boardActor)
		m_boardActor->SetRenderOffset(offset);
}

void TetrisGameMode::StartGame()
{
	// 첫 미노 스폰
	SpawnNextMino();
}

void TetrisGameMode::SpawnNextMino()
{
	// 현재 미노는 없는 상태여야 함
	if (m_currentMinoActor)
	{
		// TODO : Log
		int32 d = 0;
		return;
	}
	
	TetrominoType next = m_bag.Pop();
	m_currentMinoActor = GetWorld()->SpawnActor<TetrominoActor>(next);
	m_ghostMinoActor = GetWorld()->SpawnActor<TetrominoActor>(next);
	
	int32 spawnX = m_boardActor->GetWidth() / 2 - 1;
	int32 spawnY = m_boardActor->GetHeight() - 2;

	m_currentMinoActor->SetPos(spawnX, spawnY);
	m_currentMinoActor->SetRenderOffset(m_renderOffset);
	m_currentMinoActor->AttachToActor(m_boardActor, FAttachmentTransformRules::KeepRelativeTransform);

	m_ghostMinoActor->SetGhostMode(true);
	m_ghostMinoActor->SetRenderOffset(m_renderOffset);
	m_ghostMinoActor->AttachToActor(m_boardActor, FAttachmentTransformRules::KeepRelativeTransform);

	if (m_boardActor->WouldCollideAt(*m_currentMinoActor, 0, 0))
		m_isGameOver = true;
	
	// 미리보기 업데이트
	std::array<TetrominoType, MINO_PREVIEW_COUNT> previewMinoTypes;
	for (int32 i = 0; i < MINO_PREVIEW_COUNT; ++i)
		previewMinoTypes[i] = m_bag.Peek(i);
	
	if (m_previewPanelActor)
		m_previewPanelActor->UpdatePreviewMinos(previewMinoTypes);
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
		while (!m_boardActor->WouldCollideAt(*m_currentMinoActor, 0, -1))
			m_currentMinoActor->Move(0, -1);

		LockMinoAndProceed();
	}

	// Hold
	if (input.IsKeyDown(KeyCode::C))
	{
		if (TryHoldMino())
		{
			m_fallTimer = 0.0f;
			UpdateGhostMino();
			m_holdPanelActor->UpdateHoldMino(m_holdMinoType, true);
		}
	}


}

void TetrisGameMode::UpdateFalling(float deltaTime)
{
	if (!m_currentMinoActor)
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
	m_ghostMinoActor->SetType(m_currentMinoActor->GetType());
	m_ghostMinoActor->SetPos(m_currentMinoActor->GetPos());
	m_ghostMinoActor->SetRotation(m_currentMinoActor->GetRotation());

	while (!m_boardActor->WouldCollideAt(*m_ghostMinoActor, 0, -1))
	{
		m_ghostMinoActor->Move(0, -1);
	}
}

void TetrisGameMode::ResetCurrentMino()
{
	if (m_currentMinoActor)
	{
		m_currentMinoActor->Destroy();
		m_currentMinoActor = nullptr;
	}

	if (m_ghostMinoActor)
	{
		m_ghostMinoActor->Destroy();
		m_ghostMinoActor = nullptr;
	}
}

bool TetrisGameMode::TryRotateMino(bool cw)
{
	if (!m_currentMinoActor)
		return false;

	bool ret = false;

	IVec2 shouldOffset{};
	auto curRotIdx = static_cast<size_t>(m_currentMinoActor->GetRotation());
	auto nextRotIdx = static_cast<size_t>(NextRotation(m_currentMinoActor->GetRotation(), Rotation::R90, cw));
	auto rotatedBlocks = m_currentMinoActor->GetRelativeRotatedWorldBlocks(Rotation::R90, cw);

	switch (m_currentMinoActor->GetType())
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
				
				if (!m_boardActor->WouldCollideAt(rotatedBlocks, offset))
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

				if (!m_boardActor->WouldCollideAt(rotatedBlocks, offset))
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

				if (!m_boardActor->WouldCollideAt(rotatedBlocks, offset))
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
		m_currentMinoActor->SetPos(m_currentMinoActor->GetPos() + shouldOffset);
		m_currentMinoActor->Rotate(cw);
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
		m_holdMinoType = m_currentMinoActor->GetType();
		
		ResetCurrentMino();
		SpawnNextMino();
	}
	else
	{
		// 홀드 미노와 스왑시 충돌 발생하는지 검사
		int32 spawnX = m_boardActor->GetWidth() / 2 - 1;
		int32 spawnY = m_boardActor->GetHeight() - 2;

		if (m_boardActor->WouldCollideAt(m_holdMinoType, Rotation::R0, spawnX, spawnY))
		{
			return false;
		}

		auto oldMinoType = m_currentMinoActor->GetType();

		// 현재 미노를 홀드 타입으로 교체 및 초기화
		m_currentMinoActor->SetType(m_holdMinoType);
		m_currentMinoActor->SetPos(spawnX, spawnY);
		m_currentMinoActor->SetRotation(Rotation::R0);

		m_holdMinoType = oldMinoType;
	}

	m_bHasHeldThisTurn = true;
	return true;
}

bool TetrisGameMode::TryMoveMino(int32 dx, int32 dy)
{
	if (!m_currentMinoActor)
	{
		__debugbreak();
		return false;
	}

	if (m_boardActor->WouldCollideAt(*m_currentMinoActor, dx, dy))
		return false;

	m_currentMinoActor->Move(dx, dy);
	return true;
}

void TetrisGameMode::LockMinoAndProceed()
{
	if (!m_currentMinoActor)
	{
		__debugbreak();
	}

	m_boardActor->Lock(*m_currentMinoActor);

	int32 cleared = m_boardActor->ClearFullLines();
	if (cleared > 0)
	{
		GetGameState()->AddScore(cleared * 100);
	}

	m_bHasHeldThisTurn = false;
	m_holdPanelActor->UpdateHoldMino(m_holdMinoType, false);
	ResetCurrentMino();
	SpawnNextMino();
}
