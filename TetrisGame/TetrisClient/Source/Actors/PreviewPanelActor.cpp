#include "ClientPCH/ClientPCH.h"
#include "Actors/PreviewPanelActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Components/Renderer/TetrominoRendererComponent.h"
#include "TetrisRules.h"

PreviewPanelActor::PreviewPanelActor(IVec2 offset)
	: Actor()
	, m_offset(offset)
{
}

PreviewPanelActor::~PreviewPanelActor()
{
}

void PreviewPanelActor::OnSpawned()
{
	auto panelTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Tetris/PreviewPanel.png").get();

	float offsetX = static_cast<float>(m_offset.x);
	float offsetY = static_cast<float>(m_offset.y);

	m_panelRenderer = AddComponent<SpriteRendererComponent>();
	m_panelRenderer->SetTexture(panelTexture);
	m_panelRenderer->SetPivot(SpritePivot::TopLeft);
	m_panelRenderer->SetRenderOffset({ offsetX , offsetY });

	constexpr float PREVIEW_SPACING = CELL_SIZE * 3.0f;
	offsetX += static_cast<float>(panelTexture->GetWidth()) / 2.0f;

	// 미리보기 미노 렌더러(5개)
	for (int32 i = 0; i < MINO_PREVIEW_COUNT; ++i)
	{
		offsetY -= PREVIEW_SPACING;

		m_previewMinoRenderers[i] = AddComponent<TetrominoRendererComponent>();
		m_previewMinoRenderers[i]->SetTexture(ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png").get());
		m_previewMinoRenderers[i]->SetRenderMode(ETetrominoRenderMode::UI);
		m_previewMinoRenderers[i]->SetTetromino(TetrominoType::None);
		m_previewMinoRenderers[i]->SetRenderOffset({ offsetX , offsetY });
	}
}

IVec2 PreviewPanelActor::GetRenderOffset()
{
	if (m_panelRenderer)
	{
		auto renderOffset = m_panelRenderer->GetRenderOffset();

		return { static_cast<int32>(renderOffset.x), static_cast<int32>(renderOffset.y) };
	}

	return IVec2{};
}

void PreviewPanelActor::UpdatePreviewMinos(const std::array<TetrominoType, MINO_PREVIEW_COUNT>& previewMinoTypes)
{
	for (int32 i = 0; i < MINO_PREVIEW_COUNT; ++i)
	{
		auto type = previewMinoTypes[i];

		m_previewMinoRenderers[i]->SetTetromino(type);
	}
}
