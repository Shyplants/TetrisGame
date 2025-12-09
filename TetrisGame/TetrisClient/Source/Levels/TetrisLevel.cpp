#include "ClientPCH/ClientPCH.h"
#include "Levels/TetrisLevel.h"

#include "Actors/TetrisBoardActor.h"
#include "Components/Renderer/BoardRendererComponent.h"

#include "GameFramework/TetrisGameMode.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Actor.h"

#include "Engine/Core/Component/CameraComponent2D.h"

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

	Actor* camActor = world->SpawnActor<Actor>();
	auto cam = camActor->AddComponent<CameraComponent2D>(camActor);

    camActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, -10.0f });
    cam->SetZoom(1.0f);

	world->SetMainCamera(cam);

    world->SetGameMode(std::make_unique<TetrisGameMode>(world));
}

void TetrisLevel::OnBeginPlay()
{
    Level::OnBeginPlay();

    World* world = GetWorld();

    // º¸µå »ý¼º
    auto board = world->SpawnActor<TetrisBoardActor>();
    board->GetRootComponent()->SetLocalPosition({ -160.0f, -368.0f, 0.0f });

    // ·»´õ¸µ ÄÄÆ÷³ÍÆ® ºÎÂø
    auto boardRenderer = board->AddComponent<BoardRendererComponent>(board);
    boardRenderer->SetBoard(board);
    boardRenderer->SetBlockTexture(ResourceManager::Get().Load<Texture>(L"../Resources/White.png").get());

    // GameMode¿¡ Àü´Þ
    auto gameMode = world->GetGameMode<TetrisGameMode>();
    assert(gameMode && "gameMode is not valid!");

    gameMode->SetBoard(board);
    gameMode->StartGame();
}

void TetrisLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
}
