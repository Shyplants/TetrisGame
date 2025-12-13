#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "Common/Math/MathTypes.h"
#include "SimpleMath.h"
#include "TetrisTypes.h"
#include <array>

class TetrominoActor;
class SpriteMesh;
class GraphicsPSO;
class Texture;
class D3D11Renderer;

enum class ETetrominoRenderMode
{
	Board,  // 보드 좌표계에서 렌더링
	UI      // UI 패널에서 렌더링
};

class TetrominoRendererComponent : public RendererComponent
{
public:
	explicit TetrominoRendererComponent(Actor* owner);
	~TetrominoRendererComponent() override;

	void RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) override;

public:
	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetRenderMode(ETetrominoRenderMode mode) { m_renderMode = mode; }
	void SetTetromino(TetrominoType type, Rotation rot = Rotation::R0);

private:
	SpriteMesh* m_mesh{ nullptr };
	GraphicsPSO* m_pso{ nullptr };
	Texture* m_texture{ nullptr };

private:
	ETetrominoRenderMode m_renderMode{ ETetrominoRenderMode::Board };
	TetrominoType m_type{ TetrominoType::None };

	std::array<IVec2, MINO_COUNT> m_blocks{};
	DirectX::XMFLOAT2 m_uiOffset{0.0f, 0.0f}; // UI 중앙 정렬 맞추기 용도
};