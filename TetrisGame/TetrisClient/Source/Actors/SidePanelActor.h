#pragma once

#include "Engine/Core/World/Actor.h"
#include "Common/Types.h"
#include "Common/Math/MathTypes.h"

class SpriteRendererComponent;

class SidePanelActor : public Actor
{
public:
	explicit SidePanelActor(IVec2 offset);
	~SidePanelActor() override;

	void OnSpawned() override;

public:
	int32 GetPanelWidth() const { return m_panelWidth; }
	int32 GetPanelHeight() const { return m_panelHeight; }

	IVec2 GetRenderOffset() const;

private:
	SpriteRendererComponent* m_panelRenderer{ nullptr };

private:
	int32 m_panelWidth{}, m_panelHeight{};
	IVec2 m_offset{};
};