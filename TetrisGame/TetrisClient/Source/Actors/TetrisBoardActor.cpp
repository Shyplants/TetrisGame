#include "ClientPCH/ClientPCH.h"
#include "Actors/TetrisBoardActor.h"
#include "Actors/TetrominoActor.h"

#include "Components/Renderer/BoardRendererComponent.h"
#include "Components/Renderer/TetrominoRendererComponent.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

#include "TetrisRules.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TetrisBoardActor::TetrisBoardActor(int32 width, int32 height)
	: Actor()
	, m_width(width)
	, m_height(height)
	, m_cells(static_cast<size_t>(width*height), TetrominoType::None)
{

}

TetrisBoardActor::~TetrisBoardActor() = default;

void TetrisBoardActor::OnSpawned()
{
	m_cellRenderer = AddComponent<BoardRendererComponent>();
	SP_ASSERT(m_cellRenderer != nullptr);

	m_cellRenderer->SetBoard(this);

	auto tileTexture = ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png");
	SP_ASSERT(tileTexture != nullptr);

	m_cellRenderer->SetBlockTexture(tileTexture.get());


	auto boardTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Tetris/Board.png");
	SP_ASSERT(boardTexture != nullptr);

	m_panelWidth = boardTexture->GetWidth();
	m_panelHeight = boardTexture->GetHeight();

	// 보드 본체
	m_boardRenderer = AddComponent<SpriteRendererComponent>();
	m_boardRenderer->SetTexture(boardTexture.get());
	m_boardRenderer->SetPivot(SpritePivot::Center);

	// Test
	/*m_cells[0] = TetrominoType::I;
	m_cells[220] = TetrominoType::I;
	m_cells[225] = TetrominoType::O;
	m_cells[229] = TetrominoType::T;*/
}

void TetrisBoardActor::Tick(float deltaTime)
{
}

IVec2 TetrisBoardActor::GetRenderOffset()
{
	if (m_boardRenderer)
	{
		auto renderOffset = m_boardRenderer->GetRenderOffset();

		return { static_cast<int32>(renderOffset.x), static_cast<int32>(renderOffset.y) };
	}

	return IVec2{};
}

TetrominoType TetrisBoardActor::Get(int32 x, int32 y) const
{
	if (OOB(x, y))
		return TetrominoType::None;

	return m_cells[pos2Idx(x, y)];
}

bool TetrisBoardActor::WouldCollideAt(std::array<IVec2, MINO_COUNT> blocks, IVec2 offset) const
{
	auto movedBlocks = blocks;
	for (auto& block : movedBlocks)
	{
		block += offset;
	}

	return WouldCollideAt(movedBlocks);
}

bool TetrisBoardActor::WouldCollideAt(const TetrominoActor& t, int32 dx, int32 dy) const
{
	return WouldCollideAt(t.GetType(), t.GetRotation(), t.GetX() + dx, t.GetY() + dy);
}

bool TetrisBoardActor::WouldCollideAt(TetrominoType type, Rotation rot, int32 baseX, int32 baseY) const
{
	auto blocks = TetrisRules::GetShapeBlocks(type, rot);

	for (auto& block : blocks)
	{
		block.x += baseX;
		block.y += baseY;
	}

	return WouldCollideAt(blocks);
}

bool TetrisBoardActor::WouldCollideAt(const std::array<IVec2, MINO_COUNT>& blocks) const
{
	for (const auto& block : blocks)
	{
		// 보드 밖이면 충돌
		if (OOB(block.x, block.y))
			return true;

		// 이미 칸이 차있으면 충돌
		if (Get(block.x, block.y) != TetrominoType::None)
			return true;
	}

	return false;
}

void TetrisBoardActor::Lock(const TetrominoActor& t)
{
	auto type = t.GetType();
	auto blocks = t.GetCurrentBlocks();

	for (auto& block : blocks)
	{
		int32 x = t.GetX() + static_cast<int32>(block.x);
		int32 y = t.GetY() + static_cast<int32>(block.y);

		Set(x, y, type);
	}
}

int32 TetrisBoardActor::ClearFullLines()
{
	int32 cleared = 0;
	for (int32 y = 0; y < m_height-1;)
	{
		bool full = true;
		for (int32 x = 0; x < m_width; ++x)
		{
			if (Get(x, y) == TetrominoType::None)
			{
				full = false;
				break;
			}
		}

		if (full)
		{
			cleared++;

			// 위에서 한줄 씩 내리기
			for (int32 yy = y; yy < m_height-1; ++yy)
			{
				for (int32 x = 0; x < m_width; ++x)
					Set(x, yy, Get(x, yy + 1));
			}
		}
		else
			++y;
	}

	if (cleared > 0)
	{
		// 맨 위는 비우기
		for (int32 x = 0; x < m_width; ++x)
			Set(x, m_height-1, TetrominoType::None);
	}

	return cleared;
}

void TetrisBoardActor::Set(int32 x, int32 y, TetrominoType type)
{
	if (OOB(x, y))
	{
		// TODO : Log
		return;
	}

	m_cells[pos2Idx(x, y)] = type;
}

int32 TetrisBoardActor::pos2Idx(int32 x, int32 y) const
{
	SP_ASSERT(!OOB(x, y));

	return y * m_width + x;
}

void TetrisBoardActor::SetRenderOffset(const XMFLOAT2& offset)
{
	if (m_cellRenderer)
	{
		m_cellRenderer->SetRenderOffset(offset);
	}
}
