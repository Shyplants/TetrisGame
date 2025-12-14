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
    SP_ASSERT(world != nullptr);

    // 리소스 처음 로드시점
    SP_ASSERT(nullptr != ResourceManager::Get().Load<Texture>(L"../Resources/TileTexture.png", TextureColorSpace::Linear));

    // camera
	auto camActor = world->SpawnActor<Actor>();
    SP_ASSERT(camActor != nullptr);

	auto camComp = camActor->AddComponent<CameraComponent2D>();
    SP_ASSERT(camComp != nullptr);

    camActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, -10.0f });
    camComp->SetZoom(1.0f);
	world->SetMainCamera(camComp);

    // background
    auto bgActor = world->SpawnActor<Actor>();
    SP_ASSERT(bgActor != nullptr);

    auto bgSpriteComp = bgActor->AddComponent<SpriteRendererComponent>();
    SP_ASSERT(bgSpriteComp != nullptr);

    bgActor->GetRootComponent()->SetLocalPosition({ 0.0, 0.0f, 100.0f });
    auto bgTexture = ResourceManager::Get().Load<Texture>(L"../Resources/Background.jpg");
    SP_ASSERT(bgTexture != nullptr);
    bgSpriteComp->SetTexture(bgTexture.get());

    // game mode
    world->SetGameMode(std::make_unique<TetrisGameMode>(world));
}

void TetrisLevel::OnBeginPlay()
{
    Level::OnBeginPlay();
    SP_LOG(LogGame, ELogLevel::Info, "TetrisLevel::OnBeginPlay called");

    World* world = GetWorld();
    SP_ASSERT(world != nullptr);

    // 보드 생성
    auto boardActor = world->SpawnActor<TetrisBoardActor>();
    SP_ASSERT(boardActor != nullptr);

    // 보드 왼쪽
    HoldPanelActor* holdPanelActor = nullptr;
    {
        // 홀드 패널 생성
        auto holdPanelOffset = boardActor->GetRenderOffset() + IVec2(boardActor->GetPanelWidth() / -2 + 4, boardActor->GetPanelHeight() / 2);
        holdPanelActor = world->SpawnActor<HoldPanelActor>(holdPanelOffset);
        SP_ASSERT(holdPanelActor != nullptr);

        holdPanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);
    }
    
    // 보드 오른쪽
    SidePanelActor* sidePanelActor = nullptr;
    PreviewPanelActor* previewPanelActor = nullptr;
    {
        // 사이드(오른쪽) 패널 생성
        auto sidePanelOffset = boardActor->GetRenderOffset() + IVec2(boardActor->GetPanelWidth() / 2 - 4, 0);
        sidePanelActor = world->SpawnActor<SidePanelActor>(sidePanelOffset);
        SP_ASSERT(sidePanelActor != nullptr);

        sidePanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);

        // 미리보기 패널 생성
        auto previewPanelOffset = sidePanelActor->GetRenderOffset() + IVec2(sidePanelActor->GetPanelWidth() - 4, boardActor->GetPanelHeight() / 2);
        previewPanelActor = world->SpawnActor<PreviewPanelActor>(previewPanelOffset);
        SP_ASSERT(previewPanelActor != nullptr);

        previewPanelActor->AttachToActor(boardActor, FAttachmentTransformRules::KeepRelativeTransform);
    }

    // GameMode에 전달
    auto gameMode = world->GetGameMode<TetrisGameMode>();
    SP_ASSERT(gameMode != nullptr);

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
