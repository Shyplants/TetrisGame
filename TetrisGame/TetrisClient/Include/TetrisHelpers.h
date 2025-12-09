#pragma once

#include "TetrisTypes.h"

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