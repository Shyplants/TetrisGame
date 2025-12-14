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

PreviewPanelActor::~PreviewPanelActor() = default;

void PreviewPanelActor::OnSpawned()
{
	auto panelTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Tetris/PreviewPanel.png");
	SP_ASSERT(panelTexture != nullptr);

	auto panelWidth = static_cast<float>(panelTexture->GetWidth());
	auto panelHeight = static_cast<float>(panelTexture->GetHeight());

	float offsetX = static_cast<float>(m_offset.x);
	float offsetY = static_cast<float>(m_offset.y);

	m_panelRenderer = AddComponent<SpriteRendererComponent>();
	SP_ASSERT(m_panelRenderer != nullptr);

	m_panelRenderer->SetTexture(panelTexture.get());
	m_panelRenderer->SetPivot(SpritePivot::TopLeft);
	m_panelRenderer->SetRenderOffset({ offsetX , offsetY });

	constexpr float TOP_MARGIN_RATIO = 0.0496f;       // 패널 상단 여백
	constexpr float ITEM_SPACING_RATIO = 0.0307f;     // 미노 간 비율 기반 추가 간격
	constexpr float PREVIEW_ITEM_HEIGHT = CELL_SIZE;  // 셀 한칸 높이

	float topMargin = panelHeight * TOP_MARGIN_RATIO;
	float extraSpacing = panelHeight * ITEM_SPACING_RATIO;
	float firstItemSpacing = extraSpacing + PREVIEW_ITEM_HEIGHT;

	// 첫 미노의 시작 위치 보정
	offsetX += panelWidth * 0.5f;
	offsetY -= (topMargin + firstItemSpacing);

	// 각 미노 사이 간격
	float previewSpacing = (CELL_SIZE * 2.0f) + (extraSpacing * 2.0f);

	// 미리보기 미노 렌더러(5개)
	auto tileTexture = ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png");
	SP_ASSERT(tileTexture);
	for (int32 i = 0; i < MINO_PREVIEW_COUNT; ++i)
	{
		m_previewMinoRenderers[i] = AddComponent<TetrominoRendererComponent>();
		SP_ASSERT(m_previewMinoRenderers[i] != nullptr);

		m_previewMinoRenderers[i]->SetTexture(tileTexture.get());
		m_previewMinoRenderers[i]->SetRenderMode(ETetrominoRenderMode::UI);
		m_previewMinoRenderers[i]->SetTetromino(TetrominoType::None);
		m_previewMinoRenderers[i]->SetRenderOffset({ offsetX , offsetY });

		offsetY -= previewSpacing;
	}
}

IVec2 PreviewPanelActor::GetRenderOffset() const
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
