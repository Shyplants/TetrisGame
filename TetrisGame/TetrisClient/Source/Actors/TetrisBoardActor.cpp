#include "ClientPCH/ClientPCH.h"
#include "Actors/TetrisBoardActor.h"
#include "Actors/TetrominoActor.h"

using namespace DirectX::SimpleMath;

TetrisBoardActor::TetrisBoardActor(int32 width, int32 height)
	: Actor()
	, m_width(width)
	, m_height(height)
	, m_cells(static_cast<size_t>(width*height), TetrominoType::None)
{

}

TetrisBoardActor::~TetrisBoardActor()
{
}

void TetrisBoardActor::OnSpawned()
{
	// Test
	m_cells[0] = TetrominoType::I;
	m_cells[5] = TetrominoType::O;
	m_cells[9] = TetrominoType::T;

	m_cells[10] = TetrominoType::I;
	m_cells[15] = TetrominoType::O;
	m_cells[19] = TetrominoType::T;

	m_cells[220] = TetrominoType::I;
	// m_cells[225] = TetrominoType::O;
	m_cells[229] = TetrominoType::T;
}

void TetrisBoardActor::Tick(float deltaTime)
{
}

TetrominoType TetrisBoardActor::Get(int32 x, int32 y) const
{
	if (OOB(x, y))
		return TetrominoType::None;

	return m_cells[pos2Idx(x, y)];
}

bool TetrisBoardActor::IsCollide(const TetrominoActor& t, int32 dx, int32 dy, Rotation rot) const
{
	auto type = t.GetType();
	auto blocks = t.GetRotBlocks(rot);

	int32 baseX = t.GetX() + dx;
	int32 baseY = t.GetY() + dy;

	for (auto& block : blocks)
	{
		int32 x = baseX + static_cast<int32>(block.x);
		int32 y = baseY + static_cast<int32>(block.y);

		if (OOB(x, y) || Get(x, y) != TetrominoType::None)
			return true;
	}

	return false;
}

bool TetrisBoardActor::IsCollide(const std::array<Vector2, MINO_COUNT>& t) const
{
	for (auto& pos : t)
	{
		int32 x = static_cast<int32>(pos.x);
		int32 y = static_cast<int32>(pos.y);

		if (OOB(x, y) || Get(x, y) != TetrominoType::None)
			return true;
	}

	return false;
}

void TetrisBoardActor::Lock(const TetrominoActor& t)
{
	auto type = t.GetType();
	auto blocks = t.GetCurBlocks();

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
	if (OOB(x, y))
	{
		// TODO : Log
		return -1;
	}

	return y * m_width + x;
}
