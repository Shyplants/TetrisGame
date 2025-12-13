#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Math/MathTypes.h"
#include "TetrisConfig.h"
#include "TetrisTypes.h"
#include <array>

class TetrominoRendererComponent;

class TetrominoActor : public Actor
{
public:
	explicit TetrominoActor(TetrominoType type = TetrominoType::None);
	~TetrominoActor() override;

	void OnSpawned() override;

public:
	TetrominoType GetType() const { return m_type; }
	void SetType(TetrominoType type) { m_type = type; }

	Rotation GetRotation() const { return m_rotation; }
	void SetRotation(Rotation rot) { m_rotation = rot; }
	
	IVec2 GetPos() const;
	int32 GetX() const;
	int32 GetY() const;

	void Move(int32 dx, int32 dy);
	void SetPos(IVec2 pos);
	void SetPos(int32 x, int32 y);

	void Rotate(bool cw);

	// »ó´ë ÁÂÇ¥
	std::array<IVec2, MINO_COUNT> GetCurrentBlocks() const;
	std::array<IVec2, MINO_COUNT> GetRelativeRotatedBlocks(Rotation theta, bool cw = true) const;

	// Àý´ë ÁÂÇ¥
	std::array<IVec2, MINO_COUNT> GetCurrentWorldBlocks() const;
	std::array<IVec2, MINO_COUNT> GetRelativeRotatedWorldBlocks(Rotation theta, bool cw = true) const;

	void SetRenderOffset(const DirectX::XMFLOAT2& offset);
	void SetBoardWorldMatrix(const DirectX::XMMATRIX& mat);

	bool IsGhost() const { return m_isGhost; }
	void SetGhostMode(bool isGhost);

private:
	TetrominoType m_type{ TetrominoType::None };
	Rotation m_rotation{ Rotation::R0 };
	bool m_isGhost{ false };

private:
	TetrominoRendererComponent* m_renderer{ nullptr };
};