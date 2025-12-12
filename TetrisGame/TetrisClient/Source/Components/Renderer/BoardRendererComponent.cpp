#include "ClientPCH/ClientPCH.h"
#include "Components/Renderer/BoardRendererComponent.h"

#include "Actors/TetrisBoardActor.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Graphics/Mesh/SpriteMesh.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Command/DrawCommand.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

BoardRendererComponent::BoardRendererComponent(Actor* owner)
	: RendererComponent(owner)
{
}

BoardRendererComponent::~BoardRendererComponent()
{
}

void BoardRendererComponent::RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj)
{
	if (!m_visible || !m_board || !m_blockTexture)
		return;

	if (!m_mesh)
		m_mesh = renderer.GetSpriteMesh();

	if (!m_pso)
		m_pso = renderer.GetPSO(L"SpritePSO");

	SceneComponent* sc = GetAttachComponent();
	if (!sc)
		return;

	XMMATRIX boardWorld = sc->GetWorldMatrix();

	const int32 width = m_board->GetWidth();
	const int32 height = m_board->GetHeight();

	const float cellSize = 32.0f;

	for (int32 y = 0; y < height; ++y)
	{
		for(int32 x=0; x<width; ++x)
		{
			auto type = m_board->Get(x, y);
			if (type == TetrominoType::None)
				continue;

			auto tileIndex = GetTileIndex(type);

			float texW = static_cast<float>(m_blockTexture->GetWidth());
			float texH = static_cast<float>(m_blockTexture->GetHeight());

			float tileW = 32.0f;
			float tileH = 32.0f;
			float spacing = 1.0f;

			float u0 = (tileIndex * (tileW + spacing)) / texW;
			float v0 = 0.0f;

			float uSize = tileW / texW;
			float vSize = tileH / texH;

			// 셀 위치를 월드 좌표로 변환
			float worldX = static_cast<float>(x) * cellSize + m_renderOffset.x;
			float worldY = static_cast<float>(y) * cellSize + m_renderOffset.y;

			XMMATRIX T = XMMatrixTranslation(worldX, worldY, 0.0f);
			XMMATRIX S = XMMatrixScaling(cellSize, cellSize, 1.0f);

			XMMATRIX world = S * T * boardWorld;
			XMMATRIX wvp = world * viewProj;

			DrawCommand dc = {};

			// PSO 설정
			dc.PSOHashID = m_pso->GetHashID();
			dc.sortKey.PSOHashID = m_pso->GetHashID();

			dc.sortKey.renderLayer = GameLayer::TileMap;
			dc.sortKey.contentLayer = y;

			// 버퍼 정보
			dc.primitiveType = m_pso->GetPrimitiveType();
			dc.vertexBuffer = m_mesh->GetVertexBuffer();
			dc.vertexStride = m_mesh->GetVertexStride();
			dc.vertexOffset = m_mesh->GetVertexOffset();
			dc.indexBuffer = m_mesh->GetIndexBuffer();
			dc.indexFormat = DXGI_FORMAT_R16_UINT;
			dc.numIndices = m_mesh->GetIndexCount();

			// ConstantBuffer
			auto& cb = dc.spriteConstantBuffer;
			cb.WVP = XMMatrixTranspose(wvp);
			cb.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

			cb.TexCoord = { u0, v0 };
			cb.TexSize = { uSize, vSize };

			dc.VSConstantBuffer = renderer.GetSpriteVSConstantBuffer();
			dc.PSTextureSRV = m_blockTexture->GetSRV();
			dc.PSSampler = renderer.GetDefaultSamplerState();

			// Submit Command
			renderer.Submit(dc);
		}
	}
}
