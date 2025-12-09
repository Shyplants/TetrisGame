#pragma once

#include "Common/Types.h"

// 테트리스 미노 타입
enum class TetrominoType : int32
{
    None = 0,
    I, O, T, L, J, S, Z
};

// 회전
enum class Rotation : int32
{
    R0 = 0,
    R90,
    R180,
    R270
};

// 회전 헬퍼 함수
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

// 움직임
enum class MoveDir : int32
{
    Left = -1,
    None = 0,
    Right = 1
};

// 색상 코드
enum ColorCode : int32
{
    COLOR_I = 9,
    COLOR_O = 6,
    COLOR_T = 5,
    COLOR_L = 12,
    COLOR_J = 1,
    COLOR_S = 2,
    COLOR_Z = 4,
    COLOR_EMPTY = 0
};



// 테트리스 규칙 기반 상수
constexpr int32 ROTATION_COUNT = 4;
constexpr int32 MINO_COUNT = 4;
constexpr int32 MINO_PREVIEW_COUNT = 4; // 혹은 5 (게임 규칙에 따라 조정)
constexpr int32 MINO_TYPE_COUNT = 7;

constexpr int32 JLSTZ_OFFSET_COUNT = 5;
constexpr int32 I_OFFSET_COUNT = 5;
constexpr int32 O_OFFSET_COUNT = 1;
