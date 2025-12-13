#include "ClientPCH/ClientPCH.h"
#include "Levels/TetrisLevel.h"

#include "Actors/TetrisBoardActor.h"
#include "Actors/HoldPanelActor.h"
#include "Actors/SidePanelActor.h"
#include "Actors/PreviewPanelActor.h"

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


    // 보드 왼쪽
    HoldPanelActor* holdPanelActor = nullptr;
    {
        // 홀드 패널 생성
        auto holdPanelOffset = boardActor->GetRenderOffset() + IVec2(boardActor->GetPanelWidth() / -2 + 4, boardActor->GetPanelHeight() / 2);
        holdPanelActor = world->SpawnActor<HoldPanelActor>(holdPanelOffset);
        holdPanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);
    }
    
    // 보드 오른쪽
    SidePanelActor* sidePanelActor = nullptr;
    PreviewPanelActor* previewPanelActor = nullptr;
    {
        // 사이드(오른쪽) 패널 생성
        auto sidePanelOffset = boardActor->GetRenderOffset() + IVec2(boardActor->GetPanelWidth() / 2 - 4, 0);
        sidePanelActor = world->SpawnActor<SidePanelActor>(sidePanelOffset);
        sidePanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);

        // 미리보기 패널 생성
        auto previewPanelOffset = sidePanelActor->GetRenderOffset() + IVec2(sidePanelActor->GetPanelWidth() - 4, boardActor->GetPanelHeight() / 2);
        previewPanelActor = world->SpawnActor<PreviewPanelActor>(previewPanelOffset);
        previewPanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // GameMode에 전달
    auto gameMode = world->GetGameMode<TetrisGameMode>();
    assert(gameMode && "gameMode is not valid!");

    DirectX::XMFLOAT2 renderOffset = { -160.0f, -320.0f + 2.0f };

    gameMode->SetBoardActor(boardActor);
    gameMode->SetHoldPanelActor(holdPanelActor);
    gameMode->SetSidePanelActor(sidePanelActor);
    gameMode->SetPreviewPanelActor(previewPanelActor);

    gameMode->SetRenderOffset(renderOffset);

    // boardActor->GetRootComponent()->SetLocalPosition({ +260.0f, +200.0f, 0.0f });
}

void TetrisLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);
}
