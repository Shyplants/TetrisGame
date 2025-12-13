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