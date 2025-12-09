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
	
	DirectX::SimpleMath::Vector2 GetPos() const;
	int32 GetX() const;
	int32 GetY() const;

	void Move(int32 dx, int32 dy);
	void SetPos(DirectX::SimpleMath::Vector2 pos);
	void SetPos(int32 x, int32 y);

	void Rotate(bool cw);

	// 현재 회전 상태의 4블록 상대 좌표 반환
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetCurBlocks() const;

	// 회전한 상태의 4블록 상대 좌표 반환
	std::array<DirectX::SimpleMath::Vector2, MINO_COUNT> GetRotBlocks(Rotation rot) const;


private:
	TetrominoType m_type{ TetrominoType::None };
	Rotation m_rotation{ Rotation::R0 };

private:
	TetrominoRendererComponent* m_renderer{ nullptr };
};