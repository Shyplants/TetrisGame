#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "SimpleMath.h"
#include "TetrisTypes.h"
#include "TetrisConfig.h"

class TetrisBoardActor;
class Texture;
class SpriteMesh;
class GraphicsPSO;
class D3D11Renderer;

class BoardRendererComponent : public RendererComponent
{
public:
	explicit BoardRendererComponent(Actor* owner);
	~BoardRendererComponent() override;

	void RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) override;

public:
	void SetBoard(TetrisBoardActor* board) { m_board = board; }
	void SetBlockTexture(Texture* texture) { m_blockTexture = texture; }

private:
	TetrisBoardActor* m_board{ nullptr };

	SpriteMesh* m_mesh{ nullptr };
	GraphicsPSO* m_pso{ nullptr };
	Texture* m_blockTexture{ nullptr };
};