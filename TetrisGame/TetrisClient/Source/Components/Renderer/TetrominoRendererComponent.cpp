#include "ClientPCH/ClientPCH.h"
#include "Components/Renderer/TetrominoRendererComponent.h"

#include "Actors/TetrominoActor.h"

#include "Engine/Graphics/D3D11/D3D11Renderer.h"
#include "Engine/Graphics/Mesh/SpriteMesh.h"
#include "Engine/Graphics/PSO/GraphicsPSO.h"
#include "Engine/Graphics/Command/DrawCommand.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Core/World/Actor.h"

#include "TetrisRules.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TetrominoRendererComponent::TetrominoRendererComponent(Actor* owner)
	: RendererComponent(owner)
{
}

TetrominoRendererComponent::~TetrominoRendererComponent()
{
}

void TetrominoRendererComponent::RenderWorld(D3D11Renderer& renderer, const DirectX::XMMATRIX& viewProj)
{
    if (!m_visible || !m_texture)
        return;

    if (!m_mesh)
        m_mesh = renderer.GetSpriteMesh();

    if (!m_pso)
        m_pso = renderer.GetPSO(L"SpritePSO");


    if (m_renderMode == ETetrominoRenderMode::Board)
    {
        auto* tetro = static_cast<TetrominoActor*>(GetOwner());
        m_type = tetro->IsGhost() ? TetrominoType::Ghost : tetro->GetType();
        m_blocks = tetro->GetCurrentWorldBlocks();
    }

    if (m_type == TetrominoType::None)
        return;

    auto parentActor = GetOwner()->GetParentActor();
    auto parentWorld = parentActor->GetRootComponent()->GetWorldMatrix();
    
    float texW = static_cast<float>(m_texture->GetWidth());
    float texH = static_cast<float>(m_texture->GetHeight());

    float tileW = 32.0f;
    float tileH = 32.0f;
    float spacing = 1.0f;

    auto tileIndex = GetTileIndex(m_type);
    float u0 = (tileIndex * (tileW + spacing)) / texW;
    float v0 = 0.0f;
    float uSize = tileW / texW;
    float vSize = tileH / texH;

    const float cellSize = 32.0f;

    // 블록 4개 렌더링
    for (auto& block : m_blocks)
    {
        float worldX = block.x * cellSize + m_renderOffset.x;
        float worldY = block.y * cellSize + m_renderOffset.y;

        XMMATRIX T = XMMatrixTranslation(worldX, worldY, 0.0f);
        XMMATRIX S = XMMatrixScaling(cellSize, cellSize, 1.0f);

        XMMATRIX world = S * T * parentWorld;
        XMMATRIX wvp = world * viewProj;

        DrawCommand dc{};
        dc.PSOHashID = m_pso->GetHashID();
        dc.sortKey.PSOHashID = m_pso->GetHashID();
        dc.sortKey.renderLayer = GameLayer::TileMap;

        dc.primitiveType = m_pso->GetPrimitiveType();
        dc.vertexBuffer = m_mesh->GetVertexBuffer();
        dc.vertexStride = m_mesh->GetVertexStride();
        dc.vertexOffset = m_mesh->GetVertexOffset();
        dc.indexBuffer = m_mesh->GetIndexBuffer();
        dc.indexFormat = DXGI_FORMAT_R16_UINT;
        dc.numIndices = m_mesh->GetIndexCount();

        auto& cb = dc.spriteConstantBuffer;
        cb.WVP = XMMatrixTranspose(wvp);
        cb.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

        cb.TexCoord = { u0, v0 };
        cb.TexSize = { uSize, vSize };

        dc.VSConstantBuffer = renderer.GetSpriteVSConstantBuffer();
        dc.PSTextureSRV = m_texture->GetSRV();
        dc.PSSampler = renderer.GetDefaultSamplerState();

        renderer.Submit(dc);
    }
}

void TetrominoRendererComponent::SetTetromino(TetrominoType type, Rotation rot)
{
    m_type = type;

    if(IsMinoType(type))
        m_blocks = TetrisRules::GetShapeBlocks(type, rot);
}
