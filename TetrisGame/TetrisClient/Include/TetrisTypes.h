#pragma once

#include "Common/Types.h"
#include "Common/Math/MathTypes.h"

// 테트리스 미노 타입
enum class TetrominoType : int32
{
    None = 0,
    I, O, T, L, J, S, Z,
    Ghost,
    TEMP_0,
    TEMP_1,
    TEMP_2,
    KillMark
};

inline int32 GetTileIndex(TetrominoType type)
{
    return static_cast<int32>(type) - 1;
}

inline bool IsMinoType(TetrominoType type)
{
    return (type >= TetrominoType::I && type <= TetrominoType::Z);
}

inline int32 GetMinoShapeIndex(TetrominoType type)
{
    if (!IsMinoType(type))
        return -1;

    return static_cast<int32>(type) - 1;
}


// 회전
enum class Rotation : int32
{
    R0 = 0,
    R90,
    R180,
    R270
};

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
constexpr int32 MINO_PREVIEW_COUNT = 5;
constexpr int32 MINO_TYPE_COUNT = 7;

constexpr int32 JLSTZ_OFFSET_COUNT = 5;
constexpr int32 I_OFFSET_COUNT = 5;
constexpr int32 O_OFFSET_COUNT = 1;

constexpr int32 CELL_SIZE = 32;
