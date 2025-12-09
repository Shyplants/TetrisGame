#pragma once

#include "Engine/Core/Component/RendererComponent.h"
#include "SimpleMath.h"
#include "TetrisTypes.h"

class TetrominoActor;
class SpriteMesh;
class GraphicsPSO;
class Texture;
class D3D11Renderer;

class TetrominoRendererComponent : public RendererComponent
{
public:
	explicit TetrominoRendererComponent(Actor* owner);
	~TetrominoRendererComponent() override;

	void RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj) override;

public:
	void SetTexture(Texture* texture) { m_texture = texture; }

private:
	SpriteMesh* m_mesh{ nullptr };
	GraphicsPSO* m_pso{ nullptr };
	Texture* m_texture{ nullptr };
};