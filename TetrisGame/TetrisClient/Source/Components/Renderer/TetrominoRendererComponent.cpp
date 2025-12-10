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

    SceneComponent* sc = GetAttachComponent();
    if (!sc)
        return;

    // XMMATRIX actorWorld = sc->GetWorldMatrix();

    auto* tetro = static_cast<TetrominoActor*>(GetOwner());
    auto blocks = tetro->GetCurrentBlocks();
    Vector2 pos = tetro->GetPos();

    const float cellSize = 32.0f;


    auto type = tetro->GetType();
    Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    switch (type)
    {
        case TetrominoType::I: color = { 0.2f, 0.9f, 1.0f, 1.0f }; break;
        case TetrominoType::O: color = { 1.0f, 0.9f, 0.2f, 1.0f }; break;
        case TetrominoType::T: color = { 0.8f, 0.3f, 1.0f, 1.0f }; break;
        case TetrominoType::L: color = { 1.0f, 0.5f, 0.1f, 1.0f }; break;
        case TetrominoType::J: color = { 0.0f, 0.2f, 1.0f, 1.0f }; break;
        case TetrominoType::S: color = { 0.2f, 1.0f, 0.2f, 1.0f }; break;
        case TetrominoType::Z: color = { 1.0f, 0.2f, 0.2f, 1.0f }; break;
        default: break;
    }

    // 블록 4개 렌더링
    for (auto& b : blocks)
    {
        float worldX = (pos.x + b.x) * cellSize + m_renderOffset.x;
        float worldY = (pos.y + b.y) * cellSize + m_renderOffset.y;

        XMMATRIX T = XMMatrixTranslation(worldX, worldY, 0.0f);
        XMMATRIX S = XMMatrixScaling(cellSize, cellSize, 1.0f);

        XMMATRIX world = S * T;
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
        cb.Color = color;
        cb.TexCoord = { 0.0f, 0.0f };
        cb.TexSize = { 1.0f, 1.0f };

        dc.VSConstantBuffer = renderer.GetSpriteVSConstantBuffer();
        dc.PSTextureSRV = m_texture->GetSRV();
        dc.PSSampler = renderer.GetDefaultSamplerState();

        renderer.Submit(dc);
    }
}
