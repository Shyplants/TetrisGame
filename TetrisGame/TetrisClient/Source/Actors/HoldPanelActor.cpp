#include "ClientPCH/ClientPCH.h"
#include "Actors/HoldPanelActor.h"

#include "Engine/Core/Component/SpriteRendererComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

#include "Components/Renderer/TetrominoRendererComponent.h"
#include "TetrisRules.h"

HoldPanelActor::HoldPanelActor(IVec2 offset)
	: Actor()
	, m_offset(offset)
{
}

HoldPanelActor::~HoldPanelActor()
{
}

void HoldPanelActor::OnSpawned()
{
	auto panelTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Tetris/HoldPanel.png").get();

	float offsetX = static_cast<float>(m_offset.x);
	float offsetY = static_cast<float>(m_offset.y);

	m_panelRenderer = AddComponent<SpriteRendererComponent>();
	m_panelRenderer->SetTexture(panelTexture);
	m_panelRenderer->SetPivot(SpritePivot::TopRight);
	m_panelRenderer->SetRenderOffset({ offsetX , offsetY });

	// È¦µå ¹Ì³ë ·»´õ·¯
	offsetX += static_cast<float>(panelTexture->GetWidth()) / -2.0f;
	offsetY += static_cast<float>(panelTexture->GetHeight()) / -2.0f;
	m_holdMinoRenderer = AddComponent<TetrominoRendererComponent>();
	m_holdMinoRenderer->SetTexture(ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png").get());
	m_holdMinoRenderer->SetRenderMode(ETetrominoRenderMode::UI);
	m_holdMinoRenderer->SetTetromino(TetrominoType::None);
	m_holdMinoRenderer->SetRenderOffset({ offsetX , offsetY });
}

IVec2 HoldPanelActor::GetRenderOffset()
{
	if (m_panelRenderer)
	{
		auto renderOffset = m_panelRenderer->GetRenderOffset();

		return { static_cast<int32>(renderOffset.x), static_cast<int32>(renderOffset.y) };
	}

	return IVec2{};
}

void HoldPanelActor::UpdateHoldMino(TetrominoType type, bool bHasHeldThisTurn)
{
	m_holdMinoRenderer->SetTetromino(type);
}