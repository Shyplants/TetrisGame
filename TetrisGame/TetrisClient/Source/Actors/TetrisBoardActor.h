#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Math/MathTypes.h"
#include <vector>
#include <array>

#include "TetrisConfig.h"
#include "TetrisTypes.h"

class TetrominoActor;
class BoardRendererComponent;
class SpriteRendererComponent;

class TetrisBoardActor : public Actor
{
public:
	explicit TetrisBoardActor(int32 width = BOARD_WIDTH, int32 height = BOARD_HEIGHT);
	~TetrisBoardActor() override;

	void OnSpawned() override;
	void Tick(float deltaTime) override;

public:
	int32 GetWidth() const { return m_width; }
	int32 GetHeight() const { return m_height; }

	// 셀 접근
	TetrominoType Get(int32 x, int32 y) const;

	// 미노를 이동 했을 때 충돌하는지 여부 반환
	bool WouldCollideAt(std::array<IVec2, MINO_COUNT> blocks, IVec2 offset) const;
	bool WouldCollideAt(const TetrominoActor& t, int32 dx, int32 dy) const;
	bool WouldCollideAt(TetrominoType type, Rotation rot, int32 baseX, int32 baseY) const;

	// 미노를 보드에 고정
	void Lock(const TetrominoActor& t);

	// 가득 찬 라인 제거 및 위에서 당기기. 제거한 라인 수 반환
	int32 ClearFullLines();

	void SetRenderOffset(const DirectX::XMFLOAT2& offset);

private:
	// bool IsCollide(const std::array<DirectX::SimpleMath::Vector2, MINO_COUNT>& blocks, int32 dx, int32 dy) const;
	bool WouldCollideAt(const std::array<IVec2, MINO_COUNT>& blocks) const;

	// Cell Management
	void Set(int32 x, int32 y, TetrominoType type);

	// Helper
	bool OOB(int32 x, int32 y) const { return x < 0 || x >= m_width || y < 0 || y >= m_height; }
	int32 pos2Idx(int32 x, int32 y) const;


private:
	BoardRendererComponent* m_cellRenderer{ nullptr };

	SpriteRendererComponent* m_boardRenderer{ nullptr };
	SpriteRendererComponent* m_sidePanelRenderer{ nullptr };
	SpriteRendererComponent* m_holdPanelRenderer{ nullptr };
	SpriteRendererComponent* m_previewPanelRenderer{ nullptr };

private:
	int32 m_width{ BOARD_WIDTH }, m_height{ BOARD_HEIGHT };
	std::vector<TetrominoType> m_cells;
};