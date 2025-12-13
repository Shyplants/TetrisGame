#pragma once

#include "TetrisTypes.h"
#include "DirectXMath.h"
#include <array>
#include <algorithm>

inline int32 ColorFromType(TetrominoType type)
{
    switch (type)
    {
    case TetrominoType::I: return COLOR_I;
    case TetrominoType::O: return COLOR_O;
    case TetrominoType::T: return COLOR_T;
    case TetrominoType::L: return COLOR_L;
    case TetrominoType::J: return COLOR_J;
    case TetrominoType::S: return COLOR_S;
    case TetrominoType::Z: return COLOR_Z;
    default:               return COLOR_EMPTY;
    }
}

inline Rotation NextCW(Rotation r)
{
    switch (r)
    {
    case Rotation::R0:   return Rotation::R90;
    case Rotation::R90:  return Rotation::R180;
    case Rotation::R180: return Rotation::R270;
    case Rotation::R270: return Rotation::R0;
    }
    return Rotation::R0;
}

inline Rotation NextCCW(Rotation r)
{
    switch (r)
    {
    case Rotation::R0:   return Rotation::R270;
    case Rotation::R90:  return Rotation::R0;
    case Rotation::R180: return Rotation::R90;
    case Rotation::R270: return Rotation::R180;
    }
    return Rotation::R0;
}

inline Rotation NextRotation(Rotation cur, Rotation theta, bool cw)
{
    int32 c = static_cast<int32>(cur);
    int32 t = static_cast<int32>(theta);
    t *= (cw ? 1 : -1);

    int32 next = (c + t + 4) % 4;

    return static_cast<Rotation>(next);
}

inline DirectX::XMFLOAT2 ComputeCenteringOffset(const std::array<IVec2, MINO_COUNT>& blocks)
{
    int32 minX, maxX;
    int32 minY, maxY;

    minX = maxX = blocks[0].x;
    minY = maxY = blocks[0].y;


    for (auto& b : blocks)
    {
        minX = std::min(minX, b.x);
        maxX = std::max(maxX, b.x);
        minY = std::min(minY, b.y);
        maxY = std::max(maxY, b.y);
    }

    // 중심 = (min + max) / 2
    float centerX = static_cast<float>(minX + maxX) * 0.5f;
    float centerY = static_cast<float>(minY + maxY) * 0.5f;

    // BottomLeft -> MiddleCenter
    centerX += 0.5f;
    centerY += 0.5f;

    // 가운데 배치하기 위해 -center를 오프셋으로 반환
    return DirectX::XMFLOAT2{-centerX, -centerY};
}