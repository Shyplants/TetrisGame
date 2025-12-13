#include "ClientPCH/ClientPCH.h"
#include "Actors/SidePanelActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

SidePanelActor::SidePanelActor(IVec2 offset)
	: Actor()
	, m_offset(offset)
{
}

SidePanelActor::~SidePanelActor()
{
}

void SidePanelActor::OnSpawned()
{
	auto panelTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Tetris/SidePanel.png").get();

	m_panelWidth = panelTexture->GetWidth();
	m_panelHeight = panelTexture->GetHeight();

	float offsetX = static_cast<float>(m_offset.x);
	float offsetY = static_cast<float>(m_offset.y);

	m_panelRenderer = AddComponent<SpriteRendererComponent>();
	m_panelRenderer->SetTexture(panelTexture);
	m_panelRenderer->SetPivot(SpritePivot::MiddleLeft);
	m_panelRenderer->SetRenderOffset({ offsetX , offsetY });
}

IVec2 SidePanelActor::GetRenderOffset()
{
	if (m_panelRenderer)
	{
		auto renderOffset = m_panelRenderer->GetRenderOffset();

		return { static_cast<int32>(renderOffset.x), static_cast<int32>(renderOffset.y) };
	}

	return IVec2{};
}