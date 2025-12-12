#include "ClientPCH/ClientPCH.h"
#include "Levels/TetrisLevel.h"

#include "Actors/TetrisBoardActor.h"
#include "Components/Renderer/BoardRendererComponent.h"

#include "GameFramework/TetrisGameMode.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"

#include "Engine/Core/Component/CameraComponent2D.h"
#include "Engine/Core/Component/SpriteRendererComponent.h"

#include "Engine/Resource/ResourceManager.h"
#include "Engine/Resource/Texture/Texture.h"

TetrisLevel::TetrisLevel(World* world)
	: Level(world)
{
}

TetrisLevel::~TetrisLevel()
{
}

void TetrisLevel::OnLoad()
{
	World* world = GetWorld();

    // camera
	auto camActor = world->SpawnActor<Actor>();
	auto camComp = camActor->AddComponent<CameraComponent2D>();

    camActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, -10.0f });
    camComp->SetZoom(1.0f);
	world->SetMainCamera(camComp);

    // background
    auto bgActor = world->SpawnActor<Actor>();
    auto bgSpriteComp = bgActor->AddComponent<SpriteRendererComponent>();

    bgActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, 100.0f });
    bgSpriteComp->SetTexture(ResourceManager::Get().Load<Texture>(L"../Resources/Background.jpg").get());

    // game mode
    world->SetGameMode(std::make_unique<TetrisGameMode>(world));
}

void TetrisLevel::OnBeginPlay()
{
    Level::OnBeginPlay();

    World* world = GetWorld();

    // 보드 생성
    auto boardActor = world->SpawnActor<TetrisBoardActor>();

    // GameMode에 전달
    auto gameMode = world->GetGameMode<TetrisGameMode>();
    assert(gameMode && "gameMode is not valid!");

    DirectX::XMFLOAT2 renderOffset = { -160.0f, -320.0f + 2.0f };

    gameMode->SetBoard(boardActor);
    gameMode->SetRenderOffset(renderOffset);

    // boardActor->GetRootComponent()->SetLocalPosition({ 60.0f, +200.0f, 0.0f });
}

void TetrisLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
}
