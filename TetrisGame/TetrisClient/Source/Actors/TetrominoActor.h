#pragma once

#include "Engine/Core/World/Actor.h"
#include "TetrisConfig.h"
#include "TetrisTypes.h"
#include "SimpleMath.h"
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

	Rotation GetRotation() const { return m_rotation; }
	void SetRotation(Rotation rot) { m_rotation = rot; }
	
	DirectX::SimpleMath::Vector2 GetPos() const;
	int32 GetX() const;
	int32 GetY() const;

	void Move(int32 dx, int32 dy);
	void SetPos(DirectX::SimpleMath::Vector2 pos);
	void SetPos(int32 x, int32 y);

	void Rotate(bool cw);

	// »ó´ë ÁÂÇ¥
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetCurrentBlocks() const;
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetRelativeRotatedBlocks(Rotation theta, bool cw = true) const;

	// Àý´ë ÁÂÇ¥
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetCurrentWorldBlocks() const;
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetRelativeRotatedWorldBlocks(Rotation theta, bool cw = true) const;

	void SetRenderOffset(const DirectX::XMFLOAT2& offset);

	bool IsGhost() const { return m_isGhost; }
	void SetGhostMode(bool isGhost);

private:
	TetrominoType m_type{ TetrominoType::None };
	Rotation m_rotation{ Rotation::R0 };
	bool m_isGhost{ false };

private:
	TetrominoRendererComponent* m_renderer{ nullptr };
};