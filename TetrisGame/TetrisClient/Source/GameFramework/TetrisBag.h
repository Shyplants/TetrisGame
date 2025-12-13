#pragma once

#include "TetrisTypes.h"
#include <deque>
#include <random>
#include <vector>

class TetrisBag
{
public:
	explicit TetrisBag(uint64 seed = 0);

	// 다음 미노 하나 꺼냄
	TetrominoType Pop();

	// 미리보기용 (파괴 없이 확인)
	TetrominoType Peek(int32 index) const;

private:
	// 미노 7개가 모두 비었으면 새 가방 생성
	void Refill() const;

private:
	mutable std::vector<TetrominoType> m_currentBag;  // 7개 미노 셋
	mutable std::deque<TetrominoType> m_queue;        // 게임이 꺼내 쓰는 큐
	mutable std::mt19937_64 m_rng;                    // 랜덤 생성기

};