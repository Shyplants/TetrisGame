#include "ClientPCH/ClientPCH.h"
#include "Actors/TetrominoActor.h"

#include "Engine/Core/Component/SceneComponent.h"
#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

#include "TetrisRules.h"
#include "TetrisHelpers.h"
#include "Components/Renderer/TetrominoRendererComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TetrominoActor::TetrominoActor(TetrominoType type)
	: Actor()
	, m_type(type)
{
}

TetrominoActor::~TetrominoActor() = default;

void TetrominoActor::OnSpawned()
{
	m_renderer = AddComponent<TetrominoRendererComponent>();
	SP_ASSERT(m_renderer != nullptr);

	auto tileTexture = ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png");
	SP_ASSERT(tileTexture != nullptr);

	m_renderer->SetTexture(tileTexture.get());
}

IVec2 TetrominoActor::GetPos() const
{
	int32 x = GetX();
	int32 y = GetY();

	return IVec2{ x, y };
}

int32 TetrominoActor::GetX() const
{
	auto pos = GetRootComponent()->GetLocalPosition();
	
	return static_cast<int32>(pos.x);
}

int32 TetrominoActor::GetY() const
{
	auto pos = GetRootComponent()->GetLocalPosition();

	return static_cast<int32>(pos.y);
}

void TetrominoActor::Move(int32 dx, int32 dy)
{
	auto pos = GetRootComponent()->GetLocalPosition();
	
	auto x = static_cast<int32>(pos.x) + dx;
	auto y = static_cast<int32>(pos.y) + dy;

	SetPos({x, y});
}

void TetrominoActor::SetPos(IVec2 pos)
{
	auto fx = static_cast<float>(pos.x);
	auto fy = static_cast<float>(pos.y);

	GetRootComponent()->SetLocalPosition({ fx, fy, 0.0f });
}

void TetrominoActor::SetPos(int32 x, int32 y)
{
	SetPos({ x,y });
}

void TetrominoActor::Rotate(bool cw)
{
	m_rotation = cw ? NextCW(m_rotation) : NextCCW(m_rotation);
}

std::array<IVec2, MINO_COUNT> TetrominoActor::GetCurrentBlocks() const
{
	return TetrisRules::GetShapeBlocks(m_type, m_rotation);
}

std::array<IVec2, MINO_COUNT> TetrominoActor::GetRelativeRotatedBlocks(Rotation theta, bool cw) const
{
	auto rot = NextRotation(m_rotation, theta, cw);

	return TetrisRules::GetShapeBlocks(m_type, rot);
}

std::array<IVec2, MINO_COUNT> TetrominoActor::GetCurrentWorldBlocks() const
{
	auto blocks = GetCurrentBlocks();
	auto pos = GetPos();

	for (auto& block : blocks)
		block += pos;

	return blocks;
}

std::array<IVec2, MINO_COUNT> TetrominoActor::GetRelativeRotatedWorldBlocks(Rotation theta, bool cw) const
{
	auto blocks = GetRelativeRotatedBlocks(theta, cw);
	auto pos = GetPos();

	for (auto& b : blocks)
		b += pos;

	return blocks;
}

void TetrominoActor::SetRenderOffset(const XMFLOAT2& offset)
{
	if (m_renderer)
		m_renderer->SetRenderOffset(offset);
}

void TetrominoActor::SetGhostMode(bool isGhost)
{
	m_isGhost = isGhost;
}
