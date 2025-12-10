#include "ClientPCH/ClientPCH.h"
#include "Actors/TetrominoActor.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

#include "TetrisRules.h"
#include "Components/Renderer/TetrominoRendererComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TetrominoActor::TetrominoActor(TetrominoType type)
	: Actor()
	, m_type(type)
{
}

TetrominoActor::~TetrominoActor()
{
}

void TetrominoActor::OnSpawned()
{
	m_renderer = AddComponent<TetrominoRendererComponent>(this);
	m_renderer->SetTexture(ResourceManager::Get().Load<Texture>(L"../Resources/White.png").get());
}

Vector2 TetrominoActor::GetPos() const
{
	int32 x = GetX();
	int32 y = GetY();

	return Vector2(static_cast<float>(x), static_cast<float>(y));
}

int32 TetrominoActor::GetX() const
{
	auto& pos = GetRootComponent()->GetLocalPosition();

	return static_cast<int32>(pos.x);
}

int32 TetrominoActor::GetY() const
{
	auto& pos = GetRootComponent()->GetLocalPosition();

	return static_cast<int32>(pos.y);
}

void TetrominoActor::Move(int32 dx, int32 dy)
{
	auto& pos = GetRootComponent()->GetLocalPosition();
	
	SetPos({ pos.x + static_cast<float>(dx), pos.y + static_cast<float>(dy) });
}

void TetrominoActor::SetPos(Vector2 pos)
{
	GetRootComponent()->SetLocalPosition({ pos.x, pos.y, 0.0f });
}

void TetrominoActor::SetPos(int32 x, int32 y)
{
	SetPos({ static_cast<float>(x), static_cast<float>(y) });
}

void TetrominoActor::Rotate(bool cw)
{
	m_rotation = cw ? NextCW(m_rotation) : NextCCW(m_rotation);
}

std::array<Vector2, MINO_COUNT> TetrominoActor::GetCurBlocks() const
{
	if (m_type == TetrominoType::None)
	{
		// TODO : Log
		return std::array<Vector2, MINO_COUNT>();
	}

	size_t typeIndex = static_cast<size_t>(m_type) - 1;
	size_t rotIndex = static_cast<size_t>(m_rotation);
	return TetrisRules::Shapes[typeIndex][rotIndex];
}

std::array<Vector2, MINO_COUNT> TetrominoActor::GetRotBlocks(Rotation rot) const
{
	if (m_type == TetrominoType::None)
	{
		// TODO : Log
		return std::array<Vector2, MINO_COUNT>();
	}

	size_t typeIndex = static_cast<size_t>(m_type) - 1;
	size_t rotIndex = static_cast<size_t>(rot);
	return TetrisRules::Shapes[typeIndex][rotIndex];
}

void TetrominoActor::SetRenderOffset(const XMFLOAT2& offset)
{
	if (m_renderer)
		m_renderer->SetRenderOffset(offset);
}