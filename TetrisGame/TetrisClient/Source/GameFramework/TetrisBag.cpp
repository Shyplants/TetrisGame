#include "TetrisBag.h"

TetrisBag::TetrisBag(uint64 seed)
{
	if (seed == 0)
		seed = std::random_device{}();

	m_rng.seed(seed);
	Refill();
}

TetrominoType TetrisBag::Pop()
{
	if (m_queue.empty())
		Refill();

	TetrominoType t = m_queue.front();
	m_queue.pop_front();

	return t;
}

TetrominoType TetrisBag::Peek(int32 index) const
{
	if (index < 0)
	{
		return TetrominoType::None;
	}

	while (m_queue.size() <= index)
	{
		Refill();
	}

	return m_queue[index];
}

void TetrisBag::Refill() const
{
	m_currentBag = {
		TetrominoType::I,
		TetrominoType::O,
		TetrominoType::T,
		TetrominoType::L,
		TetrominoType::J,
		TetrominoType::S,
		TetrominoType::Z
	};

	std::shuffle(m_currentBag.begin(), m_currentBag.end(), m_rng);

	for (auto t : m_currentBag)
		m_queue.push_back(t);
}
