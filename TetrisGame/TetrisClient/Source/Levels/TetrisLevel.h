#pragma once

#include "Engine/Core/World/Level.h"

class World;
class TetrisLevel : public Level
{
public:
	explicit TetrisLevel(World* world);
	~TetrisLevel() override;

	void OnLoad() override;
	void OnBeginPlay() override;
	void Tick(float deltaTime) override;
};