#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "SimpleMath.h"
#include "TetrisTypes.h"

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
	void SetBoardWorldMatrix(const DirectX::XMMATRIX& mat) { m_boardWorld = mat; }

private:
	SpriteMesh* m_mesh{ nullptr };
	GraphicsPSO* m_pso{ nullptr };
	Texture* m_texture{ nullptr };

private:
	ETetrominoRenderMode m_renderMode{ ETetrominoRenderMode::Board };
	DirectX::XMMATRIX m_boardWorld {DirectX::XMMatrixIdentity()};
};